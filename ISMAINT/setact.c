#include "../include/iisam.h"
#include "../include/ismaint.h"

setact(idx)
int	idx;
{
	struct	keys	*tmp = head;
	int	count = 0;

	while (tmp) {
		if (++count == idx) {
			actkey = tmp;
			return(0);
		}
		tmp = tmp->link;
	}
	return(1);
}
