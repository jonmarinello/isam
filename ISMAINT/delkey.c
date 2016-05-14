#include "../include/iisam.h"
#include "../include/ismaint.h"

delkey(idx)
int	idx;
{
	int	count = 1;
	struct	keys	*del, *llink;

	if (!head)
		return(1);
	del = llink = head;
	if (idx == 1) { 	/* PRIMARY key description */
		if (!head->link) {
			free((char *)head);
			head = tail = actkey = (struct keys *)NULL;
			return(0);
		}
		llink = head->link;
		free((char *)head);
		actkey = head = llink;
			return(0);
	}
	del = head->link;
	while (del) {
		if (++count == idx) {
			llink->link = del->link;
			free((char *)del);
			if (!llink->link)
				tail = llink;
			actkey = llink;
			return(0);
		}
		llink = del;
		del = del->link;
	}
	return(1);
}
