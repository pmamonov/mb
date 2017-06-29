#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#define SZ (32 << 20)

int main(int argc, char **argv)
{
	void *m, *m1;
	volatile uint64_t *p, *p1;
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
		__asm__ __volatile__ (
			"pref    0x1e, 256(%0)\n\t"
			"pref    0x1e, 288(%0)\n\t"
			"pref    0x1e, 320(%0)\n\t"
			"pref    0x1e, 352(%0)\n\t"
			::"r" (p));
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

	len = sz / sizeof(*p) / 16;
	p = m;
	p1 = m1;
	start();
	while (len > 0) {
		__asm__ __volatile__ (
			"pref    0x1e, 256(%0)\n\t"
			"pref    0x1e, 288(%0)\n\t"
			"pref    0x1e, 320(%0)\n\t"
			"pref    0x1e, 352(%0)\n\t"
			::"r" (p));
		p[0] = p1[0];
		p[1] = p1[1];
		p[2] = p1[2];
		p[3] = p1[3];
		p[4] = p1[4];
		p[5] = p1[5];
		p[6] = p1[6];
		p[7] = p1[7];
		p[8] = p1[8];
		p[9] = p1[9];
		p[10] = p1[10];
		p[11] = p1[11];
		p[12] = p1[12];
		p[13] = p1[13];
		p[14] = p1[14];
		p[15] = p1[15];
		len -= 1;
		p += 16;
		p1 += 16;
	}
	finish("copy64");

	printf(capt);
	printf("\n");
	printf(row);
	printf("\n");
}
