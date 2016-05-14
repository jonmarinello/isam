#include "iisam.h"

/*      Malloc a string of characters   */

char    *salloc(int nchars)
{
        char    *cptr;  /* Pointer to alloc'd memory area               */

        if ((cptr = malloc((unsigned)(CHARSIZE * nchars))) == NULL) {
                fprintf(stderr, memerror);
                abort();
        }
        return(cptr);
}
