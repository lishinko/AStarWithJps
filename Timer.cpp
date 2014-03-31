#include "Timer.h"
#include <cstdio>
Timer::Timer(void)
{
}

Timer::~Timer(void)
{
}

void Timer::start(){
	ftime(&ts1);//开始计时
}
void Timer::end(){
	ftime(&ts2);//停止计时
}
TIME_T Timer::getTimer(){
    TIME_T t1,t2;
    TIME_T ti;
    ftime(&ts2);//停止计时
    t1=(TIME_T)ts1.time*1000+ts1.millitm;
	std::printf("t1=%I64d\n",t1);
    t2=(TIME_T)ts2.time*1000+ts2.millitm;
    std::printf("t2=%I64d\n",t2);
    ti=t2-t1;//获取时间间隔，ms为单位的
	return ti;
}