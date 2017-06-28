#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#define SZ (32 << 20)

int main(int argc, char **argv)
{
	void *m, *m1;
	volatile uint64_t *p;
	register uint64_t x;
	struct timeval tv, tv1;
	size_t sz = SZ, len;
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
	r += sprintf(r, "%10lld", (1000000ull * sz / us) >> 20);\
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

	len = sz / sizeof(*p) / 16;
	p = m;
	start();
	while (len > 0) {
		x = p[0];
		x = p[1];
		x = p[2];
		x = p[3];
		x = p[4];
		x = p[5];
		x = p[6];
		x = p[7];
		x = p[8];
		x = p[9];
		x = p[10];
		x = p[11];
		x = p[12];
		x = p[13];
		x = p[14];
		x = p[15];
		len -= 1;
		p += 16;
	}
	finish("read64");

	len = sz / sizeof(*p) / 16;
	p = m;
	start();
	while (len > 0) {
		p[0] = x;
		p[1] = x;
		p[2] = x;
		p[3] = x;
		p[4] = x;
		p[5] = x;
		p[6] = x;
		p[7] = x;
		p[8] = x;
		p[9] = x;
		p[10] = x;
		p[11] = x;
		p[12] = x;
		p[13] = x;
		p[14] = x;
		p[15] = x;
		len -= 1;
		p += 16;
	}
	finish("write64");

	printf(capt);
	printf("\n");
	printf(row);
	printf("\n");
}
