#include "BTime.hpp"
#include <sys/time.h>


int BTime::GetCurrentUtcSecond()
{
    struct  timeval    tv;
    struct  timezone   tz;
    gettimeofday(&tv,&tz);
    return tv.tv_sec;
}
