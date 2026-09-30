/*
 * VLCKit framework stub for IntelliSense / cross-compilation reference (macOS)
 * Actual framework provides the real implementation at link time
 */

#ifndef VLCKIT_H
#define VLCKIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Foundation/Foundation.h>
#include <AppKit/AppKit.h>
#include <QuartzCore/QuartzCore.h>

/* VLCMediaPlayer state */
typedef NS_ENUM(NSInteger, VLCMediaPlayerState) {
    VLCMediaPlayerStateStopped = 0,
    VLCMediaPlayerStatePlaying,
    VLCMediaPlayerStatePaused,
    VLCMediaPlayerStateBuffering,
    VLCMediaPlayerStateEnded,
    VLCMediaPlayerStateError
};

/* Video output mode */
typedef NS_ENUM(NSInteger, VLCVideoOutputMode) {
    VLCVideoOutputModeDefault = 0,
    VLCVideoOutputModeOpenGL,
    VLCVideoOutputModeMetal,
    VLCVideoOutputModeCALayer
};

/* VLCMediaPlayerDelegate protocol */
@protocol VLCMediaPlayerDelegate <NSObject>
@optional
- (void)mediaPlayerStateChanged:(id)mediaPlayer;
- (void)mediaPlayerEncounteredError:(id)mediaPlayer error:(NSError *)error;
- (void)mediaPlayerTimeChanged:(id)mediaPlayer;
- (void)mediaPlayerSnapshot:(id)mediaPlayer image:(NSImage *)image;
@end

/* VLCMedia class */
@interface VLCMedia : NSObject
- (instancetype)initWithURL:(NSURL *)url;
+ (instancetype)mediaWithURL:(NSURL *)url;
@end

/* VLCMediaPlayer class */
@interface VLCMediaPlayer : NSObject
- (instancetype)init;
- (void)setMedia:(VLCMedia *)media;
- (void)play;
- (void)pause;
- (void)stop;
- (VLCMediaPlayerState)state;
- (void)setDrawable:(id)drawable;  // NSView on macOS
- (id)drawable;
- (void)setVideoOutputMode:(VLCVideoOutputMode)mode;
- (void)setDelegate:(id<VLCMediaPlayerDelegate>)delegate;
- (float)volume;
- (void)setVolume:(float)volume;
- (BOOL)isMuted;
- (void)setMuted:(BOOL)muted;
- (long long)time;
- (long long)mediaLength;
- (float)position;
- (void)setPosition:(float)position;
- (void)setTime:(long long)time;
- (NSSize)videoSize;
- (NSString *)aspectRatio;
- (void)setAspectRatio:(NSString *)ratio;
- (NSInteger)numberOfVideoSubtitlesTracks;
- (NSInteger)currentVideoSubtitlesTrack;
- (void)setCurrentVideoSubtitlesTrack:(NSInteger)track;
- (NSInteger)numberOfAudioTracks;
- (NSInteger)currentAudioTrack;
- (void)setCurrentAudioTrack:(NSInteger)track;
- (void)setDialogProvider:(id)provider;
@end

/* VLCDialogProvider protocol */
@protocol VLCDialogProvider <NSObject>
@optional
- (void)displayLoginDialog:(NSString *)title message:(NSString *)message username:(NSString **)username password:(NSString **)password;
- (void)displayErrorDialog:(NSString *)title message:(NSString *)message;
- (void)displayProgressDialog:(NSString *)title message:(NSString *)message indeterminate:(BOOL)indeterminate position:(float)position cancelCallback:(void (^)(void))cancelCallback;
@end

#ifdef __cplusplus
}
#endif

#endif /* VLCKIT_H */