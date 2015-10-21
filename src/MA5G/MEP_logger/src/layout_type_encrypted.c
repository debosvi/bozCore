/// @file layout_type_encrypted.c
/// @brief Logger encrypted layout interface
/// @author MEP
/// @classification FR - INDUSTRY CONFIDENTIAL MORPHO [MEP]
/// @copyright Copyright Morpho 2013 - all rights reserved.
/// @license GNU Lesser General Public License v2.1 - See the COPYING file for the terms of usage and distribution

#include <MEP_logger.h>
#include <layout_type_encrypted.h>
#include <sd/sprintf.h>
#include <sd/sd_xplatform.h>
#include <sd/malloc.h>
#include <sd/error.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define MAX_FILENAME_LEN 100

struct __MEP_encrypt_udata {
  char* enc_algo;
  char* enc_path;
  const EVP_CIPHER* p_cipher_algo;
};


unsigned char g_key[EVP_MAX_KEY_LENGTH]; /* 128 bits key */
unsigned char g_iv[EVP_MAX_IV_LENGTH];   /* 64 bits initialization vector */

/****************************************************************************
              Internal declarations
****************************************************************************/
/**
 * Encrypts data in base64 code.
 * msgp pointer on data to encrypt.
 * Returns pointer on encrypted data (must be deallocated by caller), NULL if error.
 */
char* MEP_encrypt(MEP_encrypt_udata_t* eudatap, const unsigned char* msgp);

/****************************************************************************
              Formatter functions
****************************************************************************/

static const char* MEP_enc_basic_format(
    const log4c_layout_t*			a_layout,
    const log4c_logging_event_t*	a_event)
{
    static char buffer[MAX_MEP_LOG_BUFFER];
    char* enc_evt_msg = NULL;
    const char* algo = NULL;
    const char notset[] = "notset";

    void* eudatap = log4c_layout_get_udata(a_layout);
    if (NULL != eudatap)
    {
      sd_debug("crypted layout configured");
      /* Encrypt a_event->evt_msg into enc_evt_msg, must be deallocated by caller */
      enc_evt_msg = MEP_encrypt(eudatap, (unsigned char*)a_event->evt_msg);
      algo = MEP_encrypt_udata_get_algo(eudatap);
    }
    else
    {
      sd_debug("crypted layout not configured");
      /* enc_evt_msg is deallocated hereafter and must be allocated in all cases */
      enc_evt_msg = strdup("unable to encrypt log msg");
      algo = notset;
    }

    if(enc_evt_msg != NULL)
    {
        snprintf(buffer, sizeof(buffer), "%-8s %s - LEN=%d ENC=%s <BTXT>%s<ETXT>\n",
          log4c_priority_to_string(a_event->evt_priority),
          a_event->evt_category,
          strlen(enc_evt_msg),
          algo,
          enc_evt_msg);

        /* Free string allocated by MEP_encrypt or strdup */
        free(enc_evt_msg);

    }

    return buffer;
}

/*******************************************************************************/

