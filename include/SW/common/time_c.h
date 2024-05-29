/*
 * time_c.h
 *
 */

#ifndef TIME_C_H_
#define TIME_C_H_

#include <stdint.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "common/common.h"

/** number of microseconds per second */
#define AWE_USEC_PER_SEC INT64_C(1000000)

/** @return apr_time_t as a second */
#define awe_time_sec(time) ((time) / AWE_USEC_PER_SEC)

AWE_BEGIN_DECLS

/**
 * a structure similar to ANSI struct tm with the following differences:
 *  - tm_usec isn't an ANSI field
 *  - tm_gmtoff isn't an ANSI field (it's a BSDism)
 */
typedef struct time_exp_t {
    /** microseconds past tm_sec */
    int32_t tm_usec;
    /** (0-61) seconds past tm_min */
    int32_t tm_sec;
    /** (0-59) minutes past tm_hour */
    int32_t tm_min;
    /** (0-23) hours past midnight */
    int32_t tm_hour;
    /** (1-31) day of the month */
    int32_t tm_mday;
    /** (0-11) month of the year */
    int32_t tm_mon;
    /** year since 1900 */
    int32_t tm_year;
    /** (0-6) days since Sunday */
    int32_t tm_wday;
    /** (0-365) days since January 1 */
    int32_t tm_yday;
    /** daylight saving time */
    int32_t tm_isdst;
    /** seconds east of UTC */
    int32_t tm_gmtoff;
}time_exp_t;

/**
 * @return the current time millisecond
 */
AWE_DECLARE(int64_t) get_time_now(void);

/**
 * @return the current time microseconds
 */
AWE_DECLARE(int64_t) get_time_now_ms(void);

/**
 * Convert a time to its human readable components (GMT).
 * @param result the exploded time
 * @param input the time to explode
 */
int time_exp_lt(time_exp_t *result, int64_t input);

void delay_ms(int32_t ms);

#if defined(_WIN32)

struct timezone {
    int tz_minuteswest; /* minutes W of Greenwich */
    int tz_dsttime; /* type of dst correction */
};

int gettimeofday(struct timeval *tv, struct timezone *tz);

#endif

AWE_END_DECLS

#endif /* TIME_C_H_ */
