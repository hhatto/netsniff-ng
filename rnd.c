#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "rnd.h"

static int fd_rnd = -1;

static void randombytes(unsigned char *x, unsigned long long xlen)
{
	int ret;

	if (fd_rnd == -1) {
		for (;;) {
			fd_rnd = open("/dev/urandom", O_RDONLY);
			if (fd_rnd != -1)
				break;
			sleep(1);
		}
	}

	while (xlen > 0) {
		if (xlen < 1048576)
			ret = xlen;
		else
			ret = 1048576;

		ret = read(fd_rnd, x, ret);
		if (ret < 1) {
			sleep(1);
			continue;
		}

		x += ret;
		xlen -= ret;
	}
}

/* Note: it's not really secure, but the name only suggests it's better to use
 * than rand(3) when transferring bytes over the network in non-security
 * critical structure members. secrand() is only used to fill up salts actually.
 */
int secrand(void)
{
	int ret;
	randombytes((void *) &ret, sizeof(ret));
	return ret;
}
