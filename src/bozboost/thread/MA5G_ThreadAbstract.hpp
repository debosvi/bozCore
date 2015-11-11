/*!
 * \file MA5G_ThreadAbstract.hpp
 * \brief Abstract template encapsulation class interface.
 * \author Vincent de RIBOU
 * \version 0.1
 */
 
#ifndef _MA5G_THREAD_ABSTRACT_H_
#define _MA5G_THREAD_ABSTRACT_H_

#include <string>
#include <boost/thread.hpp>
  
namespace Morpho {
namespace MA5G {
namespace Thread {
	
/*! \brief Priority specific definitions.
 * Allows to decorrelate definitions from implementation values.
 */
typedef enum {
	PRIORITY_VERY_HIGH = 0,
	PRIORITY_HIGH,
	PRIORITY_NORMAL,
	PRIORITY_LOW,
	PRIORITY_COUNT
} MA5G_ThreadPriority;
	
/*! \brief Priority specific definitions.
 * Allows to decorrelate definitions from implementation values.
 */
typedef enum {
	TYPE_ONESHOT = 0,
	TYPE_LONGTERM,
	TYPE_COUNT
} MA5G_ThreadType;

/*! \class MA5G_ThreadAbstract
 * \brief Abstract template encapsulation class.
 * 
 * This mainly abstract common operations in order to manage thread workers. It embeds as much behavior as possible. 
 * By the way, specific worker sequence of program is basically considrered as one iteration over looping. This class then is responsible
 * to exit or loop when needed. 
 *
 * \note This class must be subclassed in order to represent a specific threaded work. To do so, subclass MUST implement \ref loopProcess method.
 */
class MA5G_ThreadAbstract {

public:
	// constructors and destructors
    /*!
     * \brief Constructor
     * \param name : thread name, effective when new loop iteration (refer to \ref internalProcess).
     * \param type : type of threaded work. Refer to \ref OneShot and \ref LongTerm.
	 *
     * \note \ref type param is \ref LongTerm by default.
     * \note Internal managed thread is not started since \ref start has been used.
     */
	explicit MA5G_ThreadAbstract(const std::string name, const MA5G_ThreadType type = TYPE_LONGTERM);
    /*!
     * \brief Destructor
	 * Automatically stops and cleans thread context.
     */
    virtual ~MA5G_ThreadAbstract();
	
	// public APIs
    /*!
     * \brief Creates and starts internal thread.
     */
	int start(void);
    /*!
     * \brief Stops internal thread.
	 * \note Internal managed thread is not destroyed, but only stopped.
     */
	int stop(void);
    /*!
     * \brief Change thread scheduling priority.
	 * \param prio : priority setting
	 * \note Only settings is stored, effective when new loop iteration (refer to \ref internalProcess).
     */
	int setSchedPriority(const MA5G_ThreadPriority prio);
    /*!
     * \brief Change thread name.
	 * \param name : thread name
	 * \note Only settings is stored, effective when new loop iteration (refer to \ref internalProcess).
     */
	int setName(const std::string name);
	
	// constants and definitions
	// ...
	
private:
    // private members
    boost::mutex _mut;				/*!< Boost mutex to protect internal members from concurrent accesses. */
	boost::thread* _th;				/*!< Boost thread instance. */
	std::string _name;				/*!< Thread name. */
	MA5G_ThreadPriority _prio;      /*!< Thread priority. */
	MA5G_ThreadType _type;			/*!< Thread type, refer to \ref OneShot and \ref LongTerm. */
	int _stop;						/*!< Stop waiting for next iteration. */
	char _update;					/*!< Priority and/or name settings to apply on next iteration. */
	
	std::map<int, int> prio_values;

    // private APIs
    /*!
     * \brief Internal implementation of loop processing. 
	 * It manages looping, exiting when stop is expected, specific loop iteration error code (refer to \ref loopProcess).
     */
    int internalProcess(void);
    /*!
     * \brief Internal implementation of loop processing. 
	 * \return 0 on success.
	 * \return -1 if last iteration is reached.
     */
    virtual int loopProcess(void) = 0;
    
};

} // namespace Thread;
} // namespace MA5G;
} // namespace Morpho;

#endif // _MA5G_THREAD_ABSTRACT_H_
