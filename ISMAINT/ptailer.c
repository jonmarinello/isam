#include "../include/iisam.h"
#include "../include/ismaint.h"

ptailer() {
	if (dupspress)
		printf("\nDuplicate records Suppressed: %ld\n", dupspress);
	printf("\nNumber of live records: %ld\n", livecnt);
	printf("\nNumber of dead records: %ld\n", deadcnt);
	printf("\nRebuild completion successful\n\n");
}
