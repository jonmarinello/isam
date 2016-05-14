#include "../include/iisam.h"
#include "../include/ismaint.h"

/*	
	Display ISAM file version information.
*/

main(argc, argv)
int	argc;
char	**argv;
{
	int	fd;
	char	shofile[64];
	char	vbuf[128];
	US_LONG	ul;
	long	l;

# ifdef VMS
	if (0)
	    iopen();	/* Needed to fool VMS linking loader */
# endif
	if (argc != 2) {
		fprintf(stderr, "usage: %s isam_file\n", *argv);
		exit(E_ER);
	}
	sprintf(shofile, "%s.idx", argv[1]);
	if ((fd = open(shofile, O_RDONLY)) < 0) {
		printf("%s: can't open %s\n", *argv, argv[1]);
		exit(E_ER);
	}
	pheader("\n\nSHOVERS	L-ISAM B-tree Display");
	read(fd, (char *)&l, (unsigned)LONGSIZE);
	printf("Number of key sortings:            %ld\n", l);
	read(fd, (char *)&l, (unsigned)LONGSIZE);
	printf("Offset of data free list:          %ld\n", l);
	read(fd, (char *)&l, (unsigned)LONGSIZE);
	printf("Offset of index free list:         %ld\n", l);
# ifdef ALTOSX86
	read(fd, (char *)&l, (unsigned)LONGSIZE);
	printf("Internal unique counter:           %ld\n", l);
# else
	read(fd, (char *)&ul, (unsigned)LONGSIZE);
	printf("Internal unique counter:           %lu\n", ul);
# endif
	read(fd, (char *)&l, (unsigned)LONGSIZE);
	printf("User referenced uniqueness number: %ld\n", l);
	read(fd, (char *)&l, (unsigned)LONGSIZE);
	printf("Length of data records:            %ld\n", l);
	read(fd, (char *)&l, (unsigned)LONGSIZE);
	printf("Number of data records:            %ld\n", l);
	read(fd, (char *)&l, (unsigned)LONGSIZE);
	read(fd, vbuf, 20);
	if (strncmp(vbuf, "version", 7)) {
		printf("Pre-Version 2.01 file.\n");
		close(fd);
		exit(E_OK);
	}
	printf("Serial number:                     %ld\n", l);
	printf("Internal version number:           %s.\n", vbuf);
	read(fd, vbuf, 50);
	printf("Internal copyright:                %s.\n", vbuf);
	close(fd);
	exit(E_OK);
}
