#include "../include/iisam.h"
#include "../include/ismaint.h"

pkeymenu() {
	printf("\n\n(A) - Add a new key description.\n");
	printf("(B) - Modify an existing key description.\n");
	printf("(C) - Delete an existing key description.\n");
	printf("(D) - Display one or all key descriptions.\n");
	printf("\nHit <RETURN> to exit menu or enter choice: ");
	gets(isarg);
	switch(tolower(*isarg)) {
		case 'a' :
			actkey = allockey();
			if (!head)
				head = actkey;
			gdups();
			gcomp();
			gkcount();
			gakfields();
			return(1);
		case 'b' :
			if (!getidx())
				while (pkmenu())
					;
			return(1);
		case 'c' :
			delidx();
			return(1);
		case 'd' :
			while (chkey())
				;
			return(1);
		case '\0' :
			return(0);
		default :
			printf("\n%cInvalid response, try again\n", BELL);
			return(1);
	}
}
