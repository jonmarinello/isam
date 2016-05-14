#include "../include/iisam.h"
#include "../include/ismaint.h"

rmnewfiles() {
	sprintf(ufile, "%s%s", nfilename, ".dat");
	unlink(ufile);
	sprintf(ufile, "%s%s", nfilename, ".idx");
	unlink(ufile);
}
