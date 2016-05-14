#include "iisam.h"

/*      Pad a keyval with spaces */

void     padkey(char *key, int start, short max)
{
        int     i;

        for (i = start; i < max; ++i)
                key[i] = ' ';
}
