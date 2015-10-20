
#ifndef _BOZAPP_H_
#define _BOZAPP_H_

#include "bozAppAbstract.hpp"

class bozApp : public bozAppAbstract {

public:
	bozApp();
	~bozApp();
	
private:
	void process(void);

};

#endif // _BOZAPP_ABSTRACT_H_