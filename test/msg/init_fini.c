
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "boz_msg.h"

#define COUNT   10

int main(int ac, char **av) {
    boz_msg_t m=BOZ_MSG_INVALID;
    int i=0;
    unsigned int ids[COUNT];

    memset(ids, -1, COUNT*sizeof(unsigned int));

    i=0;
    while(i<COUNT) {
        m=boz_msg_new(10*i, BOZ_MSG_TYPE_RAW);
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

//    if(boz_msg_release(m)<0)

    exit(EXIT_SUCCESS);
}