static const char* MEP_enc_dated_format(
    const log4c_layout_t*       	a_layout,
    const log4c_logging_event_t*	a_event)
{
    static char buffer[MAX_MEP_LOG_BUFFER];
    char* enc_evt_msg = NULL;
    const char* algo = NULL;
    const char notset[] = "notset";

    void* eudatap = log4c_layout_get_udata(a_layout);
    if (NULL != eudatap)
    {
      sd_debug("crypted layout configured");
      /* Encrypt a_event->evt_msg into enc_evt_msg, must be deallocated by caller */
      enc_evt_msg = MEP_encrypt(eudatap, (unsigned char*)a_event->evt_msg);
      algo = MEP_encrypt_udata_get_algo(eudatap);
    }
    else
    {
      sd_debug("crypted layout not configured");
      /* enc_evt_msg is deallocated hereafter and must be allocated in all cases */
      enc_evt_msg = strdup("unable to encrypt log msg");
      algo = notset;
    }


    if(enc_evt_msg != NULL)
    {

#ifndef _WIN32
#ifndef __HP_cc
#warning gmtime() routine should be defined in sd_xplatform
#endif
        struct tm   tm;

        gmtime_r(&a_event->evt_timestamp.tv_sec, &tm);

        snprintf(buffer, sizeof(buffer), "%04d%02d%02d %02d:%02d:%02d.%03ld %-8s %s - LEN=%d ENC=%s <BTXT>%s<ETXT>\n",
                 tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                 tm.tm_hour, tm.tm_min, tm.tm_sec,
                 a_event->evt_timestamp.tv_usec / 1000,
                 log4c_priority_to_string(a_event->evt_priority),
                 a_event->evt_category,
                 strlen(enc_evt_msg),
                 algo,
                 enc_evt_msg);
#else
        SYSTEMTIME stime;

        if ( FileTimeToSystemTime(&a_event->evt_timestamp, &stime)){
          snprintf(buffer, sizeof(buffer), "%04d%02d%02d %02d:%02d:%02d.%03ld %-8s %s - LEN=%d ENC=%s <BTXT>%s<ETXT>\n",
                   stime.wYear, stime.wMonth , stime.wDay,
                   stime.wHour, stime.wMinute, stime.wSecond,
                   stime.wMilliseconds,
                   log4c_priority_to_string(a_event->evt_priority),
                   a_event->evt_category,
                   strlen(enc_evt_msg),
                   algo,
                   enc_evt_msg);
        }
#endif

    }

    /* Free string allocated by MEP_encrypt or strdup */
    free(enc_evt_msg);

    return buffer;
}

/*******************************************************************************/
const log4c_layout_type_t log4c_layout_type_MEP_enc_basic = {
    "MEP_enc_basic",
    MEP_enc_basic_format
};

/*******************************************************************************/
const log4c_layout_type_t log4c_layout_type_MEP_enc_dated = {
    "MEP_enc_dated",
    MEP_enc_dated_format
};

/****************************************************************************
              Log encryption layout specific configuration functions
****************************************************************************/

MEP_encrypt_udata_t *MEP_encrypt_make_udata(void)
{
	MEP_encrypt_udata_t *eudatap = NULL;

    sd_debug("MEP_encrypt_make_udata[");
    eudatap = (MEP_encrypt_udata_t *)sd_calloc(1, sizeof(MEP_encrypt_udata_t));
    if (NULL != eudatap)
    {
      eudatap->enc_algo = NULL;
      eudatap->enc_path = NULL;
      eudatap->p_cipher_algo = NULL;
    }
    else
    {
        sd_error("memory allocation failed (size of MEP_encrypt_udata_t)");
    }
    sd_debug("]");
    return eudatap;
}

/*******************************************************************************/

int MEP_encrypt_udata_set_algo(
                MEP_encrypt_udata_t *eudatap,
                char* algo)
{
    int rc = 0;

    sd_debug("MEP_encrypt_udata_set_algo[");
    if (NULL != eudatap)
    {
      if (NULL != algo)
      {
        eudatap->enc_algo = strdup(algo);
      }
      else
      {
        rc = -1;
        sd_debug("invalid algo name");
      }
      if (NULL == eudatap->enc_algo)
      {
        rc = -1;
        sd_error("unable to allocate memory for enc_algo");
      }
      else
      {
        sd_debug("enc_algo is %s", eudatap->enc_algo);
      }
    }
    else
    {
      rc = -1;
      sd_error("unable to set encryption algorithm : encrypt user data invalid");
    }
    sd_debug("]");
    return rc;
}

/*******************************************************************************/

const char * MEP_encrypt_udata_get_algo(MEP_encrypt_udata_t* eudatap)
{
	if (NULL != eudatap)
	{
		return eudatap->enc_algo;
	}
	else
	{
		return NULL;
	}
}

/*******************************************************************************/

