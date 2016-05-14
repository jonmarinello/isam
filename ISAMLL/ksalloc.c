#include "iisam.h"

/*      Dynamically allocate a struct nodekey */

struct  nodekey *ksalloc()
{
        struct  nodekey *nkstruct;      /* Pointer to alloc'd memory    */

        if ((nkstruct = ((struct nodekey *) malloc(NDKSIZE))) == NULL) {
                fprintf(stderr, memerror);
                abort();
        }
        return(nkstruct);
}
