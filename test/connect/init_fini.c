
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "bozCore/boz_connect.h"

#define COUNT   10
#define RSIZE   128

int main(int ac, char **av) {
    boz_connect_t m=BOZ_CONNECT_INVALID;
    unsigned int i=0;
    unsigned int ids[COUNT];
    unsigned int count=COUNT;
    int fds[COUNT];    

    if(ac>1)
        count=atoi(av[1]);

    fprintf(stderr, "Iterates with %u loops\n", count);

    memset(ids, -1, COUNT*sizeof(unsigned int));

    i=0;
    while(i<(COUNT/2)) {
        pipe(&fds[2*i]);
        fprintf(stderr, "%s: new fds (%d/%d)\n", __PRETTY_FUNCTION__, fds[2*i+0], fds[2*i+1]);
        i++;
    }

    i=0;
    while(i<count) {
        boz_connect_params_t p;
        p.type = BOZ_CONNECT_TYPE_BASIC;
        p.rsize = RSIZE;
        p.fd = fds[i];
        m=boz_connect_new(&p);
        ids[i] = m;
        fprintf(stderr, "%s: new id (%u)\n", __PRETTY_FUNCTION__, m);
        i++;
    }

    i=0;
    while(i<(COUNT/2)) {
        m = ids[2*i];
        fprintf(stderr, "%s: release id (%u)\n", __PRETTY_FUNCTION__, m);
        if(boz_connect_release(m) < 0)
            fprintf(stderr, "%s: release id (%u), failed (%d/%s)\n", __PRETTY_FUNCTION__, m, errno, strerror(errno));
        i++;
    }

    exit(EXIT_SUCCESS);
}