int MEP_encrypt_udata_set_path(
                MEP_encrypt_udata_t *eudatap,
                char* path)
{
    int rc = 0;

    sd_debug("MEP_encrypt_udata_set_path[");
    if (NULL != eudatap)
    {
      if (NULL != path)
      {
        eudatap->enc_path = strdup(path);
      }
      else
      {
        rc = -1;
        sd_debug("invalid path name");
      }
      if (NULL == eudatap->enc_path)
      {
        rc = -1;
        sd_error("unable to allocate memory for enc_path");
      }
      else
      {
        sd_debug("enc_path is %s", eudatap->enc_path);
      }
    }
    else
    {
      rc = -1;
      sd_error("unable to set key + init_vector path : encrypt user data invalid");
    }
    sd_debug("]");
    return rc;
}

/*******************************************************************************/

const char * MEP_encrypt_udata_get_path(MEP_encrypt_udata_t* eudatap)
{
	if (NULL != eudatap)
	{
		return eudatap->enc_path;
	}
	else
	{
		return NULL;
	}
}

/*******************************************************************************/

void MEP_encrypt_delete_udata(MEP_encrypt_udata_t* eudatap)
{
  sd_debug("MEP_encrypt_delete_udata[");
  if (NULL != eudatap)
  {
    if (NULL != eudatap->enc_algo)
    {
      free(eudatap->enc_algo);
    }
    if (NULL != eudatap->enc_path)
    {
      free(eudatap->enc_path);
    }
    free(eudatap);
  }
  sd_debug("]");
}

/****************************************************************************
              Log encryption internal functions
****************************************************************************/

/*
** Returns a pointer to new memory containing the string with the b64 coded input data.
** This will be null terminated with an extra byte.
**
** Caller must free the returned string.
 */
char* base64(const unsigned char *input, int length)
{
  BIO *b64, *bmem;
  BUF_MEM* bptr;
  char* buffer = NULL;

  b64 = BIO_new(BIO_f_base64());
  bmem = BIO_new(BIO_s_mem());
  b64 = BIO_push(b64, bmem);
  BIO_write(b64, input, length);
  BIO_flush(b64);
  BIO_get_mem_ptr(b64, &bptr);

  sd_debug("before allocate base64 buffer, len = %d", bptr->length + 1);

  buffer = (char*)malloc(bptr->length + 1);
  if (NULL != buffer)
  {
    memcpy(buffer, bptr->data, bptr->length);
    buffer[bptr->length] = 0;
  }
  else
  {
    sd_error("unable to allocate base64 buffer");
  }

  BIO_free_all(b64);

  return buffer;
}

/*******************************************************************************/

char* MEP_encrypt(MEP_encrypt_udata_t* eudatap, const unsigned char* inbuf)
{
  unsigned char *outbuf = NULL;
  int inlen, outlen, tlen, i;
  char* b64buf = NULL;
  EVP_CIPHER_CTX ctx;

  EVP_CIPHER_CTX_init(&ctx);

  /* Sets up cipher context (ctx) for encryption with cipher type (evp_cipher_algo)
   * with the default implementation.
   * key is the symmetric key to use and iv is the IV to use.
   * The actual number of bytes used for the key and IV depends on the cipher. */
  sd_debug("EVP algorithm address = 0x%.8X", eudatap->p_cipher_algo);
  if (EVP_EncryptInit_ex(&ctx, eudatap->p_cipher_algo, NULL, g_key, g_iv) != 1)
  {
    sd_error("EVP_EncryptInit_ex failed");
    return NULL;
  }

  if (NULL != inbuf)
  {
    inlen = strlen((char*)inbuf);

    /* Allocate intermediate string buffer */
    sd_debug("data to encrypt buffer length = %d, data ='%s'", inlen, inbuf);
    outbuf = malloc(inlen + MAX_MEP_LOG_BUFFER); /* arbitrary choice... TODO should be a little greater than inlen */
    if (NULL == outbuf)
    {
      sd_error("unable to allocate crypted buffer");
      return NULL;
    }

    /* Encrypt data */
    /* EVP_EncryptUpdate encrypts inlen bytes from the buffer inbuf
     * and writes the encrypted version to outbuf.
     * The actual number of bytes written is placed in outlen */
    if (EVP_EncryptUpdate(&ctx, outbuf, &outlen, inbuf, inlen) != 1)
    {
      sd_error("error in encrypt update");
      free(outbuf);
      return NULL;
    }

	/* EVP_EncryptFinal_ex encrypts the final data,
	 * that is any data than remains in a partial block */
    if (EVP_EncryptFinal_ex(&ctx, outbuf + outlen, &tlen) != 1)
    {
      sd_error("error in encrypt final");
      free(outbuf);
      return NULL;
    }
    outlen += tlen;

    /* Code encrypted data in base 64 */
    /* Performs return string buffer allocation */
    sd_debug("binary encrypted buffer length = %d", outlen);
    b64buf = base64(outbuf, outlen);
    if (NULL == b64buf)
    {
      sd_error("unable to code crypted buffer in base64");
      perror("base64");
      free(outbuf);
      return NULL;
    }
    free(outbuf);
    sd_debug("base64 buffer length = %d, data = '%s'", strlen(b64buf), b64buf);
  }

  EVP_CIPHER_CTX_cleanup(&ctx);
  return b64buf;
}

