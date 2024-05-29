/*
 * Timer.h
 *
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

#include <list>

#include "common/thread.h"
#include "common/Mutex.h"
#include "common/Condition.h"
#include "common/Utils.h"

/**
 * delay task state
 */
typedef enum timer_task_state{
	TIMER_TASK_VALID	= 0, //task valid
	TIMER_TASK_INVALID	= 1, //task invalid
}timer_task_state;

typedef void* TimerID;
typedef void timer_task_proc(void *userdata, intptr_t arg);

class AWE_DECLARE_DATA TimerTask{
public:
	TimerTask(int64_t procms, timer_task_proc* proc, void* userdata, intptr_t arg);
	~TimerTask();

	int64_t				_proc_ms; // proc time

	timer_task_proc*	_proc;
	void*				_userdata;
	intptr_t			_arg;

	volatile int32_t	_state;
};

class AWE_DECLARE_DATA Timer{
public:
	Timer(const char* name);
	~Timer();

	int start(uint32_t id = 0);
	void stop();

	uint32_t getId();

	TimerID post(int32_t delay_ms, timer_task_proc* proc, void* userdata, intptr_t arg);

	bool cancel(TimerID* id);

	void clear();
private:
	static int TimerThreadProcess(awe_thread_t *thread, void* userdata);

	Mutex mMutex;
	Condition mCond;

	uint32_t mId = 0;

    std::list<TimerTask*> mTaskList;

    awe_thread_t* mThread = NULL;
    char mThreadName[32];

	volatile bool mStarted = false;

	DISALLOW_EVIL_CONSTRUCTORS(Timer);
};

#endif /* TIMER_H_ */
