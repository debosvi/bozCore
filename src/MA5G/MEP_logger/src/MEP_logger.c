/// @file MEP_logger.c
/// @brief Logger main implementation
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

#include <MEP_logger.h>
#include <rollingpolicy_type_complete.h>
#include <appender_type_socket.h>
#include <layout_type_encrypted.h>
#include <log4c/appender.h>
#include <log4c/rollingpolicy.h>
#include <log4c/rollingpolicy_type_sizewin.h>
#include <log4c/appender_type_rollingfile.h>
#include <MEP_logger_init.h>
#include <MEP_log_rc.h>



#include <string.h>   /* for strlen */
#include <stdlib.h>
#include <stdio.h>    /* for fopen, fwrite, fclose */
#include <time.h>     /* for tm, nanosleep */
#include <errno.h>    /* for errno var */
#include <fcntl.h>    /* for fcntl */
#include <ctype.h>    /* for isascii, isprint */
#ifdef HAVE_UNISTD_H
#include <unistd.h>   /* for write, close */
#endif

#include <sd/sprintf.h>
#include <sd/sd_xplatform.h>
#include <sd/malloc.h>
#include <sd/error.h>

#define NB_CONNECT_ATTEMPS   2


#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif


#define PRETTY_ROW_SIZE 16
#define PRETTY_BUF_LEN 80

/*
 * dump_hex_to_buffer - formats a blob of data to "hex ASCII" in memory
 * @buf: data blob to dump
 * @len: number of bytes in the @buf
 * @linebuf: where to put the converted data
 * @linebuflen: total size of @linebuf, including space for terminating NULL
 * @printoffset: true - print offset ; false - no offset printed 
 * @ascii: dump ascii formatted data next to hexdump
 * @prefix: address to print before hex dump, ignored if !printoffset
 *
 */
static void dump_hex_to_buffer(	const unsigned char *buf, size_t len,
								char *linebuf, size_t linebuflen,
								int printoffset, int ascii,
								unsigned long long prefix)
{
	static const char hex_asc[] = "0123456789abcdef";
	unsigned char ch;
	unsigned int j, lx = 0, ascii_column;
	int linebuflen_local = linebuflen;

	if (printoffset)
		lx += sprintf(linebuf, "0x%.8llx: ", prefix);

	if (!len)
		goto nil;

	if (len > PRETTY_ROW_SIZE)	/* limit to one line at a time */
		len = PRETTY_ROW_SIZE;

	for (j = 0; (j < len) && (lx + 3) <= linebuflen_local; j++) {
		ch = buf[j];
		linebuf[lx++] = hex_asc[(ch & 0xf0) >> 4];
		linebuf[lx++] = hex_asc[ch & 0x0f];
		linebuf[lx++] = ' ';
	}
	if (j)
		lx--;

	ascii_column = 3 * PRETTY_ROW_SIZE + 14;

	if (!ascii)
		goto nil;

	/* Spacing between hex and ASCII - ensure at least one space */
	lx += sprintf(linebuf + lx, "%*s",
			MAX((int)MIN(linebuflen_local, ascii_column) - 1 - lx, 1), " ");

	linebuf[lx++] = '|';
	for (j = 0; (j < len) && (lx + 2) < linebuflen_local; j++)
	{
		linebuf[lx++] = (isascii(buf[j]) && isprint(buf[j])) ? buf[j] : '.';
		if (buf[j] == '%')
		{
			linebuflen_local++;
			linebuf[lx++] = '%';
		}
	}

	linebuf[lx++] = '|';

nil:
	linebuf[lx++] = '\0';
}


/******************************************************************************/
int MEP_log_init(const char* config_file)
{
    int ret = 0;


    /* Specific MEP formatters (text, binary, hexadecimal, ...) initialization */
    ret = init_MEP_formatters();

    /* Specific MEP appenders (socket, ...) initialization */
    ret = init_MEP_appenders();

    ret = init_MEP_rollingpolicies();

    /* MEP library initialization */
    ret = MEP_log_init_with_conf_file(config_file);
    /*setenv("LOG4C_RCPATH", config_file, 1);
    ret = log4c_init();*/

    return ret;
}

/******************************************************************************/
int MEP_log_delete(const char* appender_name)
{
    return MEP_log_rc_delete(appender_name);
}

