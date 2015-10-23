
#ifndef _MA5G_THREAD_H_
#define _MA5G_THREAD_H_

#include "MA5G_ThreadAbstract.hpp"

namespace Morpho {
namespace MA5G {
namespace Thread {

class MA5G_Thread : public MA5G_ThreadAbstract {

public:
	MA5G_Thread(const int type = MA5G_ThreadAbstract::LongTerm);
	~MA5G_Thread();
	
private:

};

} // namespace Thread;
} // namespace MA5G;
} // namespace Morpho;

#endif // _MA5G_THREAD_H_
