#define _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#include "die.h"
#include "ioops.h"
#include "str.h"
#include "xutils.h"

int open_or_die(const char *file, int flags)
{
	int ret = open(file, flags);
	if (ret < 0)
		panic("Cannot open file %s! %s.\n", file, strerror(errno));

	return ret;
}

int open_or_die_m(const char *file, int flags, mode_t mode)
{
	int ret = open(file, flags, mode);
	if (ret < 0)
		panic("Cannot open or create file %s! %s.", file, strerror(errno));
	return ret;
}

void create_or_die(const char *file, mode_t mode)
{
	int fd = open_or_die_m(file, O_WRONLY | O_CREAT, mode);
	close(fd);
}

void pipe_or_die(int pipefd[2], int flags)
{
	int ret = pipe2(pipefd, flags);
	if (ret < 0)
		panic("Cannot create pipe2 event fd! %s.\n", strerror(errno));
}

int tun_open_or_die(char *name, int type)
{
	int fd, ret;
	short flags;
	struct ifreq ifr;

	if (!name)
		panic("No name provided for tundev!\n");

	fd = open_or_die("/dev/net/tun", O_RDWR);

	memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_flags = type;
	strlcpy(ifr.ifr_name, name, IFNAMSIZ);

	ret = ioctl(fd, TUNSETIFF, &ifr);
	if (ret < 0)
		panic("ioctl screwed up! %s.\n", strerror(errno));

	ret = fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
	if (ret < 0)
		panic("fctnl screwed up! %s.\n", strerror(errno));

	flags = device_get_flags(name);
	flags |= IFF_UP | IFF_RUNNING;
	device_set_flags(name, flags);

	return fd;
}

ssize_t read_or_die(int fd, void *buf, size_t len)
{
	ssize_t ret = read(fd, buf, len);
	if (ret < 0) {
		if (errno == EPIPE)
			die();
		panic("Cannot read from descriptor! %s.\n", strerror(errno));
	}

	return ret;
}

ssize_t write_or_die(int fd, const void *buf, size_t len)
{
	ssize_t ret = write(fd, buf, len);
	if (ret < 0) {
		if (errno == EPIPE)
			die();
		panic("Cannot write to descriptor! %s.", strerror(errno));
	}

	return ret;
}
