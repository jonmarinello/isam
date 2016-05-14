#include "../include/iisam.h"
#include "../include/ismaint.h"

savesetup() {
	short	kcnt = 1;
	int	i;
	char	c;

	if ((sfile = fopen(sfname, "r")) != NULL) {
		printf("\n%cSetup file already exists, overwrite (y/n) ? ",
			BELL);
		for (;;) {
			gets(isarg);
			if ((c = tolower(*isarg)) == 'y')
				break;
			else if (c == 'n') {
				fclose(sfile);
				return;
			}
			else
				printf("\n%cOverwrite (y/n) ? ", BELL);
		}
		fclose(sfile);
	}
	sfile = fopen(sfname, "w");
	oln = strlen(ofile);
	nln = strlen(nfilename);
	fwrite((char *)&oln, sizeof(short), 1, sfile);
	fwrite((char *)ofile, sizeof(char), oln, sfile);
	fwrite((char *)&nln, sizeof(short), 1, sfile);
	fwrite((char *)nfilename, sizeof(char), nln, sfile);
	fwrite((char *)&recordlength, sizeof(short), 1, sfile);
	actkey = head;
	while (actkey->link != (struct keys *)NULL) {
		++kcnt;
		actkey = actkey->link;
	}
	fwrite((char *)&kcnt, sizeof(short), 1, sfile);
	actkey = head;
	for (i = 0; i < kcnt; ++i) {
		dmpkey(actkey);
		actkey = actkey->link;
	}
	fclose(sfile);
}
