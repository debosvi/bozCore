
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include <skalibs/iopause.h>
#include "bozCore/boz_connect.h"

#define COUNT   10
#define BUF_LG  32768
#define RSIZE   128
#define GETSIZE (RSIZE/2)

#define SET_BUF   j=0; while(j++<lg) buf[j-1]=rand();
static char getbuf[GETSIZE];

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
        boz_connect_params_t p;
        int fds[2];

        if(!(i%2))
            pipe(fds);

        p.type = (i%2 ? BOZ_CONNECT_TYPE_READ_ONLY : BOZ_CONNECT_TYPE_WRITE_ONLY);
        p.rsize = i*RSIZE;
        p.fd = fds[(i%2 ? 0 : 1)];
        m=boz_connect_new(&p);
        ids[i] = m;
        fprintf(stderr, "%s: new id (%u)\n", __PRETTY_FUNCTION__, m);

        if(m!=BOZ_CONNECT_INVALID) {

            fprintf(stderr, "%s: id (%u), fd(%d)\n", __PRETTY_FUNCTION__, m, boz_connect_fd(m));
            if(i%2) {
                int getlg=0;
                fprintf(stderr, "%s: id (%u), events r1(%d)\n", __PRETTY_FUNCTION__, m, boz_connect_events(m));
                while(1) {
                    iopause_fd x;
                    int rfd;
                    tain_t deadline;
                    x.fd = boz_connect_fd(m);
                    x.events = boz_connect_events(m);
                    tain_now_g();
                    tain_addsec_g(&deadline, 1) ;
                    rfd = iopause_g(&x, 1, &deadline);
                    if(rfd<=0)
                        break;

                    getlg=boz_connect_get(m, getbuf, GETSIZE);
                    fprintf(stderr, "%s: id (%u), get lg(%d)\n", __PRETTY_FUNCTION__, m, getlg);
                }
            }
            else {
                unsigned int lg=rand()%BUF_LG;
                fprintf(stderr, "%s: id (%u), put lg(%d)\n", __PRETTY_FUNCTION__, m, lg);
                SET_BUF
                fprintf(stderr, "%s: id (%u), events w1(%d)\n", __PRETTY_FUNCTION__, m, boz_connect_events(m));
                boz_connect_put(m, buf, lg/2);
                fprintf(stderr, "%s: id (%u), events w2(%d)\n", __PRETTY_FUNCTION__, m, boz_connect_events(m));
                boz_connect_put(m, buf, lg/2);
                fprintf(stderr, "%s: id (%u), events w3(%d)\n", __PRETTY_FUNCTION__, m, boz_connect_events(m));
                boz_connect_flush(m);
                boz_connect_yield(m);
                fprintf(stderr, "%s: id (%u), events w4(%d)\n", __PRETTY_FUNCTION__, m, boz_connect_events(m));
            }
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
