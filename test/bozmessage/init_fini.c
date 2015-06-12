
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "skalibs/tai.h"
#include "skalibs/djbunix.h"
#include "bozCore/bozmessage.h"

static int g_pipe[2];

static int myhandler(bozmessage_t const *m, void *p) {
    (void)p;
    m->s[m->len] = 0;
    fprintf(stderr, "New message: %s\n", m->s);
}

int main(int ac, char **av) {
    bozmessage_t m=BOZMESSAGE_ZERO;
    bozmessage_sender_t s=BOZMESSAGE_SENDER_ZERO;
    bozmessage_receiver_t *r=bozmessage_receiver_0;
    siovec_t sio[3];
    bozmessage_v_t mv=BOZMESSAGE_V_ZERO;

//    int count=0;
//
//    if(ac>1)
//        count=atoi(av[1]);
//
//    fprintf(stderr, "Iterates with %u loops\n", count);

    pipenb(g_pipe);
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

    {
        tain_t deadline;
        tain_now_g();
        tain_addsec_g(&deadline, 10);

        bozmessage_sender_timed_flush_g(&s, &deadline);   
    }
    bozmessage_sender_flush(&s);   

    m.s = 0;
    m.len = 0;
    bozmessage_receive(r, &m);   
    bozmessage_receive(r, &m);   

    sio[0].s = "First message";
    sio[0].len = 13;
    sio[1].s = "Second message";
    sio[1].len = 14;
    sio[2].s = "Third and final message";
    sio[2].len = 23;
    mv.v = sio;
    mv.vlen=3;
    bozmessage_putv(&s, &mv);   
    bozmessage_sender_flush(&s);   
    
    {
        tain_t deadline;
        tain_now_g();
        tain_addsec_g(&deadline, 10);

        bozmessage_timed_handle_g(r, myhandler, 0, &deadline);   
    }

    bozmessage_sender_free(&s);
    bozmessage_receiver_free(r);

    exit(EXIT_SUCCESS);
}

