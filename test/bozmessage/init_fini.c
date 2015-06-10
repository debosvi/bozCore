
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "bozCore/bozmessage.h"


int main(int ac, char **av) {
    bozmessage_t m=BOZMESSAGE_ZERO;
    bozmessage_sender_t s=BOZMESSAGE_SENDER_ZERO;
//    int count=0;
//
//    if(ac>1)
//        count=atoi(av[1]);
//
//    fprintf(stderr, "Iterates with %u loops\n", count);
    bozmessage_sender_fd(&s) = 1;

    m.s = "toto is dead!!";
    m.len = 14;
    bozmessage_put(&s, &m);   

    m.s = "toto is alive now";
    m.len = 17;
    bozmessage_put(&s, &m);   

    bozmessage_unput(&s);   
//    bozmessage_sender_flush(&s);   


    bozmessage_sender_free(&s);


    exit(EXIT_SUCCESS);
}

