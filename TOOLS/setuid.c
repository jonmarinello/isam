#include "../include/iisam.h"
#include "../include/ismaint.h"

/* Reset ISAM file's unique id number */

main(argc, argv)
int	argc;
char	**argv;
{
	int	fd;
	char	uidfile[64];
	long	uid;
	extern	long	atol();

	if (argc != 3) {
		fprintf(stderr, "usage: %s uid isam_file\n", *argv);
		exit(E_ER);
	}
	uid = atol(argv[1]);
	if (uid < 0L) {
		fprintf(stderr, "%s: uid must be greater than zero\n", *argv);
		exit(E_ER);
	}
	sprintf(uidfile, "%s.idx", argv[2]);
	if ((fd = open(uidfile, O_RDWR)) >= 0) {
		lseek(fd, (long)(4 * sizeof(long)), 0);
		write(fd, (char *)&uid, sizeof(long));
		close(fd);
	}
	else {
		printf("%s: can't open %s\n", *argv, argv[2]);
		exit(E_ER);
	}
	exit(E_OK);
}
