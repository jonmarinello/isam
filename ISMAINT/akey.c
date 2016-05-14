#include "../include/iisam.h"
#include "../include/ismaint.h"

struct	keys	*allockey() {
	struct	keys	*ptr;

	if ((ptr = (struct keys *)malloc(sizeof(struct keys))) == NULL) {
		fprintf(stderr, memerror);
		exit(E_ER);
	}
	if (tail)
		tail->link = ptr;
	ptr->compression = 0;
	ptr->duplicates = 0;
	ptr->link = (struct keys *)NULL;
	tail = ptr;
	return(ptr);
}
