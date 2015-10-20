
#include "bozApp.hpp"

static boost::condition_variable cond;
static boost::mutex mut;
static int step=0;

static void onChangeSlotA(int x) {
	std::cout << __PRETTY_FUNCTION__ << ", arg: " << x << std::endl;
	boost::lock_guard<boost::mutex> lock(mut);
    step=1;
	cond.notify_one();
}
static void onChangeSlotB(int x) {
	std::cout << __PRETTY_FUNCTION__ << ", arg: " << x << std::endl;
	boost::lock_guard<boost::mutex> lock(mut);
    step=2;
	cond.notify_one();
}
static void onChangeSlotC(int x) {
	std::cout << __PRETTY_FUNCTION__ << ", arg: " << x << std::endl;
	boost::lock_guard<boost::mutex> lock(mut);
    step=3;
	cond.notify_one();
}

static void wait_for(int val){
	std::cout << __PRETTY_FUNCTION__ << ", val: " << val << std::endl;
	boost::unique_lock<boost::mutex> lock(mut);
	while(step!=val)
	{
		cond.wait(lock);
	}
}
	
static void global_processing(void) {
	wait_for(1);
	wait_for(2);
	wait_for(3);
}

int main(int ac, char **av) {
	bozApp appA(1);
	bozApp appB(2);
	bozApp appC(3);
	appA.doOnChange(onChangeSlotA);
	appB.doOnChange(onChangeSlotB);
	appC.doOnChange(onChangeSlotC);
	
	appA.start();
	appB.start();
	appC.start();
	
	global_processing();
}