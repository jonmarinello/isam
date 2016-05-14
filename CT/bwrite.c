#include "console.h"

# define DICTIONINFO     0

int     fp1 = -1;
int     livefp;
char    *fname1 = PRI;
char    *livefname;
struct  CON     *tstruct;
extern	char	*malloc();

main() {
	int     i;

        tstruct = (struct CON *)malloc(sizeof(struct CON));
	strcpy(tstruct->comment, "                    ");
	openfile(&fp1, fname1);
	trace(2);
	for (i = 0; i <= 500; ++i)
		addrecs(i);
}

addrecs(i)
int	i;
{
	printf("writing record: %08d\n", i);
	tstruct->key = i;
        iswrite(livefp, (char *)tstruct);
        if (iserrno)
                printf("\nWrite error on %s err=%03d\n", fname1, iserrno);
}

openfile(fp, filename)
int     *fp;
char    *filename;
{
        int     tfp;

        if ((tfp = isopen(filename, ISINOUT+ISMANULOCK)) < 0)
                printf("update: can't open %s, err=%03d %d\n", filename, iserrno, tfp);
        else {
                livefp = *fp = tfp;
                livefname = filename;
        }
}
