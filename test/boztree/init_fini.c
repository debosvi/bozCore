
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "skalibs/tai.h"
#include "skalibs/djbunix.h"
#include "bozCore/boztree.h"

#define MAX_INSERT   16
#define MAXBUF_SIZE 256

typedef struct {
    uint64      i;
    char    x[MAXBUF_SIZE];
} mytree_t;

int main(int ac, char **av) {
    boztree_t t=BOZTREE_ZERO;
    mytree_t e;
    unsigned int found=0;
    unsigned int i;
//    int count=0;
//
//    if(ac>1)
//        count=atoi(av[1]);
//
//    fprintf(stderr, "Iterates with %u loops\n", count);

    BOZTREE_INIT(&t, mytree_t);

    for(i=0; i<MAX_INSERT; i++) {
        e.i = 0xFFAA5500 + i;
        fprintf(stderr, "\ninsert id(%016llx)\n", (long long int)e.i);
        memset(&e.x[0], 0, MAXBUF_SIZE);
        fprintf(stderr, "\tbefore insertion: search id(%016llx)\n", e.i);
        if(avltree_search(&t.a, &e.i, &found))
            fprintf(stderr, "before insertion: found (SHOULD NOT BE PRINTED)\n");
        boztree_insert(&t, (boztree_id_t*)&e);
        fprintf(stderr, "\tafter insertion: search id(%016llx)\n", e.i);
        if(avltree_search(&t.a, &e.i, &found))
            fprintf(stderr, "\tafter insertion: found\n");
        fprintf(stderr, "\tinsert: tree total size: %lu\n", avltree_len(&t.a));
    }

    for(i=0; i<MAX_INSERT; i++) {
        e.i = 0xFFAA5500 + i;
        fprintf(stderr, "\n\tdelete id(%016llx)\n", e.i);
        boztree_delete(&t, e.i);
        fprintf(stderr, "\tafter delete: search id(%016llx)\n", e.i);
        if(avltree_search(&t.a, &e.i, &found))
            fprintf(stderr, "\tafter delettion: found (SHOULD NOT BE PRINTED)\n");
        fprintf(stderr, "\tdelete: tree total size: %lu\n", avltree_len(&t.a));
    }
    
    exit(EXIT_SUCCESS);
}

