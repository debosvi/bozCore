/*!
 * \file main.cpp
 * \brief Main program entry.
 * \author Vincent de RIBOU
 * \version 0.1
 */
 
#include <iostream>

#include "StateMachine.hpp"

int main() {
    boz::sm::StateMachine machine;
    
    machine.start();
    machine.stop();
    
    return 0;
}