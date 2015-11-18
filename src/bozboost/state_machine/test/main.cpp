/*!
 * \file main.cpp
 * \brief Main program entry.
 * \author Vincent de RIBOU
 * \version 0.1
 */
 
#include <iostream>

#include "dbus_statemachine.hpp"

int main()
{
    boz::dbus_sm sm;
    
    sm.start();
    return 0;
}