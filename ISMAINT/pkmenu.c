#include "../include/iisam.h"
#include "../include/ismaint.h"

pkmenu() {
	int	i;

	shokey(actkey);
	printf("\n\n(A) - Duplicate keys allowed.\n");
	printf("(B) - Key compression.\n");
	printf("(C) - Number of key parts.\n");
	printf("(D) - To access a key field.\n");
	printf("\n\nHit <RETURN> to exit menu or enter choice: ");
	gets(isarg);
	switch(tolower(*isarg)) {
		case 'a' :
			gdups();
			return(1);
		case 'b' :
			gcomp();
			return(1);
		case 'c' :
			gkcount();
			gakfields();
			return(1);
		case 'd' :
			printf("\nIndex of key field: ");
			gets(isarg);
			i = atoi(isarg) - 1;
			if (i < 0 || i >= actkey->kd.k_nparts) {
				printf("\n%cInvalid index\n", BELL);
				return(1);
			}
			getfinfo(i);
			return(1);
		case '\0' :
			return(0);
		default :
			printf("\n%cInvalid response, try again\n", BELL);
			return(1);
	}
}
