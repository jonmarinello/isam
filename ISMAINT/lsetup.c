#include "../include/iisam.h"
#include "../include/ismaint.h"

loadsetup(nflag)
int	nflag;
{
	int	i;
	short	kcnt;

	if ((sfile = fopen(sfname, "r")) == NULL) {
		if (nflag)
			printf("Creating new setup file \"%s\"\n", sfname);
		else
			fprintf(stderr, "%cCan't open setup file\n", BELL);
		return(0);
	}
	fread((char *)&oln, sizeof(short), 1, sfile);
	fread((char *)ofilename, sizeof(char), oln, sfile);
	if (cmfname) {
		strcpy(ofilename, cmfname);
		cmfname = (char *)NULL;
	}
	strcpy(ofile, ofilename);
	fread((char *)&nln, sizeof(short), 1, sfile);
	fread((char *)nfilename, sizeof(char), nln, sfile);
	fread((char *)&recordlength, sizeof(short), 1, sfile);
	allocrec();
	fread((char *)&kcnt, sizeof(short), 1, sfile);
	tail = (struct keys *)NULL;
	head = actkey = allockey();
	ldkey(head);
	resetkey();
	for (i = 1; i < kcnt; ++i) {
		actkey = allockey();
		ldkey(actkey);
		resetkey();
	}
	fclose(sfile);
	return(1);
}
