
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "boz_msg.h"

#define COUNT   10

int main(int ac, char **av) {
    boz_msg_t m=BOZ_MSG_INVALID;
    unsigned int i=0;
    unsigned int ids[COUNT];
    unsigned int count=COUNT;

    if(ac>1)
        count=atoi(av[1]);

    fprintf(stderr, "Iterates with %u loops\n", count);

    memset(ids, -1, COUNT*sizeof(unsigned int));

    i=0;
    while(i<count) {
        boz_msg_params_t p;
        p.size = 10*i;
        p.type = BOZ_MSG_TYPE_RAW;
        m=boz_msg_new(&p);
        ids[i] = m;
        fprintf(stderr, "%s: new id (%u)\n", __PRETTY_FUNCTION__, m);
        i++;
    }

    i=0;
    while(i<(COUNT/2)) {
        m = ids[2*i];
        fprintf(stderr, "%s: release id (%u)\n", __PRETTY_FUNCTION__, m);
        if(boz_msg_release(m) < 0)
            fprintf(stderr, "%s: release id (%u), failed (%d/%s)\n", __PRETTY_FUNCTION__, m, errno, strerror(errno));
        i++;
    }

    exit(EXIT_SUCCESS);
}
