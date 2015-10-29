/**
 * \section LICENSE
 * Copyright &copy; 2012 Safran Morpho - Safran Morpho confidential - all rights reserved
 *
 * \section DESCRIPTION
 * Implementation file of Exception class for GPIO library
 *
 * \file GPIO_exception.cpp
 */

#include "GPIO_exception.h"

/**
 * \brief Constructor of GPIO_exception
 *
 * \param [in] error_msg Error message
 * \param [in] function_name name of function from where exception is thrown
 * \param [in] line_number line number from where exception is thrown
 * \param [in] extra_string extra information about error condition
 * \param [in] internal_error_code error code if required
 */
GPIO_exception::GPIO_exception(const std::string & error_msg, const std::string & function_name,
        const uint32_t line_number, const std::string & extra_string, const int32_t internal_error_code) :
        Exception(error_msg, function_name, line_number, internal_error_code)
{
    // Appends extra debug information
    error_string.append(" ");
    error_string.append(extra_string);
}

/**
 * \brief Destructor of GPIO_exception
 *
 */
GPIO_exception::~GPIO_exception() throw ()
{
}


/**
 * \brief Constructor of GPIO_invalid_mode_for_function
 *
 * \param [in] error_msg Error message
 * \param [in] function_name name of function from where exception is thrown
 * \param [in] line_number line number from where exception is thrown
 * \param [in] extra_string extra information about error condition
 * \param [in] internal_error_code error code if required
 */
GPIO_invalid_mode_for_function::GPIO_invalid_mode_for_function(const std::string & error_msg, const std::string & function_name,
        const uint32_t line_number, const std::string & extra_string, const int32_t internal_error_code) :
        GPIO_exception(error_msg, function_name, line_number, extra_string, internal_error_code)
{
    // Log exception
    MA5G_logger::log(PRIORITY_ERROR, "%s %s", __func__, error_string.data());
}

/**
 * \brief Destructor of GPIO_SDAC_dev_close_failed
 *
 */
GPIO_invalid_mode_for_function::~GPIO_invalid_mode_for_function() throw ()
{
}


/**
 * \brief Constructor of GPIO_SDAC_dev_close_failed
 *
 * \param [in] error_msg Error message
 * \param [in] function_name name of function from where exception is thrown
 * \param [in] line_number line number from where exception is thrown
 * \param [in] extra_string extra information about error condition
 * \param [in] internal_error_code error code if required
 */
GPIO_SDAC_dev_close_failed::GPIO_SDAC_dev_close_failed(const std::string & error_msg, const std::string & function_name,
        const uint32_t line_number, const std::string & extra_string, const int32_t internal_error_code) :
        GPIO_exception(error_msg, function_name, line_number, extra_string, internal_error_code)
{
    // Log exception
    MA5G_logger::log(PRIORITY_ERROR, "%s %s", __func__, error_string.data());
}

/**
 * \brief Destructor of GPIO_SDAC_dev_close_failed
 *
 */
GPIO_SDAC_dev_close_failed::~GPIO_SDAC_dev_close_failed() throw ()
{
}


/**
 * \brief Constructor of GPIO_dev_open_failed
 *
 * \param [in] error_msg Error message
 * \param [in] function_name name of function from where exception is thrown
 * \param [in] line_number line number from where exception is thrown
 * \param [in] extra_string extra information about error condition
 * \param [in] internal_error_code error code if required
 */
GPIO_dev_open_failed::GPIO_dev_open_failed(const std::string & error_msg, const std::string & function_name,
        const uint32_t line_number, const std::string & extra_string, const int32_t internal_error_code) :
        GPIO_exception(error_msg, function_name, line_number, extra_string, internal_error_code)
{
    // Log exception
    MA5G_logger::log(PRIORITY_ERROR, "%s %s", __func__, error_string.data());
}

/**
 * \brief Destructor of GPIO_dev_open_failed
 *
 */
GPIO_dev_open_failed::~GPIO_dev_open_failed() throw ()
{
}


/**
 * \brief Constructor of GPIO_set_mode_failed
 *
 * \param [in] error_msg Error message
 * \param [in] function_name name of function from where exception is thrown
 * \param [in] line_number line number from where exception is thrown
 * \param [in] extra_string extra information about error condition
 * \param [in] internal_error_code error code if required
 */
GPIO_set_mode_failed::GPIO_set_mode_failed(const std::string & error_msg, const std::string & function_name,
        const uint32_t line_number, const std::string & extra_string, const int32_t internal_error_code) :
        GPIO_exception(error_msg, function_name, line_number, extra_string, internal_error_code)
{
    // Log exception
    MA5G_logger::log(PRIORITY_ERROR, "%s %s", __func__, error_string.data());
}

/**
 * \brief Destructor of GPIO_set_mode_failed
 *
 */
GPIO_set_mode_failed::~GPIO_set_mode_failed() throw ()
{
}


/**
 * \brief Constructor of SDAC_dev_open_failed
 *
 * \param [in] error_msg Error message
 * \param [in] function_name name of function from where exception is thrown
 * \param [in] line_number line number from where exception is thrown
 * \param [in] extra_string extra information about error condition
 * \param [in] internal_error_code error code if required
 */
