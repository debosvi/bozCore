/**
 * \section LICENSE
 * Copyright &copy; 2012 Safran Morpho - Safran Morpho confidential - all rights reserved
 *
 * \section DESCRIPTION
 * contains all common include files and global variables
 *
 * \file Common_inc.h
 */

#ifndef COMMON_INC_H_
#define COMMON_INC_H_

#include <stdint.h>
#include <exception>
#include <string.h>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "MA5G_logger.h"

/**
 * \namespace Interface contains various interface names
 */
namespace Interface
{
//! ethernet interface name
const std::string ethernet = "eth0";
//! wifi interface name
const std::string wlan = "wlan0";
//! GSM interface name
const std::string gprs = "eth1";
}

/**
 * \namespace File_path various file paths
 */
namespace File_path
{

//! Certificate image of various certifications for device
const std::string certificate_img_path = "/usr/ma5g/config/regulatory_info_image";

//! path for RS485 serial port
const std::string RS485_path = "/dev/ttymxc0";
//! path for RS422 serial port
const std::string RS422_path = RS485_path;
//! path for AUX port
const std::string USB_AUX_path = "/dev/morphoUSB0";

//! GUI local socket path
const std::string GUI_path = "/tmp/GUI_local_socket";
//! webserver local socket path
const std::string web_server_path = "/tmp/webserver_local_socket";

//! Verify state machine file path
const std::string verify_state_machine_file = "../config/state_machine_sequence.json";

//! Parameter JSON file path for MA5G mode
const std::string param_file_MA5G = "../config/config_settings_MA5G.json";

//! Parameter JSON file path for MA Legacy mode
const std::string param_file_MA2G = "../config/config_settings_MA2G.json";

//! Parameter JSON file path for L1 Legacy mode
const std::string param_file_L1 = "../config/config_settings_L1.json";

//! SD card path
const std::string sd_card_device = "/dev/mmcblk0p1";
//! SD card mount path
const std::string sd_card_mount_path = "/media/mmcblk0p1/";

//! face camera file path
const std::string face_camera_device = "/dev/video1";

/************ Security Config ***********/
//! log4crc file path (configuration for logging)
#ifdef ARM
const std::string log4crc_file = "/rootfs_data/log4crc";
#else
const std::string log4crc_file = "../../x86_files/log4crc";
#endif

//! Directory path for KMS
#ifdef ARM
const std::string KMS_base_dir = "/rootfs_data/secure_data/KMS";
#else
const std::string KMS_base_dir = "./data/secure_data/KMS";
#endif

//! Directory path for 3DES key
const std::string triple_des_key_dir = KMS_base_dir + "/3des";
//! Directory path for AES128 key
const std::string aes128_key_dir = KMS_base_dir + "/aes128";
//! Directory path for L1 site key
const std::string l1_site_key_dir = KMS_base_dir + "/l1_site_key";
//! Directory path for mifare AES128 key
const std::string mifare_aes128_key_dir = KMS_base_dir + "/mifare_aes128";
//! Directory path for mifare crypto key
const std::string mifare_crypto1_key_dir = KMS_base_dir + "/mifare_crypto1";
//! Directory path for RSA1024 key
const std::string rsa1024_key_dir = KMS_base_dir + "/rsa1024";
//! Directory path for ICLASS key
const std::string iclass_key_dir = KMS_base_dir + "/iclass";
//! Directory path for KMS file
const std::string file_dir = KMS_base_dir + "/file";
//! Directory path for KMS passphrase
const std::string passphrase_dir = KMS_base_dir + "/passphrase";

//! Error log config file
#ifdef ARM
const std::string error_log_conf_file = "/rootfs_data/error_log_conf";
#else
const std::string error_log_conf_file = "../../x86_files/error_log_conf";
#endif

//! Security directory path
const std::string security_json_dir = "../security/";

//! Security JSON file path
const std::string security_json_file = security_json_dir + "security_config.json";

//! Security directory
const std::string security_dir = "../security/password/";

//! Terminal Password directory
const std::string terminal_passowrd_dir = KMS_base_dir + "/terminal_password/";
//! Password file for terminal password
const std::string terminal_password_file = terminal_passowrd_dir + "terminal.sec";

//! Passphrase file
const std::string passphrase_file = security_dir + "passphrase.sec";

//! Cryptographic key file
const std::string cryptographic_key_file = security_dir + "cryptographic.sec";

/************ Product Config ***********/
//! Directory path for product config json file
const std::string product_json_dir("../security/");

//! File path for product config json file
const std::string product_json_file(product_json_dir + "product_config.json");

/************ Product Config ***********/
/************ Database file path ***********/
#ifdef ARM
//! Path of user database file
const std::string user_db_path = "/rootfs_data/secure_data/user_database/user_data.db";
//! Path of user database mounted file
const std::string user_db_path_sd_card = "/media/mmcblk0p1/rootfs_data/secure_data/user_database/user_data.db";
//! Path of user database sync file
const std::string user_db_sync_file_path = "/rootfs_data/user_db_sync_file.txt";
#else
//! Path of user database file
const std::string user_db_path = "user_data.db";
//! Path of user database sync file
const std::string user_db_sync_file_path = "user_db_sync_file.txt";
#endif


// No need to define #ifdef ARM macro for below file as it is used on board only
//! sensor database directory path
const std::string sensor_db_directory_path = "/rootfs_data/secure_data/user_database";
//! sensor database directory path if SD card mounted
const std::string sensor_db_directory_path_sd_card = "/media/mmcblk0p1/rootfs_data/secure_data/user_database";

// Path of database internal structure configuration file (only for MA500 mode)
//! database config file path
const std::string db_config_file_path = "/rootfs_data/secure_data/user_database/db_config.json";
//! database config file path if SD card is mounted
const std::string db_config_file_path_sd_card = "/media/mmcblk0p1/rootfs_data/secure_data/user_database/db_config.json";

//! Path of data directory
#ifdef ARM
const std::string data_dir_path = "/rootfs_data";
#else
const std::string data_dir_path = "";
#endif

//! Path of configuration parameters database file
#ifdef ARM    
const std::string config_db_path = "/rootfs_data/config_data.db";
#else
const std::string config_db_path = "config_data.db";
#endif

//! Path of job code database file
#ifdef ARM       
const std::string job_code_db_path = "/rootfs_data/jobcode_list.db";
#else
const std::string job_code_db_path = "jobcode_list.db";
#endif
//! Path of dynamic message database file (Create DB only on SD card)
#ifdef ARM       
const std::string dynamic_msg_db_path = "/media/mmcblk0p1/rootfs_data/dynamic_msg.db";
#else
const std::string dynamic_msg_db_path = "dynamic_msg.db";

#endif

//! Path of black list database file
#ifdef ARM       
const std::string blacklist_db_path = "/rootfs_data/black_list.db";
#else
const std::string blacklist_db_path = "black_list.db";
#endif


#ifdef ARM
//! Path of transaction log database file
const std::string tlog_db_path = "/rootfs_data/tlog.db";
//! Path of transaction log database file id SD card is mounted
const std::string tlog_db_path_sd_card = "/media/mmcblk0p1/rootfs_data/tlog.db";

#else
//! Path of transaction log database file
const std::string tlog_db_path = "tlog.db";
#endif

/************ SSL file path ***********/
//! L1 SSL certificate path
const std::string l1_ssl_certificate_file = "../config/secure_data/L1/server.pem";

//            const std::string cert_pem_path_profile0 = "/tmp/profile0_certificate.pem";
//! path to store extracted PEM certificate for profile0
const std::string cert_pem_path_profile0 = "/tmp/profile0_certificate.pem";
//! path to store extracted CA certificate for profile0
const std::string ca_pem_path_profile0 = "/tmp/profile0_ca.pem";
//            const std::string cert_pem_path_profile1 = "/tmp/profile1_certificate.pem";
//! path to store extracted PEM certificate for profile1
const std::string cert_pem_path_profile1 = "/tmp/profile1_certificate.pem";
//! path to store extracted CA certificate for profile1
const std::string ca_pem_path_profile1 = "/tmp/profile1_ca.pem";

/********* GUI capture image path **********/
//! GUI capture image path
const std::string default_gui_capture_image_path = "/tmp/gui_screen";

//! path to store P12 file retrieved from KMS (for profile0)
const std::string temp_certificate_0_path = "/tmp/MA0.P12";
//! path to store P12 file retrieved from KMS (for profile1)
const std::string temp_certificate_1_path = "/tmp/MA1.P12";

/********* Path of directory for language file **********/
#ifdef ARM
//! Directory path for predefined translation files
const std::string predefined_dir = "/rootfs_data/Translation/Predefined";
//! Directory path for user defined translation files
const std::string userdefined_dir = "/rootfs_data/Translation/Userdefined";
//! Directory path for current translation file
const std::string current_dir = "/rootfs_data/Translation/Current";
#else
//! Directory path for predefined translation files
const std::string predefined_dir = "./data/Translation/Predefined";
//! Directory path for user defined translation files
const std::string userdefined_dir = "./data/Translation/Userdefined";
//! Directory path for current translation file
const std::string current_dir = "./data/Translation/Current";
#endif
/********* proc files **********/
//! proc mounts file
const std::string proc_mount_partition_file = "/proc/mounts";

/********* build_date_time **********/
const std::string build_datetime_file = "../config/build_date_time.txt";

}

#endif /* COMMON_INC_H_ */

