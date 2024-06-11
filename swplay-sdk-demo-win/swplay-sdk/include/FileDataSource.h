/*
 * FileDataSource.h
 *
 */

#ifndef FILEDATASOURCE_H_
#define FILEDATASOURCE_H_

#include <string.h>

#include <list>

#include <common/Utils.h>
#include <common/Condition.h>
#include <DataSource.h>
#include <SWDataSourceListener.h>
#include <VideoDisplay.h>

class APacket;
class PlayDataSource;

class AWE_DECLARE_DATA FileDataSource: public DataSource{
public:
	FileDataSource(uint32_t id);
	~FileDataSource();

	int setPlayParams(const char* filename, int fps, int width, int height);

	int start() override;
	void stop() override;

public:
	uint32_t getId() override;
	void setId(uint32_t id) override;

	bool attach(uint32_t id) override;
	bool detach(uint32_t id) override;

    int getAudioFrameCount() override;
    int popAudioFrame(std::shared_ptr<APacket>* buffer, int waitMs) override;
    int getVideoFrameCount() override;
    int popVideoFrame(std::shared_ptr<APacket>* buffer, int waitMs) override;
private:
    static void onStartHandle(void *userdata, intptr_t arg);
    void onStart();
    
	Mutex mMutex;

	FILE* mAVfile = NULL;

	uint32_t mPlayerID = 0;

	uint32_t mId = 0;
	bool mStarted = false;
	DISALLOW_EVIL_CONSTRUCTORS(FileDataSource);
};

#endif /* FILEDATASOURCE_H_ */
