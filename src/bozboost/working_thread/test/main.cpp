/*!
 * \file main.cpp
 * \brief Main program entry.
 * \author Vincent de RIBOU
 * \version 0.1
 */
 
#include <iostream>
#include <poll.h>

#include "thread/working_thread.h"

class myThread: public qm::utils::working_thread {
public:
    explicit myThread() : working_thread("myThread") {
        std::cerr << __PRETTY_FUNCTION__ << std::endl;
    }
    virtual ~myThread() {
        std::cerr << __PRETTY_FUNCTION__ << std::endl;
    }
    
    virtual bool action() {
        int i=0;
        while(i++<16) {
            std::cerr << __PRETTY_FUNCTION__ << ", " << id() << ", " << i << std::endl;
            poll(0,0,100+(rand()%50));
        }
        return false;
    }
    
    virtual void on_start(){
        std::cerr << __PRETTY_FUNCTION__ << std::endl;
    }
    
    virtual void on_exit(){
        std::cerr << __PRETTY_FUNCTION__ << std::endl;
    }
};

int main() {
    qm::utils::working_thread::bulk bk;
    
    for(int i=0; i<5; i++)
        bk+=new myThread();

    std::cerr << "start" << std::endl;
    bk.start();
    sleep(5);
    std::cerr << "stop" << std::endl;
    bk.stop();
    return 0;
}
