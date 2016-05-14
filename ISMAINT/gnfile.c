#include "../include/iisam.h"
#include "../include/ismaint.h"

gnfile() {
	do {
		printf("\nName of target isam file: ");
		gets(nfilename);
	} while (!strlen(nfilename));
}