SDAC_dev_open_failed::SDAC_dev_open_failed(const std::string & error_msg, const std::string & function_name,
        const uint32_t line_number, const std::string & extra_string, const int32_t internal_error_code) :
        GPIO_exception(error_msg, function_name, line_number, extra_string, internal_error_code)
{
    // Log exception
    MA5G_logger::log(PRIORITY_ERROR, "%s %s", __func__, error_string.data());
}

/**
 * \brief Destructor of SDAC_dev_open_failed
 *
 */
SDAC_dev_open_failed::~SDAC_dev_open_failed() throw ()
{
}


/**
 * \brief Constructor of SDAC_set_mode_failed
 *
 * \param [in] error_msg Error message
 * \param [in] function_name name of function from where exception is thrown
 * \param [in] line_number line number from where exception is thrown
 * \param [in] extra_string extra information about error condition
 * \param [in] internal_error_code error code if required
 */
SDAC_set_mode_failed::SDAC_set_mode_failed(const std::string & error_msg, const std::string & function_name,
        const uint32_t line_number, const std::string & extra_string, const int32_t internal_error_code) :
        GPIO_exception(error_msg, function_name, line_number, extra_string, internal_error_code)
{
    // Log exception
    MA5G_logger::log(PRIORITY_ERROR, "%s %s", __func__, error_string.data());
}

/**
 * \brief Destructor of SDAC_set_mode_failed
 *
 */
SDAC_set_mode_failed::~SDAC_set_mode_failed() throw ()
{
}


/**
 * \brief Constructor of SDAC_set_params_failed
 *
 * \param [in] error_msg Error message
 * \param [in] function_name name of function from where exception is thrown
 * \param [in] line_number line number from where exception is thrown
 * \param [in] extra_string extra information about error condition
 * \param [in] internal_error_code error code if required
 */
SDAC_set_params_failed::SDAC_set_params_failed(const std::string & error_msg, const std::string & function_name,
        const uint32_t line_number, const std::string & extra_string, const int32_t internal_error_code) :
        GPIO_exception(error_msg, function_name, line_number, extra_string, internal_error_code)
{
    // Log exception
    MA5G_logger::log(PRIORITY_ERROR, "%s %s", __func__, error_string.data());
}

/**
 * \brief Destructor of SDAC_set_params_failed
 *
 */
SDAC_set_params_failed::~SDAC_set_params_failed() throw ()
{
}


/**
 * \brief Constructor of SDAC_set_relay_state_failed
 *
 * \param [in] error_msg Error message
 * \param [in] function_name name of function from where exception is thrown
 * \param [in] line_number line number from where exception is thrown
 * \param [in] extra_string extra information about error condition
 * \param [in] internal_error_code error code if required
 */
SDAC_set_relay_state_failed::SDAC_set_relay_state_failed(const std::string & error_msg, const std::string & function_name,
        const uint32_t line_number, const std::string & extra_string, const int32_t internal_error_code) :
        GPIO_exception(error_msg, function_name, line_number, extra_string, internal_error_code)
{
    // Log exception
    MA5G_logger::log(PRIORITY_ERROR, "%s %s", __func__, error_string.data());
}

/**
 * \brief Destructor of SDAC_set_relay_state_failed
 *
 */
SDAC_set_relay_state_failed::~SDAC_set_relay_state_failed() throw ()
{
}


/**
 * \brief Constructor of GPIO_set_timeout_failed
 *
 * \param [in] error_msg Error message
 * \param [in] function_name name of function from where exception is thrown
 * \param [in] line_number line number from where exception is thrown
 * \param [in] extra_string extra information about error condition
 * \param [in] internal_error_code error code if required
 */
GPIO_set_timeout_failed::GPIO_set_timeout_failed(const std::string & error_msg, const std::string & function_name,
        const uint32_t line_number, const std::string & extra_string, const int32_t internal_error_code) :
        GPIO_exception(error_msg, function_name, line_number, extra_string, internal_error_code)
{
    // Log exception
    MA5G_logger::log(PRIORITY_ERROR, "%s %s", __func__, error_string.data());
}

/**
 * \brief Destructor of GPIO_set_timeout_failed
 *
 */
GPIO_set_timeout_failed::~GPIO_set_timeout_failed() throw ()
{
}


/**
 * \brief Constructor of GPIO_set_state_failed
 *
 * \param [in] error_msg Error message
 * \param [in] function_name name of function from where exception is thrown
 * \param [in] line_number line number from where exception is thrown
 * \param [in] extra_string extra information about error condition
 * \param [in] internal_error_code error code if required
 */
GPIO_set_state_failed::GPIO_set_state_failed(const std::string & error_msg, const std::string & function_name,
        const uint32_t line_number, const std::string & extra_string, const int32_t internal_error_code) :
        GPIO_exception(error_msg, function_name, line_number, extra_string, internal_error_code)
{
    // Log exception
    MA5G_logger::log(PRIORITY_ERROR, "%s %s", __func__, error_string.data());
}

