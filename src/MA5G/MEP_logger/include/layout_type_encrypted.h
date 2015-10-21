/// @file layout_type_encrypted.h
/// @brief Logger encrypted layout interface
/// @author MEP
/// @classification FR - INDUSTRY CONFIDENTIAL MORPHO [MEP]
/// @copyright Copyright Morpho 2013 - all rights reserved.
/// @license GNU Lesser General Public License v2.1 - See the COPYING file for the terms of usage and distribution

#ifndef LAYOUT_TYPE_ENCRYPTED_H_
#define LAYOUT_TYPE_ENCRYPTED_H_

/**
 * @file layout_type_encrypted.h
 *
 * @brief Implement a basic and a dated encrypted layout.
 *
 * The basic encrypted layout has the following
 * conversion pattern: @c "%P %c - %m\n".
 *
 * The dated encrypted layout has the following
 * conversion pattern: @c "%d %P %c - LEN=%l ENC=%a <BTXT>%m<ETXT>\n".
 *
 * Where
 * @li @c "%d" is the date of the logging event
 * @li @c "%P" is the priority of the logging event
 * @li @c "%c" is the category of the logging event
 * @li @c "%l" is the length of the logging event (between tags)
 * @li @c "%a" is the encryption algorithm of the logging event
 * @li @c <BTXT> and <ETXT> are the beginning and ending tags of the logging event
 * @li @c "%m" is the application supplied message (encrypted, base64 encoding)
 * associated with the logging event
 *
 *
 *
 **/

#include <log4c/layout.h>
#include <openssl/evp.h>

#ifdef __cplusplus
extern "C" {
#endif

const log4c_layout_type_t log4c_layout_type_MEP_enc_basic;

const log4c_layout_type_t log4c_layout_type_MEP_enc_dated;

typedef struct __MEP_encrypt_udata MEP_encrypt_udata_t; /* opaque */

/** @brief Maximum length for an absolute path name*/
#define MAX_PATH_LEN     512

/** @brief Maximum length for an encryption algorithm name*/
#define MAX_ALGO_LEN     16

/**
 * @brief Get a new logs encryption layout configuration object.
 * @return a new layout configuration object, otherwise NULL.
*/
MEP_encrypt_udata_t * MEP_encrypt_make_udata(void);

/**
 * @brief Set the encryption algorithm for the layout configuration.
 * @param eudatap the layout configuration object.
 * @param algo the encryption algorithm string to set.
 * @return zero if successful, non-zero otherwise.
 */
int MEP_encrypt_udata_set_algo(
                MEP_encrypt_udata_t* eudatap,
                char* algo);

/**
 * @brief Get the encryption algorithm string for the layout configuration.
 * @param eudatap the layout configuration object.
 * @return the encryption algorithm string.
 */
const char * MEP_encrypt_udata_get_algo(MEP_encrypt_udata_t* eudatap);

/**
 * @brief Set the key and initialization vector path for the layout configuration.
 * @param eudatap the layout configuration object.
 * @param path the path for key and initialization vector to use.
 * @return zero if successful, non-zero otherwise.
 */
int MEP_encrypt_udata_set_path(
                MEP_encrypt_udata_t* eudatap,
                char* path);

/**
 * @brief Get the key and initialization vector path for the layout configuration.
 * @param eudatap the layout configuration object.
 * @return the path for key and initialization vector.
 */
const char * MEP_encrypt_udata_get_path(MEP_encrypt_udata_t* eudatap);

/**
 * @brief Delete the user data allocated strings (path and algo for the layout configuration)
 *        and the layout configuration object.
 * @param eudatap the layout configuration object.
 */
void MEP_encrypt_delete_udata(MEP_encrypt_udata_t* eudatap);

/****************************************************************************
              Internal declarations
****************************************************************************/
/**
 * Initializes encryption context with openssl/EVP routines.
 * algo pointer on encryption algorithm name.
 * path pointer on encryption key and initialization vector path.
 * Returns 0 if successful, -1 else.
 */
int MEP_encrypt_init(MEP_encrypt_udata_t* eudatap, char* algo, char* path);

#ifdef __cplusplus
}
#endif

#endif /* LAYOUT_TYPE_ENCRYPTED_H_ */
