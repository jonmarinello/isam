#include "../include/iisam.h"
#include "../include/ismaint.h"

/*	
		Print B-TREE free lists (DEBUGGING PURPOSES ONLY)
*/

# define	BELL		'\007'
# define	MAXSLEN		128

char	*msg_1 = "%s: isam open error (%03d) on \"%s\"\n";
char	filename[MAXSLEN];
int	fp;	

main(argc, argv)
int	argc;
char	**argv;
{
	setbuf(stdout, NULL);
	switch (argc) {
		case 2 :
			strcpy(filename, argv[1]);
			break;
		case 1 :
			getfile();
			break;
		default :
			fprintf(stderr, "usage: %s [filename]\n", *argv);
			exit(E_ER);
	}
	if (fp = isopen(filename, ISINPUT)) {
		fprintf(stderr, msg_1, *argv, iserrno, filename);
		exit(E_ER);
	}
	printflist();
	isclose(fp);
}


getfile() {	/* Get name of isam file */
	do {
		printf("Filename: ");
		gets(filename);
	} while (!strlen(filename));
}


printflist() {
	struct	flist	flpage;
	int	i, j;
	long	offset;
	int	ifp, dfp;

	ifp = ftbl[0].idxfd;
	dfp = ftbl[0].datfd;
	if (ftbl[0].dptr.kfree != NILL) {
		offset = ftbl[0].dptr.kfree;
		j = 1;
		printf("**Index freelist:");
		lseek(ifp, ftbl[0].dptr.kfree, 0);
		do {
			read(ifp, (char *)&flpage, (int)PAGESIZE);
			if (flpage.fidx < 0) {
				printf(" (NULL)");
				break;
			}
			printf("\n\t(Offset = %ld) Free page (%d):\n",
				offset, j++);
			for (i = flpage.fidx; i >= 0; --i)
				printf("\t\t%3d->%05ld\n", i, flpage.foffsets[i]);
			lseek(ifp, flpage.llink, 0);
			offset = flpage.llink;
		} while (flpage.llink != NILL);
		printf("\n");
	}
	if (ftbl[0].dptr.dfree != NILL) {
		offset = ftbl[0].dptr.dfree;
		j = 1;
		printf("**Data freelist:");
		lseek(ifp, ftbl[0].dptr.dfree, 0);
		do {
			read(ifp, (char *)&flpage, (int)PAGESIZE);
			if (flpage.fidx < 0) {
				printf(" (NULL)");
				break;
			}
			printf("\n\t(Offset = %ld) Free page (%d):\n",
				offset, j++);
			for (i = flpage.fidx; i >= 0; --i)
				printf("\t\t%3d->%05ld\n", i, flpage.foffsets[i]);
			lseek(ifp, flpage.llink, 0);
			offset = flpage.llink;
		} while (flpage.llink != NILL);
		printf("\n");
	}
}
