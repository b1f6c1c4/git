#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "instru.h"

struct instru_t g_instrus[INSTRU_NUM] = {};

void instru_finalize(void)
{
	const char *env = getenv("INSTRU");
	if (!env)
		return;

	int fd = open(env, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0) {
		perror("open(2) during instru_finalize");
		return;
	}

	const char *ptr = (const char *)g_instrus;
	size_t sz = sizeof(g_instrus);
	while (sz) {
		ssize_t ret = write(fd, ptr, sz);
		if (ret > 0) {
			sz -= ret;
			continue;
		}
		if (errno != EINTR) {
			perror("write(2) during instru_finalize");
			return;
		}
	}

	close(fd);
}
