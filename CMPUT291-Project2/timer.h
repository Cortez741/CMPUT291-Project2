#ifndef TIMER_INCLUDED
#define TIMER_INCLUDED

#include <stdlib.h>
#include <sys/timeb.h>

typedef struct TimerStruct Timer;
struct TimerStruct {
	struct timeb start, end;
	void(*begin)(Timer*);
	int(*stop)(Timer*);
	int running;
};
void _initTimer(Timer *self);
void _begin(Timer* self);
int _stop(Timer* self);
Timer* TimerCreate();

#endif