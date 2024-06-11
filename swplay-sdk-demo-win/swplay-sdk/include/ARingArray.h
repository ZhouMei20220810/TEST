/*
 * ARingArray.h
 *
 */

#ifndef ARINGARRAY_H_
#define ARINGARRAY_H_

#include <common/common.h>
#include <common/Condition.h>
#include <common/Utils.h>
#include <common/Timers.h>

#define DEFAULT_TIMED_WAIT 3000 //3 secs

template<class T>
class AWE_DECLARE_DATA ARingArrayFreeLock{
public:
	ARingArrayFreeLock(size_t capcaity)
		:mArray(new T[capcaity]),
		 mCapacity(capcaity),
		 rindex(0),
		 windex(0),
		 nmemb(0){
		memset(mArray, 0, sizeof(T) * capcaity);
	}
	~ARingArrayFreeLock(){
		delete[] mArray;
	}
	T* get(int index){
		if(index < 0 || index >= (int)mCapacity) return NULL;
		return &(mArray[index]);
	}
	T* writeReady(int *wresult){
		if(nmemb >= mCapacity){
			*wresult = 0;
			return NULL;
		}
		*wresult = 1;
		return &(mArray[windex]);
	}
	void writeFinish(int wresult){
		if(wresult <= 0){
			return;
		}
		uint32_t index = windex;
		windex = ++index % mCapacity;
		nmemb++;
	}
	T* readReady(int *rresult){
		if(nmemb <= 0){
			*rresult = 0;
			return NULL;
		}
		*rresult = 1;
		return &(mArray[rindex]);
	}
	void readFinish(int rresult){
		if(rresult <= 0){
			return;
		}
		uint32_t index = rindex;

		rindex = ++index % mCapacity;
		nmemb--;
	}
//	void clear();
//	void abort(){
//		Mutex::Autolock autoLock(mLock);
//		abort_request = 1;
//		mCondition.signal();
//	}

	size_t capacity(){return mCapacity;}
	size_t num(){return nmemb;}
private:
	T* mArray;
	size_t mCapacity;

	uint32_t rindex;
	uint32_t windex;

	uint32_t nmemb;

	DISALLOW_EVIL_CONSTRUCTORS(ARingArrayFreeLock);
};


template<class T>
class AWE_DECLARE_DATA ARingArray {
public:
	ARingArray(size_t size, int32_t ms)
		:mArray(new T[size]),
		 timedwait(0),
		 mCapacity(size),
		 rindex(0),
		 windex(0),
		 nmemb(0),
		 abort_request(0){
		memset(mArray, 0, sizeof(T) * size);
		if(ms <= 0){
			ms = DEFAULT_TIMED_WAIT;
		}
		timedwait = milliseconds_to_nanoseconds(ms);
	}

	~ARingArray(){
		abort();
		delete[] mArray;
	}

	T* get(int index){
		if(index < 0 || index >= (int)mCapacity) return NULL;
		return &(mArray[index]);
	}
	T* writeReady(int *wresult, bool block){
		Mutex::Autolock autoLock(mLock);
		if(block && !abort_request && nmemb >= mCapacity){
			mCondition.waitRelative(mLock, timedwait);
		}
		if(abort_request){
			*wresult = -1;
			return NULL;
		}
		if(nmemb >= mCapacity){
			*wresult = 0;
			return NULL;
		}
		*wresult = 1;
		return &(mArray[windex]);
	}
	void writeFinish(int wresult){
		if(wresult <= 0){
			return;
		}
		size_t size = mCapacity;
		uint32_t index = windex;

		Mutex::Autolock autoLock(mLock);
		windex = ++index % size;
		nmemb++;
		mCondition.signal();
	}
	T* readReady(int *rresult, bool block){
		Mutex::Autolock autoLock(mLock);
		if(block && !abort_request && nmemb <= 0){
			mCondition.waitRelative(mLock, timedwait);
		}
		if(abort_request){
			*rresult = -1;
			return NULL;
		}
		if(nmemb <= 0){
			*rresult = 0;
			return NULL;
		}
		*rresult = 1;
		return &(mArray[rindex]);
	}
	void readFinish(int rresult){
		if(rresult <= 0){
			return;
		}
		size_t size = mCapacity;
		uint32_t index = rindex;

		Mutex::Autolock autoLock(mLock);
		rindex = ++index % size;
		nmemb--;
		mCondition.signal();
	}
	void abort(){
		Mutex::Autolock autoLock(mLock);
		abort_request = 1;
		mCondition.signal();
	}

	bool waitForAbort(int waitMs){
		Mutex::Autolock autoLock(mLock);
		mCondition.waitRelative(mLock, milliseconds_to_nanoseconds(waitMs));
		return (abort_request == 1);
	}

	size_t size(){return mCapacity;}

	size_t num(){return nmemb;}

private:
	Condition mCondition;
	Mutex mLock;

	T* mArray;

	int64_t timedwait;

	size_t mCapacity;
	uint32_t rindex;
	uint32_t windex;
	uint32_t nmemb;
	int32_t abort_request;

	DISALLOW_EVIL_CONSTRUCTORS(ARingArray);
};

#endif /* ARINGARRAY_H_ */
