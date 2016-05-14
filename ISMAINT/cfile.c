#include "../include/iisam.h"
#include "../include/ismaint.h"

createfile() {
	int	i = 2;
	int	fd;
	char	uidfile[64];
	
	head->kd.k_flags = head->compression + head->duplicates;
	nfp = isbuild(nfilename, recordlength, &head->kd, ISINOUT+ISEXCLLOCK);
	if (iserrno == EEXIST) {
		printerror();
		return;
	}
	else if (iserrno == EBADKEY) {
		printerror();
		return;
	}
	else if (iserrno) {
		printerror();
		rmnewfiles();
		return;
	}
	sprintf(uidfile, "%s.idx", nfilename);
	if ((fd = open(uidfile, O_RDWR)) >= 0) {
		lseek(fd, (long)(4 * sizeof(long)), 0);
		write(fd, (char *)&uid, sizeof(long));
		close(fd);
	}
	else {
		printf("\n\nWARNING!!! Can't create UNIQUEID in new index\n\n");
		uid = 0L;
	}
	actkey = head;
	while (actkey->link != (struct keys *)NULL) {
		actkey = actkey->link;
		actkey->kd.k_flags = actkey->compression +
			actkey->duplicates;
		isaddindex(nfp, &actkey->kd);
		if (iserrno) {
			printerror();
			rmnewfiles();
		}
		++i;
	}
}