/*******************************************************************************/

int MEP_encrypt_init(MEP_encrypt_udata_t* eudatap, char* algo, char* path)
{
  int key_fd, iv_fd; /* key and init vector files descriptors */
  char algo_name[MAX_ALGO_LEN];
  char key_path[MAX_PATH_LEN+MAX_FILENAME_LEN];
  char iv_path[MAX_PATH_LEN+MAX_FILENAME_LEN];
  ssize_t len;
  const EVP_CIPHER* evp_cipher_algo = NULL;
  const char key_name[MAX_FILENAME_LEN] = "/key";
  const char iv_name[MAX_FILENAME_LEN] = "/iv";

  /* Get encryption symmetric algorithm name */
  if (NULL == algo)
  {
    sd_error("invalid algorithm name");
    return -1;
  }
  else
  {
    strcpy(algo_name, algo);
  }
  sd_debug("encrypt algorithm name = %s", algo_name);

  /* Choose EVP cipher routine (DES or AES) */
  if (0 == strcmp(algo_name, "des_ecb"))
  {
    evp_cipher_algo = EVP_des_ecb();
  }
  else if (0 == strcmp(algo_name, "des_ede"))
  {
    evp_cipher_algo = EVP_des_ede();
  }
  else if (0 == strcmp(algo_name, "des_ede3"))
  {
    evp_cipher_algo = EVP_des_ede3();
  }
  else if (0 == strcmp(algo_name, "des_ede_ecb"))
  {
    evp_cipher_algo = EVP_des_ede_ecb();
  }
  else if (0 == strcmp(algo_name, "des_ede3_ecb"))
  {
    evp_cipher_algo = EVP_des_ede3_ecb();
  }
  else if (0 == strcmp(algo_name, "des_cfb64"))
  {
    evp_cipher_algo = EVP_des_cfb64();
  }
  else if (0 == strcmp(algo_name, "des_cfb1"))
  {
    evp_cipher_algo = EVP_des_cfb1();
  }
  else if (0 == strcmp(algo_name, "des_cfb8"))
  {
    evp_cipher_algo = EVP_des_cfb8();
  }
  else if (0 == strcmp(algo_name, "des_ede_cfb64"))
  {
    evp_cipher_algo = EVP_des_ede_cfb64();
  }
  else if (0 == strcmp(algo_name, "des_ede3_cfb64"))
  {
    evp_cipher_algo = EVP_des_ede3_cfb64();
  }
  /* Due to bug in openssl des_ede3_cfb1 is disabled.
   * http://rt.openssl.org/Ticket/Display.html?id=2867&user=guest&pass=guest */
  /* else if (0 == strcmp(algo_name, "des_ede3_cfb1"))
  {
    evp_cipher_algo = EVP_des_ede3_cfb1();
  }
  */
  else if (0 == strcmp(algo_name, "des_ede3_cfb8"))
  {
    evp_cipher_algo = EVP_des_ede3_cfb8();
  }
  else if (0 == strcmp(algo_name, "des_ofb"))
  {
    evp_cipher_algo = EVP_des_ofb();
  }
  else if (0 == strcmp(algo_name, "des_ede_ofb"))
  {
    evp_cipher_algo = EVP_des_ede_ofb();
  }
  else if (0 == strcmp(algo_name, "des_ede3_ofb"))
  {
    evp_cipher_algo = EVP_des_ede3_ofb();
  }
  else if (0 == strcmp(algo_name, "des_cbc"))
  {
    evp_cipher_algo = EVP_des_cbc();
  }
  else if (0 == strcmp(algo_name, "des_ede_cbc"))
  {
    evp_cipher_algo = EVP_des_ede_cbc();
  }
  else if (0 == strcmp(algo_name, "des_ede3_cbc"))
  {
    evp_cipher_algo = EVP_des_ede3_cbc();
  }
  else if (0 == strcmp(algo_name, "aes_128_ecb"))
  {
    evp_cipher_algo = EVP_aes_128_ecb();
  }
  else if (0 == strcmp(algo_name, "aes_128_cbc"))
  {
    evp_cipher_algo = EVP_aes_128_cbc();
  }
  else if (0 == strcmp(algo_name, "aes_128_cfb1"))
  {
    evp_cipher_algo = EVP_aes_128_cfb1();
  }
  else if (0 == strcmp(algo_name, "aes_128_cfb8"))
  {
    evp_cipher_algo = EVP_aes_128_cfb8();
  }
  else if (0 == strcmp(algo_name, "aes_128_cfb128"))
  {
    evp_cipher_algo = EVP_aes_128_cfb128();
  }
  else if (0 == strcmp(algo_name, "aes_128_ofb"))
  {
    evp_cipher_algo = EVP_aes_128_ofb();
  }
  else
  {
    sd_error("invalid algorithm name : %s", algo_name);
    return -1;
  }

  /* Memorize EVP algorithm to use */
  eudatap->p_cipher_algo = evp_cipher_algo;
  sd_debug("EVP algorithm address = 0x%.8X", eudatap->p_cipher_algo);

  /* Get path name */
  if (NULL == path)
  {
    sd_error("invalid key + iv path");
    return -1;
  }
  else
  {
    strcpy(key_path, path);
    strcpy(iv_path, path);
  }
  strcat(key_path, "/");
  strcat(key_path, key_name);
  sd_debug("encrypt key file = %s", key_path);

  strcat(iv_path, "/");
  strcat(iv_path, iv_name);
  sd_debug("encrypt iv file = %s", iv_path);

  /* Get key file descriptor */
  key_fd = open(key_path, O_RDONLY);
  if (-1 == key_fd)
  {
    sd_error("unable to open encrypt key file on %s", key_path);
    perror("open");
    return -1;
  }
  else
  {
    sd_debug("open encrypt key file on %s", key_path);
  }

  /* Get init vector file descriptor */
  iv_fd = open(iv_path, O_RDONLY);
  if (-1 == iv_fd)
  {
    sd_error("unable to open encrypt init vector file on %s", iv_path);
    perror("open");
    close(key_fd);
    return -1;
  }
  else
  {
    sd_debug("open encrypt init vector file on %s", iv_path);
  }

  /* Read symmetric key file */
  len = read(key_fd, g_key, sizeof(g_key));
  if (-1 == len)
  {
    sd_error("unable to read encrypt key file on %s", key_path);
    perror("read");
    close(key_fd);
    close(iv_fd);
    return -1;
  }
  else
  {
    sd_debug("read %d bytes in encrypt key file on %s", len, key_path);
    close(key_fd);
  }

  /* Read init vector file */
  len = read(iv_fd, g_iv, sizeof(g_iv));
  if (-1 == len)
  {
    sd_error("unable to read encrypt init vector file on %s", iv_path);
    perror("read");
    close(iv_fd);
    return -1;
  }
  else
  {
    sd_debug("read %d bytes in encrypt init vector file on %s", len, iv_path);
    close(iv_fd);
  }

  return 0;
}

/*******************************************************************************/

