#include "iisam.h"

/*      Compute the size of a given key in bytes.                       */

int	complen(struct keydesc *keydesc)
{
        return((keydesc->k_len + (NDKSIZE - sizeof(char *))));
}
