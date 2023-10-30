#ifndef TIMECOUNT_H
#define TIMECOUNT_H

#include <chrono>

template <typename F>
long long timeCount(F&& f);

#endif 