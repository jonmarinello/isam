#include "../include/iisam.h"
#include "../include/ismaint.h"

/*	
	Rebuild index file from user's ".dat" file.
*/

char	*cmfname;		/* Command line file name */
char	ofilename[MAXSLEN];
char	nfilename[MAXSLEN];
char	sfname[MAXSLEN];
char	ufile[MAXSLEN];
char	ofile[MAXSLEN];
char	sarg[MAXSLEN];
short	recordlength;
long	dupspress = 0L;
FILE	*ofp;
FILE	*sfile;
int	nfp;
char	*record;
long	livecnt;
long	deadcnt;
short	overall;
short	atoi();
short	oln, nln;
short	setup;
struct	keys	*head, *tail, *actkey;
struct	keys	*allockey();
char	isarg[512];
long	uid = 0L;
int	gotuid;

main(argc, argv)
int	argc;
char	**argv;
{
	setbuf(stdout, (char *)NULL);
	head = actkey = allockey();
	if (argc < 2 || argc > 3) {
		fprintf(stderr, "usage: %s setupfile [isam_file]\n", *argv);
		exit(E_ER);
	}
	pheader("\n\nREBUILD   L-ISAM B-tree Rebuilder");
	if (argc == 3)
		cmfname = argv[2];
	strcpy(sfname, argv[1]);
	if (loadsetup(1))
		++setup;
	if (!setup)
		getdata();
	do {
		iserrno = 0;
		while (pmenu())
			;
		if (!strcmp(ofile, nfilename)) {
			fprintf(stderr, "%c\nSource & target files have the same name\n",
				BELL);
			++iserrno;
			continue;
		}
		createfile();
		if (iserrno)
			continue;
		buildindex();
	} while (iserrno);
	isclose(nfp);
	renfiles();
	ptailer();
	exit(E_OK);
}

char	*gfname(path)
char	*path;
{
	char	*p = path + (unsigned)strlen(path);

	while (p-- > path)
		if (*p == '/')
			break;
	++p;
	return(p);
}
