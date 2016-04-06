#include "timer.h"

#ifdef _WIN32
#include <windows.h>

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define U64(x) x##Ui64
#else
#define U64(x) x##ULL
#endif

#define DELTA_EPOCH_IN_100NS  U64(116444736000000000)

long ticks_to_nanos(LONGLONG subsecond_time, LONGLONG frequency)
{
	return (long)((1e9 * subsecond_time) / frequency);
}

ULONGLONG to_quad_100ns(FILETIME ft)
{
	ULARGE_INTEGER li;
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;
	return li.QuadPart;
}

void to_timespec_from_100ns(ULONGLONG t_100ns, long *t)
{
	t[0] = (long)(t_100ns / 10000000UL);
	t[1] = 100 * (long)(t_100ns % 10000000UL);
}

void clock_readtime(long* t)
{
	LARGE_INTEGER time;
	LARGE_INTEGER frequency;
	QueryPerformanceCounter(&time);
	QueryPerformanceFrequency(&frequency);
	t[0] = time.QuadPart / frequency.QuadPart; // seconds
	t[1] = ticks_to_nanos(time.QuadPart % frequency.QuadPart, frequency.QuadPart); // nanoseconds
}

#endif