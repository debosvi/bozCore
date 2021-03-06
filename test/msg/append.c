
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "bozCore/boz_msg.h"

#define COUNT    10
#define BUF_LG   65536

#define SET_BUF   j=0; while(j++<BUF_LG) buf[j-1]=rand();

int main(int ac, char **av) {
    boz_msg_t m=BOZ_MSG_INVALID;
    unsigned int i=0, j=0;
    unsigned int ids[COUNT];
    unsigned int count=COUNT;

    if(ac>1)
        count=atoi(av[1]);

    fprintf(stderr, "Iterates with %u loops\n", count);

    memset(ids, -1, COUNT*sizeof(unsigned int));

    i=0;
    while(i<count) {
        char buf[BUF_LG];
        boz_msg_params_t p;
        p.size = 10*i;
        p.type = BOZ_MSG_TYPE_RAW;
        m=boz_msg_new(&p);
        ids[i] = m;
        fprintf(stderr, "%s: new id (%u)\n", __PRETTY_FUNCTION__, m);

        SET_BUF
        boz_msg_append(m, buf, BUF_LG);
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
