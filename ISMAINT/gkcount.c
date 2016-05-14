#include "../include/iisam.h"
#include "../include/ismaint.h"

gkcount() {
	do {
		printf("\nNumber of fields in key description: ");
		actkey->kd.k_nparts = atoi(gets(isarg));
		if (actkey->kd.k_nparts <= 0 || actkey->kd.k_nparts > NPARTS)
			printf("\n%cInvalid field count\n", BELL);
	} while (actkey->kd.k_nparts <= 0 || actkey->kd.k_nparts > NPARTS);
}
