
#include "MA5G_Thread.hpp"

using namespace Morpho::MA5G::Thread;

class myProcessing {
private:
	boost::condition_variable cond;
	boost::mutex mut;
	int step;
	MA5G_Thread appA,appB,appC;
	
public:
	myProcessing() : step(0), appA(1), appB(2), appC(3)  {
		bozAppAbstract::OnChange::slot_type slot = boost::bind(&myProcessing::onChangeSlot, this, _1);
		
		appA.doOnChange(slot);
		appB.doOnChange(slot);
		appC.doOnChange(slot);
	}

	void onChangeSlot(int x) {
		std::cerr << __PRETTY_FUNCTION__ << ", arg: " << x << std::endl;
		boost::lock_guard<boost::mutex> lock(mut);
		step=x;
		cond.notify_one();
	}

	void wait_for(int val){
		std::cerr << __PRETTY_FUNCTION__ << ", val: " << val << std::endl;
		boost::unique_lock<boost::mutex> lock(mut);
		while(step!=val)
		{
			cond.wait(lock);
		}
	}
	
	void global_processing(void) {
		appA.start();
		wait_for(1);
		appB.start();
		wait_for(2);
		appC.start();
		wait_for(3);
	}
	
	
};

int main(int ac, char **av) {
	myProcessing mp;
	
	mp.global_processing();
}
