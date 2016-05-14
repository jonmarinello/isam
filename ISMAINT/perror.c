#include "../include/iisam.h"
#include "../include/ismaint.h"

printerror() {
	if (!iserrno)
		return;
	if (iserrno >= 100 && iserrno < is_nerr)
		printf("\n%cISAM error %d: %s\n\n", BELL, iserrno,
			is_errlist[iserrno-100]);
	else
		printf("\n%cISAM error %d: See system documentation\n\n", BELL,
			iserrno);
}
