/**
 * \section LICENSE
 * Copyright &copy; 2012 Safran Morpho - Safran Morpho confidential - all rights reserved
 *
 * \section DESCRIPTION
 * Header file of Exception class for GPIO library
 *
 * \file GPIO_exception.h
 */

#ifndef GPIO_EXCEPTION_H_
#define GPIO_EXCEPTION_H_

#include "Exception.h"

/**
 * \brief Base exception class for GPIO library.
 *        All the exception classes for GPIO are derived from this class.
 *
 */
class GPIO_exception: public Exception
{

public:

    //! \brief Constructor
    GPIO_exception(const std::string & error_msg, const std::string & function_name, const uint32_t line_number,
            const std::string & extra_string, const int32_t internal_error_code = 0);

    //! \brief Destructor
    virtual ~GPIO_exception() throw () = 0;
};


/**
 * \brief Exception class for GPIO/SDAC device close failure.
 *
 */
class GPIO_invalid_mode_for_function: public GPIO_exception
{
public:

    //! \brief Constructor
    GPIO_invalid_mode_for_function(const std::string & error_msg, const std::string & function_name, const uint32_t line_number,
            const std::string & extra_string, const int32_t internal_error_code = 0);

    //! \brief Destructor
    virtual ~GPIO_invalid_mode_for_function() throw ();
};


/**
 * \brief Exception class for GPIO/SDAC device close failure.
 *
 */
class GPIO_SDAC_dev_close_failed: public GPIO_exception
{
public:

    //! \brief Constructor
    GPIO_SDAC_dev_close_failed(const std::string & error_msg, const std::string & function_name, const uint32_t line_number,
            const std::string & extra_string, const int32_t internal_error_code = 0);

    //! \brief Destructor
    virtual ~GPIO_SDAC_dev_close_failed() throw ();
};


/**
 * \brief Exception class for GPIO device open failure.
 *
 */
class GPIO_dev_open_failed: public GPIO_exception
{
public:

    //! \brief Constructor
    GPIO_dev_open_failed(const std::string & error_msg, const std::string & function_name, const uint32_t line_number,
            const std::string & extra_string, const int32_t internal_error_code = 0);

    //! \brief Destructor
    virtual ~GPIO_dev_open_failed() throw ();
};


/**
 * \brief Exception class for GPIO set mode failure.
 *
 */
class GPIO_set_mode_failed: public GPIO_exception
{
public:

    //! \brief Constructor
    GPIO_set_mode_failed(const std::string & error_msg, const std::string & function_name, const uint32_t line_number,
            const std::string & extra_string, const int32_t internal_error_code = 0);

    //! \brief Destructor
    virtual ~GPIO_set_mode_failed() throw ();
};


/**
 * \brief Exception class for SDAC device open failure.
 *
 */
class SDAC_dev_open_failed: public GPIO_exception
{
public:

    //! \brief Constructor
    SDAC_dev_open_failed(const std::string & error_msg, const std::string & function_name, const uint32_t line_number,
            const std::string & extra_string, const int32_t internal_error_code = 0);

    //! \brief Destructor
    virtual ~SDAC_dev_open_failed() throw ();
};


/**
 * \brief Exception class for SDAC set mode failure.
 *
 */
class SDAC_set_mode_failed: public GPIO_exception
{
public:

    //! \brief Constructor
    SDAC_set_mode_failed(const std::string & error_msg, const std::string & function_name, const uint32_t line_number,
            const std::string & extra_string, const int32_t internal_error_code = 0);

    //! \brief Destructor
    virtual ~SDAC_set_mode_failed() throw ();
};

/**
 * \brief Exception class for SDAC set params failure.
 *
 */
class SDAC_set_params_failed: public GPIO_exception
{
public:

    //! \brief Constructor
    SDAC_set_params_failed(const std::string & error_msg, const std::string & function_name, const uint32_t line_number,
            const std::string & extra_string, const int32_t internal_error_code = 0);

