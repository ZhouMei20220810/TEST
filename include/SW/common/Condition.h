/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CONDITION_H
#define CONDITION_H
#include <limits.h>
#include <stdint.h>
#include <sys/types.h>
#if !defined(_WIN32)
# include <pthread.h>
# include <sys/time.h>
#endif

#include <common/common.h>
#include <common/Mutex.h>

//// ---------------------------------------------------------------------------
//namespace android {
//// ---------------------------------------------------------------------------

/*
 * Condition variable class.  The implementation is system-dependent.
 *
 * Condition variables are paired up with mutexes.  Lock the mutex,
 * call wait(), then either re-wait() if things aren't quite what you want,
 * or unlock the mutex and continue.  All threads calling wait() must
 * use the same mutex for a given Condition.
 */
class AWE_DECLARE_DATA Condition {
public:
    enum {
        PRIVATE = 0,
        SHARED = 1
    };
    enum WakeUpType {
        WAKE_UP_ONE = 0,
        WAKE_UP_ALL = 1
    };
    Condition();
    explicit Condition(int type);
    ~Condition();
    // Wait on the condition variable.  Lock the mutex before calling.
    // Note that spurious wake-ups may happen.
    status_t wait(Mutex& mutex);
    // same with relative timeout
    status_t waitRelative(Mutex& mutex, int64_t reltime);
    // Signal the condition variable, allowing one thread to continue.
    void signal();
    // Signal the condition variable, allowing one or all threads to continue.
    void signal(WakeUpType type) {
        if (type == WAKE_UP_ONE) {
            signal();
        } else {
            broadcast();
        }
    }
    // Signal the condition variable, allowing all threads to continue.
    void broadcast();
private:
#if !defined(_WIN32)
    pthread_cond_t mCond;
#else
    void*   mState;
#endif
};
// ---------------------------------------------------------------------------
#if !defined(_WIN32)

inline Condition::Condition() {
    pthread_cond_init(&mCond, NULL);
}
inline Condition::Condition(int type) {
    if (type == SHARED) {
        pthread_condattr_t attr;
        pthread_condattr_init(&attr);
        pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_cond_init(&mCond, &attr);
        pthread_condattr_destroy(&attr);
    } else {
        pthread_cond_init(&mCond, NULL);
    }
}
inline Condition::~Condition() {
    pthread_cond_destroy(&mCond);
}
inline status_t Condition::wait(Mutex& mutex) {
    return -pthread_cond_wait(&mCond, &mutex.mMutex);
}
inline status_t Condition::waitRelative(Mutex& mutex, int64_t reltime) {
    struct timespec ts;
#if defined(__linux__)
    clock_gettime(CLOCK_REALTIME, &ts);
#else // __APPLE__
    // Apple doesn't support POSIX clocks.
    struct timeval t;
    gettimeofday(&t, NULL);
    ts.tv_sec = t.tv_sec;
    ts.tv_nsec = t.tv_usec*1000;
#endif

    // On 32-bit devices, tv_sec is 32-bit, but `reltime` is 64-bit.
    int64_t reltime_sec = reltime/1000000000;

    ts.tv_nsec += reltime%1000000000;
    if (reltime_sec < INT64_MAX && ts.tv_nsec >= 1000000000) {
        ts.tv_nsec -= 1000000000;
        ++reltime_sec;
    }

    int64_t time_sec = ts.tv_sec;
    if (time_sec > INT64_MAX - reltime_sec) {
        time_sec = INT64_MAX;
    } else {
        time_sec += reltime_sec;
    }

#if defined(__LP64__)
    ts.tv_sec = time_sec;
#else
    ts.tv_sec = (time_sec > INT32_MAX) ? INT32_MAX : time_sec;
#endif

    return -pthread_cond_timedwait(&mCond, &mutex.mMutex, &ts);
}
inline void Condition::signal() {
    /*
     * POSIX says pthread_cond_signal wakes up "one or more" waiting threads.
     * However bionic follows the glibc guarantee which wakes up "exactly one"
     * waiting thread.
     *
     * man 3 pthread_cond_signal
     *   pthread_cond_signal restarts one of the threads that are waiting on
     *   the condition variable cond. If no threads are waiting on cond,
     *   nothing happens. If several threads are waiting on cond, exactly one
     *   is restarted, but it is not specified which.
     */
    pthread_cond_signal(&mCond);
}
inline void Condition::broadcast() {
    pthread_cond_broadcast(&mCond);
}
#endif // !defined(_WIN32)
//// ---------------------------------------------------------------------------
//}  // namespace android
//// ---------------------------------------------------------------------------
#endif // CONDITION_H