/******************************************************************************/
void MEP_log_hex(const log4c_category_t* a_category,
				 int a_priority,
				 const void* a_buffer, const size_t n_bytes)
{
	int i;
	char *log_buf;
	
	if (log4c_category_is_priority_enabled(a_category, a_priority)) 
	{
		log_buf = malloc(PRETTY_BUF_LEN+16);
		if(log_buf != NULL)
		{
			for (i = 0; i < n_bytes; i += PRETTY_ROW_SIZE) 
			{
				dump_hex_to_buffer( a_buffer + i, n_bytes - i,
									log_buf, PRETTY_BUF_LEN, 1, 1, i);
				//MEP_category_log(a_category, a_priority, MEP_TXT, log_buf);
                log4c_category_log(a_category, a_priority, log_buf);
			}
			
			free(log_buf);
		}
	}
}

/******************************************************************************/
int MEP_dump(const char* a_file,
             const void* a_buffer,
             const size_t n_bytes)
{
    int ret = 0;
    FILE* ofile = NULL;

    ofile = fopen(a_file,"w");
    if (NULL == ofile)
    {
      sd_error("error in opening file %s for writing", a_file);
      ret = -1;
    }
    else
    {
      size_t nbytes;
      const size_t len = n_bytes;
      nbytes = fwrite(a_buffer, 1, len, ofile);
      if (len == nbytes)
      {
        /*ret = fclose(ofile);
        if (0 != ret)
        {
          sd_error("error in closing file %s", a_file);
          ret = -3;
        }*/
      }
      else
      {
        sd_error("error writing buffer in file");
        ret = -2;
      }

      ret = fclose(ofile);
      if (0 != ret)
      {
        sd_error("error in closing file %s", a_file);
        ret = -3;
      }

    }

    return ret;
}

/******************************************************************************/
int MEP_network_dump(const struct sockaddr_in* psockaddr,
                     const void* a_buffer,
                     const size_t n_bytes)
{
    int ret = -1;
    int sock;  /* socket = file descriptor */
    const struct timespec requested_time = {0, 1000000};  /* 1 ms */
    struct timespec remaining = {0, 0};
    ssize_t len;

    /* Open the socket */
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 != sock)
    {
      sd_debug("socket opened");
      /* To avoid blocking when trying to connect with distant server if not found... */
      fcntl(sock, F_SETFL, O_NONBLOCK);
      /* Connect to the server in nonblocking mode ; typical case :
       *  1rst attempt : errno 115=EINPROGRESS "Operation now in progress"
       *  2d   attempt : success
       */
      int attempts = 0;
      do
      {
    	attempts++;
        ret = connect(sock,
                      psockaddr,
                      sizeof(*psockaddr));
        if (-1 == ret)
        {
          sd_debug("Attempt %d to connect to server, errno = %d", attempts, errno);
          nanosleep(&requested_time, &remaining); /* wait requested_time */
        }
        else
        {
          sd_debug("connexion to server succeed");

          /* Send the buffer */
          /* TEMP_FAILURE_RETRY evaluate expression,
           * and repeat as long as it returns -1 with `errno' set to EINTR.  */
          len = TEMP_FAILURE_RETRY(write(sock,
                                         a_buffer,
                                         n_bytes));
          if (-1 == len)
          {
            sd_error("unable to write in socket, errno = %d", errno);
            ret = -1;
          }
          else
          {
            if (len == n_bytes)
            {
              sd_debug("write succeed");
            }
            else
            {
              /* len < nbytes : should loop on write ? */
              sd_debug("partial write : %d bytes", len);
              ret = -1;
            }
          }

          /* Close the socket */
          ret = close(sock);
          if (0 == ret)
          {
            sd_debug("socket closed");
          }
          else
          {
            sd_error("unable to close socket, errno = %d", errno);
          }
          break;  // do {} while
        }
      } while (attempts < NB_CONNECT_ATTEMPS);
    }
    else
    {
      sd_error("unable to create socket, errno = %d", errno);
      ret = -1;
    }

    return ret;
}



/**
 * @brief MEP log request with the specified priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_priority The priority of this log message
 * @param a_MEP_type The type of MEP log message (text, binary, hexadecimal, none=log4c like)
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 *
 **/