    //! \brief Destructor
    virtual ~SDAC_set_params_failed() throw ();
};

/**
 * \brief Exception class for SDAC set relay state failure.
 *
 */
class SDAC_set_relay_state_failed: public GPIO_exception
{
public:

    //! \brief Constructor
    SDAC_set_relay_state_failed(const std::string & error_msg, const std::string & function_name, const uint32_t line_number,
            const std::string & extra_string, const int32_t internal_error_code = 0);

    //! \brief Destructor
    virtual ~SDAC_set_relay_state_failed() throw ();
};

/**
 * \brief Exception class for GPIO set timeout failure.
 *
 */
class GPIO_set_timeout_failed: public GPIO_exception
{
public:

    //! \brief Constructor
    GPIO_set_timeout_failed(const std::string & error_msg, const std::string & function_name, const uint32_t line_number,
            const std::string & extra_string, const int32_t internal_error_code = 0);

    //! \brief Destructor
    virtual ~GPIO_set_timeout_failed() throw ();
};

/**
 * \brief Exception class for GPIO set state failure.
 *
 */
class GPIO_set_state_failed: public GPIO_exception
{
public:

    //! \brief Constructor
    GPIO_set_state_failed(const std::string & error_msg, const std::string & function_name, const uint32_t line_number,
            const std::string & extra_string, const int32_t internal_error_code = 0);

    //! \brief Destructor
    virtual ~GPIO_set_state_failed() throw ();
};

/**
 * \brief Exception class for GPIO set default state failure.
 *
 */
class GPIO_set_default_state_failed: public GPIO_exception
{
public:

    //! \brief Constructor
    GPIO_set_default_state_failed(const std::string & error_msg, const std::string & function_name, const uint32_t line_number,
            const std::string & extra_string, const int32_t internal_error_code = 0);

    //! \brief Destructor
    virtual ~GPIO_set_default_state_failed() throw ();
};

/**
 * \brief Exception class for SDAC door unlock failure.
 *
 */
class SDAC_door_unlock_failed: public GPIO_exception
{
public:

    //! \brief Constructor
    SDAC_door_unlock_failed(const std::string & error_msg, const std::string & function_name, const uint32_t line_number,
            const std::string & extra_string, const int32_t internal_error_code = 0);

    //! \brief Destructor
    virtual ~SDAC_door_unlock_failed() throw ();
};

/**
 * \brief Exception class for SDAC door open schedule set failure.
 *
 */
class SDAC_door_open_schedule_set_failed: public GPIO_exception
{
public:

    //! \brief Constructor
    SDAC_door_open_schedule_set_failed(const std::string & error_msg, const std::string & function_name, const uint32_t line_number,
            const std::string & extra_string, const int32_t internal_error_code = 0);

    //! \brief Destructor
    virtual ~SDAC_door_open_schedule_set_failed() throw ();
};

/**
 * \brief Exception class for SDAC set TOM timeout failure.
 *
 */
class SDAC_set_TOM_timeout_failed: public GPIO_exception
{
public:

    //! \brief Constructor
    SDAC_set_TOM_timeout_failed(const std::string & error_msg, const std::string & function_name, const uint32_t line_number,
            const std::string & extra_string, const int32_t internal_error_code = 0);

    //! \brief Destructor
    virtual ~SDAC_set_TOM_timeout_failed() throw ();
};

/**
 * \brief Exception class for SDAC set TOM mode failure.
 *
 */
class SDAC_set_TOM_mode_failed: public GPIO_exception
{
public:

    //! \brief Constructor
    SDAC_set_TOM_mode_failed(const std::string & error_msg, const std::string & function_name, const uint32_t line_number,
            const std::string & extra_string, const int32_t internal_error_code = 0);

    //! \brief Destructor
    virtual ~SDAC_set_TOM_mode_failed() throw ();
};


#endif /* GPIO_EXCEPTION_H_ */
