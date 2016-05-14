#include "iisam.h"

/*      Remove the corresponding '.idx' and '.dat' files from the disk. */

int	iserase(char *filename)
{
        char    *fname;         /* Filename work buffer         */

        errno = iserrno = 0;
        if (flenchk(filename))
                return(-1);
        fname = salloc(strlen(filename)+5);
        sprintf(fname, "%s.idx", filename);
        if (!unlink(fname)) {
                sprintf(fname, "%s.dat", filename);
                unlink(fname);
        }
        iserrno = errno;
        free(fname);
        if (iserrno)
                return(-1);
        else
                return(0);
}
