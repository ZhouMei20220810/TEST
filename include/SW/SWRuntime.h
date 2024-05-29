/*
 * SWRuntime.h
 */

#ifndef SWRUNTIME_H_
#define SWRUNTIME_H_

#include <string>

#include <common/common.h>
#include <common/Utils.h>
#include <common/Timer.h>

#include <SWLog.h>

class AWE_DECLARE_DATA NotifyMessage{
public:
	NotifyMessage(int what, int ext1, int ext2, const char* msg);
	~NotifyMessage();

public:
	int what = 0;
	int ext1 = 0;
	int ext2 = 0;
	std::string obj;

private:
	DISALLOW_EVIL_CONSTRUCTORS(NotifyMessage);
};

class AWE_DECLARE_DATA SWRuntime{
public:
	~SWRuntime();
	void init(int level, const char* filename);
	void deinit();

	Timer* getTimer();
	uint32_t obtainID();

	static SWRuntime* getInstance();
private:
	SWRuntime();

	static SWRuntime _instance;

	volatile uint32_t _gid;

	int inited = 0;

	Timer* mTimer = NULL;

	DISALLOW_EVIL_CONSTRUCTORS(SWRuntime);
};

#endif /* SWLOG_H_ */
