#include "../include/iisam.h"
#include "../include/ismaint.h"

delidx() {
	for(;;) {
		if (!head) {
			printf("\n%cNo available key descriptions.\n", BELL);
			return;
		}
		printf("\nEnter number of key description: ");
		if (delkey((int)atoi(gets(isarg))))
			printf("\n%cThat key does not exist.\n", BELL);
		else
			return;
	}
}
