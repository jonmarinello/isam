#include "iisam.h"

/*      Maintain audit trail information. Currently not supported but   */
/*      included for compatibility reasons.                             */

int	isaudit(int isfd, char *filename, int mode)
{
	iserrno = 0;
	/* Following increments needed to get around compiler warnings */
	++isfd;
	++mode;
	++filename;
	return(0);
}
