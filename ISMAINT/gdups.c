#include "../include/iisam.h"
#include "../include/ismaint.h"

gdups() {
	char	c;

	do {
		printf("\nDuplicate keys allowed (y/n) ? : ");
		gets(isarg);
		if ((c = tolower(*isarg)) == 'y') {
			actkey->duplicates = ISDUPS;
			break;
		}
		else if (c == 'n') {
			actkey->duplicates = ISNODUPS;
			break;
		}
	} while (c != 'y' && c != 'n');
}
