/*!
 * \file main.cpp
 * \brief Main program entry.
 * \author Vincent de RIBOU
 * \version 0.1
 */
 
#include "MyAppClass.hpp"

using namespace Morpho::MA5G::Example;

int main(int ac, char **av) {
	MyAppClass mc;
	(void)ac;
	(void)av;
	
	mc.doIt();
	
	// avoind thread none completion
	sleep(20);
	
	return 0;	
}
