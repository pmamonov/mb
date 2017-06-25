#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#define SZ (32 << 20)

int main(int argc, char **argv)
{
	void *m, *m1;
	register uint64_t *p, *end, x;
	struct timeval tv, tv1;
	size_t sz = SZ;
	char capt[4096];
	char *s = capt;
	char row[4096];
	char *r = row;

#define start()		gettimeofday(&tv, NULL)

#define finish(label) {						\
	unsigned long long us;					\
								\
	gettimeofday(&tv1, NULL);				\
	us = 1000000 * (tv1.tv_sec - tv.tv_sec);		\
	us += (tv1.tv_usec - tv.tv_usec);			\
	r += sprintf(r, "%10lld", (1000000 * sz / us) >> 20);	\
	s += sprintf(s, "%10s", label);				\
}

	if (argc >= 2)
		sz = atol(argv[1]) << 20;

	m = malloc(sz);
	m1 = malloc(sz);

	if (!m || !m1) {
		printf("ERROR: failed to alloc 2 x %d bytes\n", sz);
		return 1;
	}

	memset(m, -1, sz);
	memset(m1, -1, sz);

	start();
	memset(m, 0, sz);
	finish("memset");

	start();
	memcpy(m1, m, sz);
	finish("memcpy");

	end = m + sz;
	start();
	for (p = m; p != end; x = *p++)
		;
	finish("read64");

	start();
	for (p = m; p != end; *p++ = x)
		;
	finish("write64");

	printf(capt);
	printf("\n");
	printf(row);
	printf("\n");
}
