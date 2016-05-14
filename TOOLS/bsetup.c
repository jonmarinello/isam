#include "../include/iisam.h"
#include "../include/ismaint.h"

/*
	bsetup - Build "setup" file for future rebuild from existing ".idx"
		file.
*/

char	isamfile[MAXSLEN];
char	setupfile[MAXSLEN];
char	sarg[MAXSLEN];	
short	recordlength;
short	kcnt;
short	nisam;
short	oisam;
struct	dictinfo	dictinfo;
FILE	*sfile;

char	*msg_1 = "%s: can't read key description (%d) on \"%s\" (%03d)\n";
char	*outfile = "FIXED";
int	isamfd;	

main(argc, argv)
int	argc;
char	**argv;
{
	int	i, j;
	char	c;
	struct	keys	K;

	setbuf(stdout, NULL);
	if (argc != 3) {
		fprintf(stderr, "usage: %s isamfile setupfile\n", *argv);
		exit(E_ER);
	}
	strcpy(isamfile, argv[1]);
	strcpy(setupfile, argv[2]);
	if (isamfd = isopen(isamfile, ISINPUT+ISEXCLLOCK)) {
		printerror();
		exit(E_ER);
	}
	if (isindexinfo(isamfd, (char *)&dictinfo, 0)) {
		printerror();
		exitsys();
	}
	pheader("\n\nBSETUP   L-ISAM B-tree Setup Creation");
	printf("L-ISAM File: %s.idx\n\n", isamfile);
	if ((sfile = fopen(setupfile, "r")) != NULL) {
		printf("\n%cSetup file already exists, overwrite (y/n) ? ",
			BELL);
		for (;;) {
			gets(sarg);
			if ((c = tolower(*sarg)) == 'y')
				break;
			else if (c == 'n') {
				fclose(sfile);
				exitsys();
			}
			else
				printf("\n%cOverwrite (y/n) ? ", BELL);
		}
		fclose(sfile);
	}
	if (!(sfile = fopen(setupfile, "w"))) {
		fprintf(stderr, "%s: can't create %s\n", *argv, setupfile);
		exitsys();
	}
	oisam = strlen(isamfile);
	nisam = strlen(outfile);
	fwrite((char *)&oisam, sizeof(short), 1, sfile);
	fwrite((char *)isamfile, sizeof(char), oisam, sfile);
	fwrite((char *)&nisam, sizeof(short), 1, sfile);
	fwrite((char *)outfile, sizeof(char), nisam, sfile);
	recordlength = dictinfo.di_recsize;
	fwrite((char *)&recordlength, sizeof(short), 1, sfile);
	kcnt = dictinfo.di_nkeys;
	fwrite((char *)&kcnt, sizeof(short), 1, sfile);
	for (i = 1; i <= dictinfo.di_nkeys; ++i) {
		if (isindexinfo(isamfd, (char *)&K.kd, i)) {
			printerror();
			exitsys();
		}
		K.compression = (K.kd.k_flags & COMPMASK ? COMPRESS : 0);
		K.duplicates = (K.kd.k_flags & DUPSMASK ? ISDUPS : 0);
		dmpkey(&K);
	}
	fclose(sfile);
	isclose(isamfd);
	printf("\nL-ISAM   setup file \"%s\" successfully created\n\n",
		setupfile);
}

exitsys() {
	isclose(isamfd);
	exit(E_ER);
}
