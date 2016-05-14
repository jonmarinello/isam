#include "iisam.h"

/*      Dynamically reallocate more struct nodekey pointers to array    */

/*      Note - All calls currently allocate only one more pointer       */

struct  nodekey **nkrealloc(struct nodekey **aptr, int nnk)
{
        struct  nodekey **nkptr;        /* Pointer to realloc'd memory  */

        if ((nkptr = (struct nodekey **)realloc((char *)aptr,
                (unsigned)(sizeof(struct nodekey *) * nnk))) == NULL) {
                        fprintf(stderr, memerror);
                        abort();
        }
        return(nkptr);
}
