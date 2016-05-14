#include "../include/iisam.h"
#include "../include/ismaint.h"

pmenu() {
	int	fd;
	char	uidfile[64];

	if (!*ofile || gotuid)
		goto SKUID;
	sprintf(uidfile, "%s.idx", gfname(ofile));
	if ((fd = open(uidfile, O_RDONLY)) >= 0) {
		lseek(fd, (long)(4 * sizeof(long)), 0);
		read(fd, (char *)&uid, sizeof(long));
		close(fd);
	}
	else {
		printf("\n\nWARNING!!! Can't get UNIQUEID from old index (%03d)\n\n", errno);
		uid = 0L;
	}
SKUID:
	printf("\n(A) - Old/Damaged ISAM filename: \"%s\"\n", gfname(ofile));
	printf("(B) - Temporary work filename: \"%s\"\n", nfilename);
	printf("(C) - Length of data records: %d\n", recordlength);
	printf("(D) - Access key description menu.\n");
	printf("(E) - To save this setup.\n");
	printf("(F) - To load setup.\n");
	printf("(G) - To exit from rebuild.\n");
	printf("(H) - Unique ID number: %ld.\n", uid);
	printf("(I) - Remove Temporary work filename: \"%s\"\n", nfilename);
	printf("\nHit <RETURN> to begin restore or enter choice: ");
	gets(isarg);
	switch(tolower(*isarg)) {
		case 'a' :
			gofile();
			return(1);
		case 'b' :
			gnfile();
			return(1);
		case 'c' :
			grlength();
			return(1);
		case 'd' :
			while (pkeymenu())
				;
			return(1);
		case 'e' :
			savesetup();
			return(1);
		case 'f' :
			loadsetup(0);
			return(1);
		case 'g' :
			isclose(nfp);
			exit(E_OK);
		case 'h' :
			guid();
			return(1);
		case 'i' :
			iserase(nfilename);
			return(1);
		case '\0' :
			return(0);
		default :
			printf("\n%cInvalid response, try again\n", BELL);
			return(1);
	}
}