void MEP_log(const log4c_category_t* a_category,
					  int a_priority,
					  const char* a_format,
					  ...)
{
    if (log4c_category_is_priority_enabled(a_category, a_priority)) {
      va_list va;
      va_start(va, a_format);

      log4c_category_vlog(a_category, a_priority, a_format, va);

      va_end(va);
    }
}

/**
 * @brief MEP log request with fatal priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 **/
void MEP_log_fatal(const log4c_category_t* a_category,
					  const char* a_format,
					  ...)
{
    if (log4c_category_is_priority_enabled(a_category, LOG4C_PRIORITY_FATAL)) {
      va_list va;
      va_start(va, a_format);

      log4c_category_vlog(a_category, LOG4C_PRIORITY_FATAL, a_format, va);

      va_end(va);
    }
}

/**
 * @brief MEP log request with alert priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 **/
void MEP_log_alert(const log4c_category_t* a_category,
					  const char* a_format,
					  ...)
{
    if (log4c_category_is_priority_enabled(a_category, LOG4C_PRIORITY_ALERT)) {
      va_list va;
      va_start(va, a_format);

      log4c_category_vlog(a_category, LOG4C_PRIORITY_ALERT, a_format, va);

      va_end(va);
    }
}

/**
 * @brief MEP log request with crit priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 **/
void MEP_log_crit(const log4c_category_t* a_category,
					  const char* a_format,
					  ...)
{
    if (log4c_category_is_priority_enabled(a_category, LOG4C_PRIORITY_CRIT)) {
      va_list va;
      va_start(va, a_format);

      log4c_category_vlog(a_category, LOG4C_PRIORITY_CRIT, a_format, va);

      va_end(va);
    }
}

/**
 * @brief MEP log request with error priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 **/
void MEP_log_error(const log4c_category_t* a_category,
					  const char* a_format,
					  ...)
{
    if (log4c_category_is_priority_enabled(a_category, LOG4C_PRIORITY_ERROR)) {
      va_list va;
      va_start(va, a_format);

      log4c_category_vlog(a_category, LOG4C_PRIORITY_ERROR, a_format, va);

      va_end(va);
    }
}

/**
 * @brief MEP log request with warn priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 **/
void MEP_log_warn(const log4c_category_t* a_category,
					  const char* a_format,
					  ...)
{
    if (log4c_category_is_priority_enabled(a_category, LOG4C_PRIORITY_WARN)) {
      va_list va;
      va_start(va, a_format);

      log4c_category_vlog(a_category, LOG4C_PRIORITY_WARN, a_format, va);

      va_end(va);
    }
}

/**
 * @brief MEP log request with notice priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 **/
void MEP_log_notice(const log4c_category_t* a_category,
					  const char* a_format,
					  ...)
{
    if (log4c_category_is_priority_enabled(a_category, LOG4C_PRIORITY_NOTICE)) {
      va_list va;
      va_start(va, a_format);

      log4c_category_vlog(a_category, LOG4C_PRIORITY_NOTICE, a_format, va);

      va_end(va);
    }
}

/**
 * @brief MEP log request with info priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 **/
void MEP_log_info(const log4c_category_t* a_category,
					  const char* a_format,
					  ...)
{
    if (log4c_category_is_priority_enabled(a_category, LOG4C_PRIORITY_INFO)) {
      va_list va;
      va_start(va, a_format);

      log4c_category_vlog(a_category, LOG4C_PRIORITY_INFO, a_format, va);

      va_end(va);
    }
}

/**
 * @brief MEP log request with debug priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 **/
void MEP_log_debug(const log4c_category_t* a_category,
					  const char* a_format,
					  ...)
{
    if (log4c_category_is_priority_enabled(a_category, LOG4C_PRIORITY_DEBUG)) {
      va_list va;
      va_start(va, a_format);

      log4c_category_vlog(a_category, LOG4C_PRIORITY_DEBUG, a_format, va);

      va_end(va);
    }
}

/**
 * @brief MEP log request with trace priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 **/
void MEP_log_trace(const log4c_category_t* a_category,
					  const char* a_format,
					  ...)
{
    if (log4c_category_is_priority_enabled(a_category, LOG4C_PRIORITY_TRACE)) {
      va_list va;
      va_start(va, a_format);

      log4c_category_vlog(a_category, LOG4C_PRIORITY_TRACE, a_format, va);

      va_end(va);
    }
}


