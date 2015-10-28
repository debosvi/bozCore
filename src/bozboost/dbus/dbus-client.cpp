/*!
 * \file main.cpp
 * \brief Main program entry.
 * \author Vincent de RIBOU
 * \version 0.1
 */
 
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "MyDBusClassClient.hpp"

using namespace std;
using namespace Morpho::MA5G;

static const char *ECHO_SERVER_NAME = "Morpho.MA5G.MyDBusClass";
static const char *ECHO_SERVER_PATH = "/Morpho/MA5G/MyDBusClass";

static const size_t THREADS = 3;

static bool spin = true;
static void niam(int sig);
static void *greeter_thread(void *arg);
static void handler1(const void *data, void *buffer, unsigned int nbyte);
static void handler2(const void *data, void *buffer, unsigned int nbyte);
static void handler3(const void *data, void *buffer, unsigned int nbyte);

MyDBusClassClient *g_client = NULL;

DBus::Pipe *thread_pipe_list[THREADS];

DBus::BusDispatcher dispatcher;
DBus::DefaultTimeout *timeout;

void *greeter_thread(void *arg)
{
    char idstr[16];
    size_t i = (size_t) arg;

    snprintf(idstr, sizeof(idstr), "%lu", pthread_self());

    thread_pipe_list[i]->write(idstr, strlen(idstr) + 1);

    cout << idstr << " done (" << i << ")" << endl;

    return NULL;
}

void niam(int sig)
{
    (void)sig;
    spin = false;

    dispatcher.leave();
}

void handler1(const void *data, void *buffer, unsigned int nbyte)
{
    char *str = (char *) buffer;
    cout << "buffer1: " << str << ", size: " << nbyte << endl;
    for (int i = 0; i < 30 && spin; ++i)
    {
        cout << "call1: " << g_client->Random() << endl;
    }
}

void handler2(const void *data, void *buffer, unsigned int nbyte)
{
    char *str = (char *) buffer;
    cout << "buffer2: " << str << ", size: " << nbyte << endl;
    for (int i = 0; i < 30 && spin; ++i)
    {
        cout << "call2: " << g_client->Random() << endl;
    }
}

void handler3(const void *data, void *buffer, unsigned int nbyte)
{
    char *str = (char *) buffer;
    cout << "buffer3: " << str << ", size: " << nbyte << endl;
    for (int i = 0; i < 30 && spin; ++i)
    {
        cout << "call3: " << g_client->Random() << endl;
    }
}

int main()
{
    size_t i;

    signal(SIGTERM, niam);
    signal(SIGINT, niam);

    DBus::_init_threading();

    DBus::default_dispatcher = &dispatcher;

    // increase DBus-C++ frequency
    new DBus::DefaultTimeout(100, false, &dispatcher);

    DBus::Connection conn = DBus::Connection::SessionBus();

    MyDBusClassClient client(conn, ECHO_SERVER_PATH, ECHO_SERVER_NAME);
    g_client = &client;

    pthread_t threads[THREADS];

    thread_pipe_list[0] = dispatcher.add_pipe(handler1, NULL);
    thread_pipe_list[1] = dispatcher.add_pipe(handler2, NULL);
    thread_pipe_list[2] = dispatcher.add_pipe(handler3, NULL);
    for (i = 0; i < THREADS; ++i)
    {
        pthread_create(threads + i, NULL, greeter_thread, (void *) i);
    }

    dispatcher.enter();

    cout << "terminating" << endl;

    for (i = 0; i < THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    dispatcher.del_pipe(thread_pipe_list[0]);
    dispatcher.del_pipe(thread_pipe_list[1]);
    dispatcher.del_pipe(thread_pipe_list[2]);

    return 0;
}
