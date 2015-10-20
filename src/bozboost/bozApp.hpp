
#ifndef _BOZAPP_H_
#define _BOZAPP_H_

#include <boost/signals2.hpp>
#include <boost/thread.hpp>

class bozApp {
	typedef boost::signals2::signal<void (int x)> OnChange;

public:
	bozApp();
	~bozApp();
	
	typedef OnChange::slot_type OnChangeSlotType;
	// forward slots through Button interface to its private signal
	boost::signals2::connection doOnChange(const OnChangeSlotType & slot);

	void start(void);
	void stop(void);
	
private:
	OnChange _onChange;
	boost::thread _th;
	
	void process(void);

};

#endif // _BOZAPP_H_