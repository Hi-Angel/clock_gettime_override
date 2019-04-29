#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

extern "C"
int clock_gettime(clockid_t clk_id, struct timespec *tp) {
    using clock_gettime_ptr = int (*)(clockid_t, struct timespec*);
    const static clock_gettime_ptr real_clock_gettime
        = (clock_gettime_ptr) dlsym(RTLD_NEXT, "clock_gettime");
    int ret = real_clock_gettime(clk_id, tp);
    if (ret != -1) {
        const short AMOUNT_TO_SLOW = 4;
        const time_t NSEC_FRAC = 1000000000 / AMOUNT_TO_SLOW;
        const long nsec_part = NSEC_FRAC * (tp->tv_sec % AMOUNT_TO_SLOW);
        tp->tv_nsec = tp->tv_nsec / AMOUNT_TO_SLOW + nsec_part;
        tp->tv_sec = tp->tv_sec / AMOUNT_TO_SLOW;
    }
    return ret;
}
