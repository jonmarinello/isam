#include "iisam.h"

/*      Reset node length subtracting out compression is any    */

void	rstnl(struct keydesc *keydesc, struct NODE *worknode)
{
        worknode->ndesc.nodeln = NDDSIZE +
                (worknode->ndesc.keycnt * complen(keydesc));
        worknode->ndesc.nodeln -= ccpress(keydesc, worknode);
}
