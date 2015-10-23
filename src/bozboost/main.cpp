
#include "MyAppClass.hpp"

using namespace Morpho::MA5G;

int main(int ac, char **av) {
	MyAppClass mc;
	(void)ac;
	(void)av;
	
	mc.doIt();
	
	sleep(20);
	
	return 0;	
}
