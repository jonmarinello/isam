#include "iisam.h"

#ifdef NO_LOCKING

int	isplock(int fildes, int function, long size)
{
	errno = 0;
	fildes = 0;
	function = 0;
	size = 0L;
        return(0);
}

#else
#ifdef LOCKF    /* Standard system V locking */
#ifdef NO_FTEST

int	isplock(int fildes, int function, long size)
{
        errno = 0;
        if (function == F_TEST) {
                if (lockf(fildes, F_TLOCK, size) < 0) {
                        if (errno == EDEADLK) {
                                fprintf(stderr, lockerror);
                                abort();
                        }
                        return(-1);
                }
                else {
                        if (lockf(fildes, F_ULOCK, size) < 0) {
                                if (errno == EDEADLK) {
                                        fprintf(stderr, lockerror);
                                        abort();
                                }
                        }
                        return(0);
                }
        }
        else {
                if (lockf(fildes, function, size) < 0) {
                        if (errno == EDEADLK) {
                                fprintf(stderr, lockerror);
                                abort();
                        }
                        return(-1);
                }
                return(0);
        }
}

#else

int	isplock(int fildes, int function, long size)
{
        errno = 0;
        if (lockf(fildes, function, size) < 0) {
                if (errno == EDEADLK) {
                        fprintf(stderr, lockerror);
                        abort();
                }
                return(-1);
        }
        return(0);
}

#endif
#else

#ifdef LOCKING          /* Old locking */
#ifdef  SUPERMAX        /* Supermax version doesn't support sleep or TEST */

int	isplock(int fildes, int function, long size)
{
        int     lret;

        if (function == F_LOCK) {       /* Simulate sleep */
                do {
                        errno = 0;
                        lret = locking(fildes, function, (int)size);
                        if (errno == EDEADLK) {
                                fprintf(stderr, lockerror);
                                abort();
                        }
                } while (errno == EACCES);
                return(lret);
        }
        else {
                errno = 0;
                if (function == F_TEST) {
                        if (locking(fildes, F_TLOCK, size)) {
                                if (errno == EDEADLK) {
                                        fprintf(stderr, lockerror);
                                        abort();
                                }
                                return(-1);
                        }
                        else {
                                if (locking(fildes, F_ULOCK, size)) {
                                        if (errno == EDEADLK) {
                                                fprintf(stderr, lockerror);
                                                abort();
                                        }
                                }
                                return(0);
                        }
                }
                else {
                        if (locking(fildes, function, size)) {
                                if (errno == EDEADLK) {
                                        fprintf(stderr, lockerror);
                                        abort();
                                }
                                return(-1);
                        }
                        return(0);
                }
        }
}

#else           /* Locking() doesn't support region lock TEST */

int	isplock(int fildes, int function, long size)
{
	errno = 0;
	return 0;			/* Temp fix to debug single user */
        if (function == F_TEST) {
		if (locking(fildes, F_TEST, size)) {
			if (errno == EDEADLOCK) {
                                fprintf(stderr, lockerror);
                                abort();
                        }
                        return(-1);
                }
                else {
                        if (locking(fildes, F_ULOCK, size)) {
				if (errno == EDEADLOCK) {
                                        fprintf(stderr, lockerror);
                                        abort();
                                }
                        }
                        return(0);
                }
        }
        else {
                if (locking(fildes, function, size)) {
			if (errno == EDEADLOCK) {
                                fprintf(stderr, lockerror);
                                abort();
                        }
                        return(-1);
                }
                return(0);
        }
}

#endif
#endif
#endif
#endif