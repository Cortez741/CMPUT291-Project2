#include "timer.h"
#include <stddef.h>

void clock_readtime(long * s)
{
	struct timeval timer;
	gettimeofday(&timer, NULL);
	s = (long *)timer.tv_usec;
}

long diff_time(long* end, long* start)
{
	return (end - start) /1000;
}



