#include "../include/iisam.h"
#include "../include/ismaint.h"

gcomp() {
	char	c;

	do {
		printf("\nKey compression (y/n) ? : ");
		gets(isarg);
		if ((c = tolower(*isarg)) == 'y') {
			actkey->compression = COMPRESS;
			break;
		}
		else if (c == 'n') {
			actkey->compression = 0;
			break;
		}
	} while (c != 'y' && c != 'n');
}
