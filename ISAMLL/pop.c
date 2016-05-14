#include "iisam.h"

/*      Pop and return an address from the father stack.        */

long    pop()
{
        return(fstack[--stackptr]);
}
