#include "iisam.h"

/*      Push a return address on the father stack.              */

void	push(long address)
{
        if (!stackmax) {        /* Initial allocation */
                fstack = (long *)salloc(LONGSIZE);
                ++stackmax;
        }
        else if (stackptr + 1 == stackmax) {    /* Additional allocations */
                if ((fstack = (long *)realloc((char *)fstack,
                        (unsigned)(LONGSIZE * ++stackmax))) == NULL) {
                        fprintf(stderr, memerror);
                        abort();
                }
        }       /* Space may already be available       */
        fstack[++stackptr] = address;
}
