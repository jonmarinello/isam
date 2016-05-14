#include "../include/iisam.h"
#include "../include/ismaint.h"

buildindex() {
	sprintf(ofilename, "%s%s", gfname(ofile), ".dat");
	if ((ofp = fopen(ofilename, "r")) == NULL) {
		fprintf(stderr, "\n%ccan't open %s\n", BELL, ofilename);
		++iserrno;
		rmnewfiles();
		return;
	}
	while (getdrec() != EOF) {
		if (iserrno) {
			fclose(ofp);
			isclose(nfp);
			++iserrno;
			return;
		}
		iswrite(nfp, record);
		if (iserrno == EDUPL) {
			++dupspress;
			iserrno = 0;
		}
		else if (iserrno) {
			printerror();
			fclose(ofp);
			isclose(nfp);
			rmnewfiles();
			++iserrno;
			return;
		}
	}
}
