#include "iisam.h"

/*      Load variable length key descriptions to index file     */

/*      Please note - The for loop is neccessary because some   */
/*      machines do allignment in the 'k_part' structure, for   */
/*      example the new AT&T. The HP-9000 does not. So to       */
/*      ensure portability of the user's isam files (Assuming   */
/*      he didn't use int's) the loop is neccessary to read     */
/*      in all shorts one by one. If you change it, the isam    */
/*      WON'T BE PORTABLE. This may even happen between 32 bit  */
/*      machines as in the example above. See dkey().           */

void	lkey(int isfd, struct keydesc *keydesc)
{
        int     i;

        read(ISIFD, (char *)&keydesc->k_flags, SHORTSIZE);
        read(ISIFD, (char *)&keydesc->k_nparts, SHORTSIZE);
        for (i = 0; i < keydesc->k_nparts; ++i) {
                read(ISIFD, (char *)&keydesc->k_part[i].kp_start, SHORTSIZE);
                read(ISIFD, (char *)&keydesc->k_part[i].kp_leng, SHORTSIZE);
                read(ISIFD, (char *)&keydesc->k_part[i].kp_type, SHORTSIZE);
        }
        read(ISIFD, (char *)&keydesc->k_len, SHORTSIZE);
        read(ISIFD, (char *)&keydesc->k_rootnode, LONGSIZE);
}
