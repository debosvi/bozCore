
#ifndef _BOZAPP_ABSTRACT_H_
#define _BOZAPP_ABSTRACT_H_

#include <boost/signals2.hpp>
#include <boost/thread.hpp>

class bozAppAbstract {

public:
	typedef boost::signals2::signal<void (int x)> OnChange;

	~bozAppAbstract();
	
	typedef OnChange::slot_type OnChangeSlotType;
	// forward slots through Button interface to its private signal
	boost::signals2::connection doOnChange(const OnChangeSlotType & slot);

	void start(void);
	void stop(void);

	OnChange _onChange;
	
private:
	boost::thread _th;
	
	virtual void process(void) = 0;

};

#endif // _BOZAPP_ABSTRACT_H_