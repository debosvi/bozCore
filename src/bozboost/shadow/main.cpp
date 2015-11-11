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

#include <boost/asio.hpp>
#include "ShadowPublic.hpp"

using namespace Bozo::Shadow;

static void printEvent(const std::string name, const int val)
{
  std::cerr << __PRETTY_FUNCTION__ << "(" << name << ", " << val << ")" << std::endl;
}

int main()
{
    size_t i;

    ShadowPublic sp("test_name", 1);
    
    sp.doOnEvent(&printEvent);
    
    boost::asio::io_service timerService;

    //create and bind the timer
    boost::asio::deadline_timer timer(timerService,
    boost::posix_time::milliseconds(7000));
    timer.wait();

    return 0;
}
