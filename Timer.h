#pragma once

#include <sys/timeb.h>
#if defined(WIN32)
	# define  TIMEB    _timeb
	# define  ftime    _ftime
	typedef __int64 TIME_T;
#else
	#define TIMEB timeb
	typedef long long TIME_T;
#endif

class Timer
{
public:
	Timer(void);
	~Timer(void);
	TIME_T getTimer();
	void start();
	void end();
protected:
	struct TIMEB ts1,ts2;
};
