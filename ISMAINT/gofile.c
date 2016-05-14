#include "../include/iisam.h"
#include "../include/ismaint.h"

gofile() {
	if (cmfname) {
		strcpy(ofilename, cmfname);
		cmfname = (char *)NULL;
		return;
	}
	do {
		printf("\nName of damaged isam file: ");
		gets(ofilename);
	} while (!strlen(ofilename));
	strcpy(ofile, ofilename);
}
