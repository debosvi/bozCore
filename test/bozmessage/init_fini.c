
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "bozCore/bozmessage.h"

int g_pipe[2];

int main(int ac, char **av) {
    bozmessage_t m=BOZMESSAGE_ZERO;
    bozmessage_sender_t s=BOZMESSAGE_SENDER_ZERO;
    bozmessage_receiver_t *r=bozmessage_receiver_0;

//    int count=0;
//
//    if(ac>1)
//        count=atoi(av[1]);
//
//    fprintf(stderr, "Iterates with %u loops\n", count);

    pipe(g_pipe);
    bozmessage_receiver_fd(r) = g_pipe[0];
    bozmessage_sender_fd(&s) = g_pipe[1];

    m.s = "toto is dead!!";
    m.len = 14;
    bozmessage_put(&s, &m);   

    m.s = "toto is alive now";
    m.len = 17;
    bozmessage_put(&s, &m);   

    bozmessage_unput(&s);   

    m.s = "toto is alive now";
    m.len = 17;
    bozmessage_put(&s, &m);   

    bozmessage_sender_flush(&s);   
    bozmessage_sender_flush(&s);   

    m.s = 0;
    m.len = 0;
    bozmessage_receive(r, &m);   

    bozmessage_sender_free(&s);

    exit(EXIT_SUCCESS);
}

