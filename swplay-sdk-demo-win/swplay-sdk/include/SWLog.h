/*
 * SWLog.h
 */

#ifndef SWLOG_H_
#define SWLOG_H_

#include <common/common.h>
#include <common/Utils.h>

class AWE_DECLARE_DATA SWLog{
public:
	~SWLog();
	void init(int level, const char* filename);
	static SWLog* getInstance();

	static void write();
private:
	static SWLog _instance;
	SWLog();
	DISALLOW_EVIL_CONSTRUCTORS(SWLog);
};

#endif /* SWLOG_H_ */
