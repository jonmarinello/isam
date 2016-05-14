#include "iisam.h"

/*      Check filename (including pathname if present)          */

int	flenchk(char *filename)
{
        char    *slash; /* Position of slash if present         */

        if (!(slash = strrchr(filename, '\\')))
                slash = filename;
        else
                ++slash;
        if (strlen(slash) > (MAXSYSNAME - 4)) {
                iserrno = EFNAME;
                return(-1);
        }
        return(0);
}
