#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/times.h>

void 
get_cur_time(unsigned long *now)
{
	struct timeval  tm;
	unsigned long a;
	gettimeofday(&tm, NULL);
	a = tm.tv_sec;
	a *= (1000*1000);
	a += tm.tv_usec;
	a *= 1000;
	*now = a;
}
