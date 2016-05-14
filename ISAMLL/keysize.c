#include "iisam.h"

/*      Compute the total variable length of a given key description
        record.
*/

int	keysize(struct keydesc *keydesc)
{
        return(keydesc->k_nparts * KPSIZE + 10);
}
