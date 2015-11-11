/*!
 * \file main.cpp
 * \brief Main program entry.
 * \author Vincent de RIBOU
 * \version 0.1
 */
 
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "ShadowPublic.hpp"

using namespace Bozo::Shadow;

int main()
{
    size_t i;

    ShadowPublic sp("test_name", 1);
    
    return 0;
}
