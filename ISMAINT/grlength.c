#include "../include/iisam.h"
#include "../include/ismaint.h"

grlength() {
	do {
		printf("\nLength of damaged isam file data records: ");
		recordlength = (int)atoi(gets(isarg));
		if (recordlength <= 0)
			fprintf(stderr, "\n%cInvalid record length\n", BELL);
	} while (recordlength <= 0);
	allocrec();
}
