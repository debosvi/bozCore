/// @file appender_type_socket.c
/// @brief Logger socket appender implementation
/// @author MEP
/// @classification FR - INDUSTRY CONFIDENTIAL MORPHO [MEP]
/// @copyright Copyright Morpho 2013 - all rights reserved.
/// @license GNU Lesser General Public License v2.1 - See the COPYING file for the terms of usage and distribution

# define TEMP_FAILURE_RETRY(expression) \
  (__extension__                                                              \
    ({ long int __result;                                                     \
       do __result = (long int) (expression);                                 \
       while (__result == -1L && errno == EINTR);                             \
       __result; }))


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>    /* for strtol */
#include <string.h>    /* for strdup, strlen */
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>     /* for errno var */
#include <fcntl.h>     /* for fcntl */
#include <time.h>      /* for nanosleep */
#ifdef WIN32
#include <Winsock2.h>  /* for sockaddr_in struct, inet_pton */
#else
#include <netinet/in.h> /* for sockaddr_in struct */
#include <arpa/inet.h> /* for inet_pton */
#endif
#include <unistd.h>    /* for write, close */
#include <pthread.h>   /* for pthread_mutex_t */
#include <appender_type_socket.h>

#include <sd/sprintf.h>
#include <sd/sd_xplatform.h>
#include <sd/malloc.h>
#include <sd/error.h>


/* Internal structs that defines the conf and the state info
 * for an instance of the appender_type_socket type.
 */
struct __socket_conf {
  struct sockaddr_in sc_sock_addr; /* socket address or name */
  const char* s_port;
  const char* s_ipaddress;
};

struct __MEP_socket_udata {
  int sock;  /* socket = file descriptor */
  struct __socket_conf s_conf;
  pthread_mutex_t s_mutex;
};

#define NB_CONNECT_ATTEMPS   2

/***************************************************************************
     Internet socket appender interface functions: open, append, close

****************************************************************************/

/*******************************************************************************/
static int MEP_socket_open(log4c_appender_t* this)
{
    int rc = 0;
    const struct timespec requested_time = {0, 1000000};  /* 1 ms */
    struct timespec remaining = {0, 0};
    MEP_socket_udata_t *sudatap = log4c_appender_get_udata(this);

    sd_debug("MEP_socket_open[");

    if (sudatap == NULL)
    {
      /* No info provided so set defaults */
      sd_debug("making new MEP_socket conf object, with default port/IP address");
      sudatap = MEP_socket_make_udata();
      if (sudatap)
      {
        int res1, res2;
        res1 = MEP_socket_udata_set_port(sudatap, SOCKET_DEFAULT_PORT);
        res2 = MEP_socket_udata_set_ipaddress(sudatap, SOCKET_DEFAULT_IP_ADDRESS);
        if ((!res1) && (!res2))
        {
          log4c_appender_set_udata(this, sudatap);
        }
        else
        {
          sd_error("unable to set socket default port and IP address");
          sd_debug("]");
          return(-1);
        }
      }
      else
      {
        sd_error("socket user data invalid");
        sd_debug("]");
        return(-1);
      }
    }
    else
    {
      sd_debug("appender already configured : port='%s' ip_address='%s'",
      MEP_socket_udata_get_port(sudatap),
      MEP_socket_udata_get_ipaddress(sudatap));
    }

    pthread_mutex_init(&sudatap->s_mutex, NULL);

    /* Create the socket :
     * internet namespace PF_INET,
     * communication style SOCK_STREAM,
     * default protocol = 0 (TCP)
     */
    sudatap->sock = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 != sudatap->sock)
    {
      sd_debug("socket opened");
      /* To avoid blocking when trying to connect with distant server if not found... */
      fcntl (sudatap->sock, F_SETFL, O_NONBLOCK);
      /* Connect to the server in nonblocking mode ; typical case :
       *  1rst attempt : errno 115=EINPROGRESS "Operation now in progress"
       *  2d   attempt : success
       */
      int attempts = 0;
      do
      {
    	attempts++;
        rc = connect(sudatap->sock,
                     & sudatap->s_conf.sc_sock_addr,
                     sizeof(sudatap->s_conf.sc_sock_addr));
        if (-1 == rc)
        {
          sd_debug("Attempt %d to connect to server, errno = %d", attempts, errno);
          nanosleep(&requested_time, &remaining); /* wait requested_time */
        }
        else
        {
          sd_debug("connexion to server succeed");
          break;
        }
      } while (attempts < NB_CONNECT_ATTEMPS);

      if (-1 == rc)
      {
        close(sudatap->sock);
      }

    }
    else
    {
      sd_error("unable to create socket, errno = %d", errno);
      rc = -1;
    }

    sd_debug("]");
    return(rc);
}

