#include "../include/iisam.h"
#include "../include/ismaint.h"

getidx() {
	for(;;) {
		if (!head) {
			printf("\n%cNo available key descriptions.\n", BELL);
			return(1);
		}
		printf("\nEnter number of key description: ");
		if (setact((int)atoi(gets(isarg)))) {
			printf("\n%cThat key does not exist.\n", BELL);
			return(1);
		}
		else
			return(0);
	}
}