/**
 * @brief MEP log library clean-up
 * @return 0 for success
 **/
int MEP_log_cleanup()
{
	return log4c_fini();
}


/**********************************************************************
 *
 * MEP message formatter with data length and tags before and after data
 *
 **********************************************************************/
static const char* MEP_basic(
    const log4c_layout_t*       a_layout,
    const log4c_logging_event_t* a_event)
{
    static char buffer[MAX_MEP_LOG_BUFFER];

    snprintf(buffer, sizeof(buffer), "%-8s %s - LEN=%d ENC=0 <BTXT>%s<ETXT>\n",
      log4c_priority_to_string(a_event->evt_priority),
      a_event->evt_category,
      strlen(a_event->evt_msg),
      a_event->evt_msg);

    return buffer;
}

static const char* MEP_dated(
    const log4c_layout_t*       a_layout,
    const log4c_logging_event_t*a_event)
{
    static char buffer[MAX_MEP_LOG_BUFFER];

#ifndef _WIN32
#ifndef __HP_cc
#warning gmtime() routine should be defined in sd_xplatform
#endif
    struct tm   tm;

    gmtime_r(&a_event->evt_timestamp.tv_sec, &tm);

    snprintf(buffer, sizeof(buffer), "%04d%02d%02d %02d:%02d:%02d.%03ld %-8s %s - LEN=%d ENC=0 <BTXT>%s<ETXT>\n",
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec,
             a_event->evt_timestamp.tv_usec / 1000,
             log4c_priority_to_string(a_event->evt_priority),
             a_event->evt_category,
             strlen(a_event->evt_msg),
             a_event->evt_msg);
#else
    SYSTEMTIME stime;

    if ( FileTimeToSystemTime(&a_event->evt_timestamp, &stime)){
      snprintf(buffer, sizeof(buffer), "%04d%02d%02d %02d:%02d:%02d.%03ld %-8s %s - LEN=%d ENC=0 <BTXT>%s<ETXT>\n",
               stime.wYear, stime.wMonth , stime.wDay,
               stime.wHour, stime.wMinute, stime.wSecond,
               stime.wMilliseconds,
               log4c_priority_to_string(a_event->evt_priority),
               a_event->evt_category,
               strlen(a_event->evt_msg),
               a_event->evt_msg);
    }
#endif

    return buffer;
}

const log4c_layout_type_t log4c_layout_type_MEP_basic  = {
    "MEP_basic",
    MEP_basic
};

const log4c_layout_type_t log4c_layout_type_MEP_dated  = {
    "MEP_dated",
    MEP_dated
};

/* Initialization of MEP formatters */

static const log4c_layout_type_t * const MEP_layout_types[] = {
    &log4c_layout_type_MEP_basic,
    &log4c_layout_type_MEP_dated,
    &log4c_layout_type_MEP_enc_basic,
    &log4c_layout_type_MEP_enc_dated
};

static int n_MEP_layout_types =
	(int)(sizeof(MEP_layout_types) / sizeof(MEP_layout_types[0]));

int init_MEP_formatters()
{
    int rc = 0;
    int i = 0;

    for (i = 0; i < n_MEP_layout_types; i++)
      log4c_layout_type_set(MEP_layout_types[i]);

    return(rc);

}

/* Initialization of MEP appenders */

static const log4c_appender_type_t * const MEP_appender_types[] = {
    &log4c_appender_type_MEP_socket
};

static int n_MEP_appender_types =
	(int)(sizeof(MEP_appender_types) / sizeof(MEP_appender_types[0]));

int init_MEP_appenders()
{
    int rc = 0;
    int i = 0;

    for (i = 0; i < n_MEP_appender_types; i++)
      log4c_appender_type_set(MEP_appender_types[i]);

    return(rc);

}


static const log4c_rollingpolicy_type_t * const MEP_rollingpolicy_types[] = {
    &log4c_rollingpolicy_type_complete
};
static size_t n_MEP_rollingpolicy_types = 
    sizeof(MEP_rollingpolicy_types) / sizeof(MEP_rollingpolicy_types[0]);

int init_MEP_rollingpolicies()
{
    int rc = 0;
    int i = 0;

    for (i = 0; i < n_MEP_rollingpolicy_types; i++)
      log4c_rollingpolicy_type_set(MEP_rollingpolicy_types[i]);

    return(rc);

}
