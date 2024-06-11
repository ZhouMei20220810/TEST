/*
 * SWCommon.h
 *
 */

#ifndef SW_COMMON_H_
#define SW_COMMON_H_

enum VIDEO_QUALITY
{
	VIDEO_QUALITY_HD = 1,
	VIDEO_QUALITY_NORMAL,
	VIDEO_QUALITY_FAST,
	VIDEO_QUALITY_ULTRA_FAST,
};

struct VideoLevel{
    int encodetype = 2;
    int width = 432;
    int height = 768;
    int maxfps = 20;
    int minfps = 15;
    int bitrate = 1024;
    int gop = 45;
    int resolutionLevel = 3;
    int videoQuality = 3;
    int maxDelay = 0;
    int minDelay = 0;
};

#define MAX_VIDEO_LEVELS 5

#endif /* SW_COMMON_H_ */
