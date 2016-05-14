#include "../include/iisam.h"
#include "../include/ismaint.h"

renfiles() {
	iserase(gfname(ofile));
	isrename(nfilename, gfname(ofile));
}