/*******************************************************************************/
static int MEP_socket_append(log4c_appender_t*	this,
				const log4c_logging_event_t* a_event)
{
    int rc = 0;
    size_t len;
    ssize_t nbytes;
    MEP_socket_udata_t* sudatap = log4c_appender_get_udata(this);

    sd_debug("MEP_socket_append[");

    if (sudatap)
    {
      pthread_mutex_lock(&sudatap->s_mutex);  /***** LOCK ****/

      /* Write the event log to the server */
      len = strlen(a_event->evt_rendered_msg)+1;
      /* TEMP_FAILURE_RETRY evaluate expression,
       * and repeat as long as it returns -1 with `errno' set to EINTR.  */
      nbytes = TEMP_FAILURE_RETRY(write(sudatap->sock,
                                        a_event->evt_rendered_msg,
                                        len));
      if (-1 == nbytes)
      {
        sd_error("unable to write in socket, errno = %d", errno);
        rc = -1;
      }
      else
      {
        if (len == nbytes)
        {
          sd_debug("write succeed");
        }
        else
        {
          /* nbytes < len : should loop on write ? */
          sd_debug("partial write : %d bytes", nbytes);
          rc = -1;
        }
      }
      pthread_mutex_unlock(&sudatap->s_mutex);  /****** UNLOCK *****/
    }
    else
    {
      sd_error("unable to log in socket : socket user data invalid");
      rc = -1;
    }

    sd_debug("]");
	return(rc);
}

/*******************************************************************************/
static int MEP_socket_close(log4c_appender_t*	this)
{
    int rc = 0;
    MEP_socket_udata_t* sudatap = NULL;

    sd_debug("MEP_socket_close[");
    if (!this)
    {
      rc = -1;
    }
    else
    {
      sudatap = log4c_appender_get_udata(this);

      if (sudatap)
      {
        if (sudatap->s_conf.s_port)
        {
          free((char *)sudatap->s_conf.s_port);
          sudatap->s_conf.s_port = NULL;
        }
        if (sudatap->s_conf.s_ipaddress)
        {
          free((char *)sudatap->s_conf.s_ipaddress);
          sudatap->s_conf.s_ipaddress = NULL;
        }

        /* Close the socket */
        rc = close(sudatap->sock);
        if (0 == rc)
        {
          sudatap->sock = -1;
          sd_debug("socket closed");
        }
        else
        {
          sd_error("unable to close socket, errno = %d", errno);
        }

        free(sudatap);
        sudatap = NULL;
      }
    }
    sd_debug("]");
    return(rc);
}

/****************************************************************************
              Internet socket appender specific conf functions
****************************************************************************/

MEP_socket_udata_t *MEP_socket_make_udata(void)
{
    MEP_socket_udata_t *sudatap = NULL;

    sd_debug("MEP_socket_make_udata[");
    sudatap = (MEP_socket_udata_t *)sd_calloc(1, sizeof(MEP_socket_udata_t));
    sd_debug("]");
    return(sudatap);
}

/*******************************************************************************/

int MEP_socket_udata_set_port(
                MEP_socket_udata_t *sudatap,
                char* port)
{
    long int port_number;
    char* tail;
    int rc = 0;

    sd_debug("MEP_socket_udata_set_port[");
    if (sudatap)
    {

      if(port)
      {
        sudatap->s_conf.sc_sock_addr.sin_family = AF_INET;
        sudatap->s_conf.s_port = strdup(port);
        port_number = strtol(port, &tail, 0);
        if ((port_number >= IPPORT_USERRESERVED) && (port_number <= 65535))
        {
          sudatap->s_conf.sc_sock_addr.sin_port = htons(port_number);
          sd_debug("port number is valid = %d", ntohs(sudatap->s_conf.sc_sock_addr.sin_port));
        }
        else
        {
          rc = -1;
          sd_error("port number is invalid or < IPPORT_USERRESERVED = %s", port);
        }
      }
      else
      {
        rc = -1;
        sd_debug("port number is invalid");
      }
    }
    else
    {
      rc= -1;
      sd_error("unable to set port number : socket user data invalid");
    }
    sd_debug("]");
    return(rc);
}

/*******************************************************************************/

const char * MEP_socket_udata_get_port(MEP_socket_udata_t* sudatap)
{
  return(sudatap->s_conf.s_port);
}

/*******************************************************************************/

int MEP_socket_udata_set_ipaddress(
                MEP_socket_udata_t *sudatap,
                char* ip_address)
{
    int rc = 0;

    sd_debug("MEP_socket_udata_set_ipaddress[");
    if (sudatap)
    {

      if(ip_address)
      {
        sudatap->s_conf.s_ipaddress = strdup(ip_address);
        if (inet_pton(AF_INET, ip_address, & sudatap->s_conf.sc_sock_addr.sin_addr))
        {
          sd_debug("ip_address is valid = %s", ip_address);
        }
        else
        {
          rc = -1;
          sd_error("ip_address is invalid = %s", ip_address);
        }
      }
      else
      {
        rc = -1;
        sd_error("ip_address is invalid");
      }
    }
    else
    {
      rc= -1;
      sd_error("unable to set IP address : socket user data invalid");
    }
    sd_debug("]");
    return(rc);
}

/*******************************************************************************/

const char * MEP_socket_udata_get_ipaddress(MEP_socket_udata_t* sudatap)
{
  return(sudatap->s_conf.s_ipaddress);
}

/*****************************************************************************
                           Private functions
*****************************************************************************/

/****************************************************************************/
const log4c_appender_type_t log4c_appender_type_MEP_socket = {
    "MEP_socket",
    MEP_socket_open,
    MEP_socket_append,
    MEP_socket_close
};
