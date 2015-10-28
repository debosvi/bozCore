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

DBus::BusDispatcher dispatcher;

void niam(int sig) {
    (void)sig;
    dispatcher.leave();
}

int main(int ac, char **av) {
	(void)ac;
	(void)av;
    signal(SIGTERM, niam);
    signal(SIGINT, niam);

    DBus::default_dispatcher = &dispatcher;

    DBus::Connection conn = DBus::Connection::SessionBus();
    conn.request_name(MyDBusClassServer::SERVER_NAME);

    MyDBusClassServer server(conn);

    dispatcher.enter();

    return 0;

}
