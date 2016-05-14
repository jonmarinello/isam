#include "iisam.h"

/*      Compare two isam key descriptions for pre-existence.    */

int	kdsccmp(char *newkey, char *curkey)
{
        int     i;

        for (i = 0; i < KDSIZE - LONGSIZE; ++i)
                if ((unsigned)newkey[i] != (unsigned)curkey[i])
                        return(1);
        return(0);
}
