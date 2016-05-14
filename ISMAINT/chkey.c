#include "../include/iisam.h"
#include "../include/ismaint.h"

chkey() {
	struct	keys	*tmp;
	int	count = 1;

	printf("\n\n(A) - Display a key description.\n");
	printf("(B) - Display all key descriptions.\n");
	printf("\nHit <RETURN> to exit menu or enter choice: ");
	gets(isarg);
	switch(tolower(*isarg)) {
		case 'a' :
			if (!getidx())
				shokey(actkey);
			return(1);
		case 'b' :
			tmp = head;
			while (tmp) {
				printf("\n\nKey #%d\n", count++);
				shokey(tmp);
				tmp = tmp->link;
			}
			return(1);
		case '\0' :
			return(0);
		default :
			printf("\n%cInvalid response, try again\n", BELL);
			return(1);
	}
}
