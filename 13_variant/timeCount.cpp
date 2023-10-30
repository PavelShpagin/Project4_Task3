#include "timeCount.h"

template <typename F>
long long timeCount(F&& f)
{
    auto time1 = std::chrono::high_resolution_clock::now();
    f();
    auto time2 = std::chrono::high_resolution_clock::now();
    auto duration = time2 - time1;
    return duration.count();
}