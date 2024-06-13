/*
 * APacket.h
 *
 */

#ifndef APACKET_H_
#define APACKET_H_

#include <stdint.h>

#include <common/common.h>
#include <common/Utils.h>

class AWE_DECLARE_DATA APacket{
public:
    APacket(int32_t capacity);
    APacket(void *data, int32_t capacity);

    ~APacket();

    uint8_t *base() { return (uint8_t *)mData; }
    uint8_t *data() { return (uint8_t *)mData + mRangeOffset; }
    int32_t capacity() const { return mCapacity; }
    int32_t size() const { return mRangeLength; }
    int32_t offset() const { return mRangeOffset; }

    uint16_t getSeq() { return mSeq; }
    void setSeq(uint16_t seq) { mSeq = seq;}

    uint32_t getDts(){ return mDts; }
    void setDts(uint32_t dts) { mDts = dts; }

//    int64_t getPts(){ return mPts; }
//    void setPts(int64_t pts){ mPts = pts; }

    void setRange(int32_t offset, int32_t size);

    void unFreeData(bool b);
private:
    void* mData;
    int32_t mCapacity;
    int32_t mRangeOffset;
    int32_t mRangeLength;

    uint32_t mDts;
//    int64_t mPts;

    uint16_t mSeq;

    bool mOwnsData;

    DISALLOW_EVIL_CONSTRUCTORS(APacket);
};

typedef struct MediaPacket{
    uint32_t length;
    uint32_t type;    //0:audio, 1:I frame, 2:P frame, 7:sps, 8:pps, 100:fps
    int64_t timestamp;
}MediaPacket;

#endif /* APACKET_H_ */
