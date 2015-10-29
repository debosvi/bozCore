/*!
 * \file main.cpp
 * \brief Main program entry.
 * \author Vincent de RIBOU
 * \version 0.1
 */
 
#include <unistd.h>
#include <signal.h>

#include "MyDBusClassServer.hpp"

using namespace Morpho::MA5G;

static void niam(int sig);
static void* greeter_thread(void *arg);
static MyDBusClassServer *ps=NULL;

DBus::BusDispatcher dispatcher;

void niam(int sig) {
    (void)sig;
    dispatcher.leave();

}

void* greeter_thread(void *arg) {

    sleep(20);
    ps->valueChanged("customSource");
    
    return NULL;
}

int main(int ac, char **av) {
    pthread_t th;
	(void)ac;
	(void)av;
    signal(SIGTERM, niam);
    signal(SIGINT, niam);

    DBus::default_dispatcher = &dispatcher;

    DBus::Connection conn = DBus::Connection::SessionBus();
    conn.request_name(MyDBusClassServer::SERVER_NAME);

    MyDBusClassServer server(conn);
    ps = &server;

    pthread_create(&th, NULL, greeter_thread, NULL);

    dispatcher.enter();
 
    pthread_join(th, NULL);

    return 0;

}
