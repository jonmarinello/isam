#include "iisam.h"

/*      Dynamically allocate some struct nodekey pointers to array      */

struct  nodekey **nkalloc(int nnk)
{
        struct  nodekey **nkptr;        /* Pointer to alloc'd memory    */

#ifdef __BORLANDC__
	if (nnk == 0)
		nnk = 1;
#endif

        if ((nkptr = ((struct nodekey **)
                malloc((unsigned)(nnk * sizeof(struct nodekey *))))) == NULL) {
                        fprintf(stderr, memerror);
                        abort();
        }
        return(nkptr);
}
