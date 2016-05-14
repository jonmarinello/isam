#include "iisam.h"

/*      Rename an isam file.    */

int	isrename(char *oldname, char *newname)
{
        char    *oname;
        char    *nname;

        errno = iserrno = 0;
        if (flenchk(newname))
                return(-1);
        oname = salloc(strlen(oldname)+5);
        nname = salloc(strlen(newname)+5);
        sprintf(oname, "%s.idx", oldname);
        sprintf(nname, "%s.idx", newname);
        if (!rename(oname, nname)) {
                sprintf(oname, "%s.dat", oldname);
                sprintf(nname, "%s.dat", newname);
                rename(oname, nname);
        }
        iserrno = errno;
        free(oname);
        free(nname);
        return(iserrno ? -1 : 0);
}
