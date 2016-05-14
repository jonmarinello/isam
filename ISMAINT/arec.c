#include "../include/iisam.h"
#include "../include/ismaint.h"

allocrec() {
	if (record) {
		free((char *)record);
		record = (char *)NULL;
	}
	if ((record = (char *)malloc((sizeof(char)) *
		(recordlength + 1))) == NULL) {
			fprintf(stderr, memerror);
			exit(E_ER);	
	}
}
