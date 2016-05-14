#include "../include/iisam.h"
#include "../include/ismaint.h"

guid() {
	extern	long	atol();

	++gotuid;
	do {
		printf("\nNew unique id value: ");
		uid = atol(gets(isarg));
		if (uid < 0L)
			fprintf(stderr, "\n%cInvalid unique id\n", BELL);
	} while (uid < 0L);
}
