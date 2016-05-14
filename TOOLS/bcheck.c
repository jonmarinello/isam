#include "../include/iisam.h"
#include "../include/ismaint.h"

/*			bcheck - display dictionary and key description information
			as well as the index file (description by description)
*/

# define	MAXSLEN		128

char	*msg_2 = "Key   Flags   Parts  Length    Root   Dups   Compression\n";
int	freeon = 1;
char	filename[MAXSLEN];
char	bname[MAXSLEN];
int	fd;
struct	keydesc	keydesc;
struct	idict	intdict;
int	fp;	
char	*malloc();
char	*record;
char	fmtctrl[MAXSLEN];
char	syscom[MAXSLEN];

short	sfmt;
int	ifmt;
long	lfmt;
float	ffmt;
double	dfmt;

main(argc, argv)
int	argc;
char	**argv;
{
	int	i, j;

	setbuf(stdout, NULL);
	switch (argc) {
		case 2 :
			strcpy(filename, argv[1]);
			break;
		default :
			fprintf(stderr, "usage: %s [filename]\n", *argv);
			exit(E_ER);
	}
	printf("\n\nBCHECK   L-ISAM B-tree Checker %s\n", isversnumber);
	printf("%s\n", iscopyright);
	printf("Software Serial Number %ld\n\n", isserial);
	printf("L-ISAM File: %s.idx\n\n", filename);
	sprintf(bname, "%s.idx", filename);
	if ((fd = open(bname, O_RDONLY)) < 0) {
		fprintf(stderr, "%s: can't open %s\n", *argv, bname);
		exit(E_ER);
	}
	read(fd, (char *)&intdict, IDSIZE);
	close(fd);
	if (fp = isopen(filename, ISINPUT+ISEXCLLOCK)) {
		printerror();
		exit(E_ER);
	}
	printf("Dictionary:\n\n");
	printf("\t  key descriptions: %ld\n", intdict.dsccnt);
	printf("\tdata record length: %ld\n", intdict.datalength);
	printf("\t         page size: %ld\n", PAGESIZE);
	printf("\t   unique id value: %ld\n", intdict.unique);
	printf("\t number of records: %ld\n\n", intdict.reccount);
	record = malloc((unsigned)(sizeof(char) * intdict.datalength + 1));
	printf("\t(Start,Length,Type,Asc/Desc)\t  CHARTYPE = 0\n");	
	printf("\t\t\t\t\t   INTTYPE = 1\n");	
	printf("\t\t\t\t\t  LONGTYPE = 2\n");	
	printf("\t\t\t\t\tDOUBLETYPE = 3\n");	
	printf("\t\t\t\t\t FLOATTYPE = 4\n");	
	printf("\t\t\t\t\t SHORTTYPE = 5\n\n");	
	for (i = 1; i <= intdict.dsccnt; ++i) {
		if (isindexinfo(fp, (char *)&keydesc, i)) {
			printerror();
			exit(E_ER);
		}
		printf(msg_2);
		printf("%3d   %5d   %5d   %5d  %6ld    %3s       %3s\n",
			i, keydesc.k_flags, keydesc.k_nparts,
			keydesc.k_len - LONGSIZE, keydesc.k_rootnode,
			(keydesc.k_flags & DUPSMASK ? "YES" : "NO"),
			(keydesc.k_flags & COMPMASK ? "YES" : "NO"));
		for (j = 0; j < keydesc.k_nparts; ++j)
			printf("(%d,%d,%d,%c) ", keydesc.k_part[j].kp_start,
			keydesc.k_part[j].kp_leng,
			keydesc.k_part[j].kp_type & TYPEMASK,
			(keydesc.k_part[j].kp_type & ISDESC ? '-' : '+'));
		printf("\n\n");
		isstart(fp, &keydesc, 0, record, ISFIRST);
		while(!isread(fp, record, ISNEXT)) {
			for (j = 0; j < keydesc.k_nparts; ++j) {
				buildfmtctrl(&keydesc, j);
				putchar('|');
			}
			putchar('\n');
		}
		if ((iserrno != EENDFILE) && (iserrno != ENOCURR)) {
			printerror();
			exit(E_ER);
		}
		printf("(EOF)\n\n");
	}
	isclose(fp);
	if (freeon) {
		printf("Index Node and Data Record Free Lists:\n\n");
		sprintf(syscom, "freedump %s", filename);
		system(syscom);
	}
}

buildfmtctrl(keydesc, part)
struct	keydesc	*keydesc;
int	part;
{
	struct	keypart	*keypart = &keydesc->k_part[part];

	switch(keypart->kp_type & TYPEMASK) {
	case CHARTYPE :
		if (keypart->kp_leng == 1)
			printf("%c", record[keypart->kp_start]);
		else
			printf("%s", record + keypart->kp_start);
		break;
	case SHORTTYPE :
		unstring(SHORTTYPE, keypart->kp_start);
		printf("%d", sfmt);
		break;
	case INTTYPE :
		unstring(INTTYPE, keypart->kp_start);
		printf("%d", ifmt);
		break;
	case LONGTYPE :
		unstring(LONGTYPE, keypart->kp_start);
		printf("%ld", lfmt);
		break;
	case FLOATTYPE :
		unstring(FLOATTYPE, keypart->kp_start);
		printf("%f", ffmt);
		break;
	case DOUBLETYPE :
		unstring(DOUBLETYPE, keypart->kp_start);
		printf("%f", dfmt);
		break;
	}
}

unstring(type, start)
int	type;
int	start;
{
	int	i;
	char	*ptr;

	switch(type) {
	case SHORTTYPE :
		ptr = (char *)&sfmt;
		for (i = 0; i < SHORTSIZE; ++i)
			*ptr++ = record[start + i];
		break;
	case INTTYPE :
		ptr = (char *)&ifmt;
		for (i = 0; i < INTSIZE; ++i)
			*ptr++ = record[start + i];
		break;
	case LONGTYPE :
		ptr = (char *)&lfmt;
		for (i = 0; i < LONGSIZE; ++i)
			*ptr++ = record[start + i];
		break;
	case FLOATTYPE :
		ptr = (char *)&ffmt;
		for (i = 0; i < FLOATSIZE; ++i)
			*ptr++ = record[start + i];
		break;
	case DOUBLETYPE :
		ptr = (char *)&dfmt;
		for (i = 0; i < DOUBLESIZE; ++i)
			*ptr++ = record[start + i];
		break;
	}
}