/**
 * \brief Destructor of GPIO_set_state_failed
 *
 */
GPIO_set_state_failed::~GPIO_set_state_failed() throw ()
{
}


/**
 * \brief Constructor of GPIO_set_default_state_failed
 *
 * \param [in] error_msg Error message
 * \param [in] function_name name of function from where exception is thrown
 * \param [in] line_number line number from where exception is thrown
 * \param [in] extra_string extra information about error condition
 * \param [in] internal_error_code error code if required
 */
GPIO_set_default_state_failed::GPIO_set_default_state_failed(const std::string & error_msg, const std::string & function_name,
        const uint32_t line_number, const std::string & extra_string, const int32_t internal_error_code) :
        GPIO_exception(error_msg, function_name, line_number, extra_string, internal_error_code)
{
    // Log exception
    MA5G_logger::log(PRIORITY_ERROR, "%s %s", __func__, error_string.data());
}

/**
 * \brief Destructor of GPIO_set_default_state_failed
 *
 */
GPIO_set_default_state_failed::~GPIO_set_default_state_failed() throw ()
{
}


/**
 * \brief Constructor of SDAC_door_unlock_failed
 *
 * \param [in] error_msg Error message
 * \param [in] function_name name of function from where exception is thrown
 * \param [in] line_number line number from where exception is thrown
 * \param [in] extra_string extra information about error condition
 * \param [in] internal_error_code error code if required
 */
SDAC_door_unlock_failed::SDAC_door_unlock_failed(const std::string & error_msg, const std::string & function_name,
        const uint32_t line_number, const std::string & extra_string, const int32_t internal_error_code) :
        GPIO_exception(error_msg, function_name, line_number, extra_string, internal_error_code)
{
    // Log exception
    MA5G_logger::log(PRIORITY_ERROR, "%s %s", __func__, error_string.data());
}

/**
 * \brief Destructor of SDAC_door_unlock_failed
 *
 */
SDAC_door_unlock_failed::~SDAC_door_unlock_failed() throw ()
{
}


/**
 * \brief Constructor of SDAC_door_open_schedule_set_failed
 *
 * \param [in] error_msg Error message
 * \param [in] function_name name of function from where exception is thrown
 * \param [in] line_number line number from where exception is thrown
 * \param [in] extra_string extra information about error condition
 * \param [in] internal_error_code error code if required
 */
SDAC_door_open_schedule_set_failed::SDAC_door_open_schedule_set_failed(const std::string & error_msg, const std::string & function_name,
        const uint32_t line_number, const std::string & extra_string, const int32_t internal_error_code) :
        GPIO_exception(error_msg, function_name, line_number, extra_string, internal_error_code)
{
    // Log exception
    MA5G_logger::log(PRIORITY_ERROR, "%s %s", __func__, error_string.data());
}

/**
 * \brief Destructor of SDAC_door_open_schedule_set_failed
 *
 */
SDAC_door_open_schedule_set_failed::~SDAC_door_open_schedule_set_failed() throw ()
{
}


/**
 * \brief Constructor of SDAC_set_TOM_timeout_failed
 *
 * \param [in] error_msg Error message
 * \param [in] function_name name of function from where exception is thrown
 * \param [in] line_number line number from where exception is thrown
 * \param [in] extra_string extra information about error condition
 * \param [in] internal_error_code error code if required
 */
SDAC_set_TOM_timeout_failed::SDAC_set_TOM_timeout_failed(const std::string & error_msg, const std::string & function_name,
        const uint32_t line_number, const std::string & extra_string, const int32_t internal_error_code) :
        GPIO_exception(error_msg, function_name, line_number, extra_string, internal_error_code)
{
    // Log exception
    MA5G_logger::log(PRIORITY_ERROR, "%s %s", __func__, error_string.data());
}

/**
 * \brief Destructor of SDAC_set_TOM_timeout_failed
 *
 */
SDAC_set_TOM_timeout_failed::~SDAC_set_TOM_timeout_failed() throw ()
{
}


/**
 * \brief Constructor of SDAC_set_TOM_mode_failed
 *
 * \param [in] error_msg Error message
 * \param [in] function_name name of function from where exception is thrown
 * \param [in] line_number line number from where exception is thrown
 * \param [in] extra_string extra information about error condition
 * \param [in] internal_error_code error code if required
 */
SDAC_set_TOM_mode_failed::SDAC_set_TOM_mode_failed(const std::string & error_msg, const std::string & function_name,
        const uint32_t line_number, const std::string & extra_string, const int32_t internal_error_code) :
        GPIO_exception(error_msg, function_name, line_number, extra_string, internal_error_code)
{
    // Log exception
    MA5G_logger::log(PRIORITY_ERROR, "%s %s", __func__, error_string.data());
}

/**
 * \brief Destructor of SDAC_set_TOM_mode_failed
 *
 */
SDAC_set_TOM_mode_failed::~SDAC_set_TOM_mode_failed() throw ()
{
}


