/*
 * VideoFrame.h
 *
 */

#ifndef VIDEOFRAME_H_
#define VIDEOFRAME_H_

struct AVFrame;

typedef union FrameData {
	int8_t* buffer;
	AVFrame* avframe;
}FrameData;

struct VideoFrame {
	FrameData frame;

	AVFrame* swframe;
	AVFrame* hwframe;

	int64_t pts;
	int16_t allocated;
	int16_t iframe;
	int dectime;
};

#endif /* VIDEOFRAME_H_ */
