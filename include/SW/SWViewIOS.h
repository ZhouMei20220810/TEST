//
//  SWViewIOS.h
//
#ifndef SWVIEW_IOS_H__
#define SWVIEW_IOS_H__

#import <UIKit/UIKit.h>

typedef void touchEventHandle(int eventAction, int pointerCount, int x[], int y[], float force[], void* userdata);

@interface SWView: UIView

//@property (nonatomic, assign) int videoWidth;
//@property (nonatomic, assign) int videoHeight;

- (id) initWithFrame:(CGRect)rect;

- (void) setTouchEventHandler:(touchEventHandle*)handle userdata:(void*)userdata;

- (void) setVideoSize:(int)width height:(int)height;

- (void) render:(UIImage*)image;

@end

#endif //SWVIEW_IOS_H__
