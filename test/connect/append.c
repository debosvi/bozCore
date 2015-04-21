
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include "bozCore/boz_connect.h"

#define COUNT   10
#define BUF_LG  236
#define RSIZE   128

#define SET_BUF   j=0; while(j++<lg) buf[j-1]=rand();

int main(int ac, char **av) {
    boz_connect_t m=BOZ_CONNECT_INVALID;
    unsigned int i=0, j=0;
    unsigned int ids[COUNT];
    unsigned int count=COUNT;

    if(ac>1)
        count=atoi(av[1]);

    srand(time(NULL));

    fprintf(stderr, "Iterates with %u loops\n", count);

    memset(ids, -1, COUNT*sizeof(unsigned int));

    i=0;
    while(i<count) {
        char buf[BUF_LG];
        unsigned int lg=rand()%BUF_LG;
        boz_connect_params_t p;
        int fds[2];

        pipe(fds);

        p.type = BOZ_CONNECT_TYPE_BASIC;
        p.rsize = i*RSIZE;
        p.fd = fds[1];
        m=boz_connect_new(&p);
        ids[i] = m;
        fprintf(stderr, "%s: new id (%u), put lg(%d)\n", __PRETTY_FUNCTION__, m, lg);

        if(m!=BOZ_CONNECT_INVALID) {
            fprintf(stderr, "%s: new id (%u), fd(%d)\n", __PRETTY_FUNCTION__, m, boz_connect_fd(m));
            SET_BUF
                fprintf(stderr, "%s: new id (%u), events 1(%d)\n", __PRETTY_FUNCTION__, m, boz_connect_events(m));
            boz_connect_put(m, buf, lg/2);
            fprintf(stderr, "%s: new id (%u), events 2(%d)\n", __PRETTY_FUNCTION__, m, boz_connect_events(m));
            boz_connect_put(m, buf, lg/2);
            fprintf(stderr, "%s: new id (%u), events 3(%d)\n", __PRETTY_FUNCTION__, m, boz_connect_events(m));
            boz_connect_flush(m);
            boz_connect_yield(m);
        }
        i++;
    }

    i=count/2;
    while(i) {
        m = ids[2*i-1];
        fprintf(stderr, "%s: release id (%u)\n", __PRETTY_FUNCTION__, m);
        if(boz_connect_release(m) < 0)
            fprintf(stderr, "%s: release id (%u), failed (%d/%s)\n", __PRETTY_FUNCTION__, m, errno, strerror(errno));
        else
            fprintf(stderr, "%s: id (%u), fd(%d)\n", __PRETTY_FUNCTION__, m, boz_connect_fd(m));
        i--;
    }

    exit(EXIT_SUCCESS);
}
