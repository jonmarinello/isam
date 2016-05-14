#include "../include/iisam.h"
#include "../include/ismaint.h"

getdrec() {
	do {
		if (!(fread((char *)record, sizeof(char),
			recordlength + 1, ofp)))
				return(EOF);
		switch (record[recordlength]) {
		case ALIVE :
			++livecnt;
			break;
		case DEAD :
			++deadcnt;
			break;
		default :
			fprintf(stderr, "\n%cRecord length doesn't match data\n", BELL);
			++iserrno;
			rmnewfiles();
			return(0);
		}
	} while (record[recordlength] == DEAD);
	return(0);
}
