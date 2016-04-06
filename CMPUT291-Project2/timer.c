#include "timer.h"

void _begin(Timer* self)
{
	ftime(&(self->start));
	self->running = 1;
}

int _stop(Timer* self)
{
	if (self->running == 1)
	{
		ftime(&(self->end));
		int diff = (int)(1000.0 * (self->end.time - self->start.time) + (self->end.millitm - self->start.millitm));
		self->running = 0;
		return diff;
	}
	else
	{
		return -1;
	}
}

void _initTimer(Timer* self)
{
	self->begin = _begin;
	self->stop = _stop;
}

Timer* TimerCreate()
{
	Timer _T;
	memset(&_T, 0, sizeof(_T));
	_init(&_T);
	return &_T;
}