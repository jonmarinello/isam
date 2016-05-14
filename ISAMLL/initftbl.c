#include "iisam.h"

void	initftbl(struct filetable *file, int fdidx, int fddat, int mode)
{
        file->idxfd = fdidx;
        file->datfd = fddat;
        file->offset = NILL;
        file->omode = mode;
        file->livekey = PRIMARYKEY;
        file->startsw = file->lastsw = 0;
        file->olockoffset = -1L;
        file->descptr = (struct keydesc *)NULL;
        file->kbptr = (struct keyblock *)NULL;
}
