/*
 * VLCKit/MobileVLCKit wrapper implementation for iVLC
 * Pure C implementation using objc_msgSend to call VLCKit's Objective-C API
 * No .m files needed - works with pure C99 compilation
 */

#include "vlckit_wrapper.h"
#include <objc/objc.h>
#include <objc/message.h>
#include <objc/runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* VLCKit class names */
#define VLCKIT_CLASS_MEDIA_PLAYER "VLCMediaPlayer"
#define VLCKIT_CLASS_MEDIA "VLCMedia"
#define VLCKIT_CLASS_DIALOG_PROVIDER "VLCDialogProvider"

/* Selector names */
#define SEL_INIT "init"
#define SEL_DEALLOC "dealloc"
#define SEL_MEDIA_WITH_URL "mediaWithURL:"
#define SEL_SET_MEDIA "setMedia:"
#define SEL_PLAY "play"
#define SEL_PAUSE "pause"
#define SEL_STOP "stop"
#define SEL_STATE "state"
#define SEL_SET_DRAWABLE "setDrawable:"
#define SEL_DRAWABLE "drawable"
#define SEL_SET_VIDEO_OUTPUT_MODE "setVideoOutputMode:"
#define SEL_SET_DELEGATE "setDelegate:"
#define SEL_VOLUME "volume"
#define SEL_SET_VOLUME "setVolume:"
#define SEL_MUTED "isMuted"
#define SEL_SET_MUTED "setMuted:"
#define SEL_TIME "time"
#define SEL_MEDIA_LENGTH "mediaLength"
#define SEL_POSITION "position"
#define SEL_SET_POSITION "setPosition:"
#define SEL_SET_TIME "setTime:"
#define SEL_VIDEO_SIZE "videoSize"
#define SEL_ASPECT_RATIO "aspectRatio"
#define SEL_SET_ASPECT_RATIO "setAspectRatio:"
#define SEL_NUMBER_OF_VIDEO_SUBTITLES_TRACKS "numberOfVideoSubtitlesTracks"
#define SEL_CURRENT_VIDEO_SUBTITLES_TRACK "currentVideoSubtitlesTrack"
#define SEL_SET_CURRENT_VIDEO_SUBTITLES_TRACK "setCurrentVideoSubtitlesTrack:"
#define SEL_NUMBER_OF_AUDIO_TRACKS "numberOfAudioTracks"
#define SEL_CURRENT_AUDIO_TRACK "currentAudioTrack"
#define SEL_SET_CURRENT_AUDIO_TRACK "setCurrentAudioTrack:"
#define SEL_SET_DIALOG_PROVIDER "setDialogProvider:"

/* Delegate protocol methods */
#define SEL_MEDIA_PLAYER_STATE_CHANGED "mediaPlayerStateChanged:"
#define SEL_MEDIA_PLAYER_ENCOUNTERED_ERROR "mediaPlayerEncounteredError:"

/* Static delegate instance */
static id g_vlckit_delegate = nil;
static vlckit_state_changed_cb g_state_cb = NULL;
static void *g_state_user_data = NULL;
static vlckit_error_cb g_error_cb = NULL;
static void *g_error_user_data = NULL;

/* Forward declarations */
static void vlckit_delegate_state_changed(id self, SEL _cmd, id player);
static void vlckit_delegate_encountered_error(id self, SEL _cmd, id player, id error);

/* Create delegate class at runtime */
static Class vlckit_create_delegate_class(void) {
    Class delegateClass = objc_allocateClassPair(objc_getClass("NSObject"), "VLCKitDelegate", 0);
    if (!delegateClass) return nil;
    
    /* Add state changed method */
    class_addMethod(delegateClass, sel_registerName(SEL_MEDIA_PLAYER_STATE_CHANGED),
                    (IMP)vlckit_delegate_state_changed, "v@:@");
    
    /* Add error method */
    class_addMethod(delegateClass, sel_registerName(SEL_MEDIA_PLAYER_ENCOUNTERED_ERROR),
                    (IMP)vlckit_delegate_encountered_error, "v@:@@");
    
    objc_registerClassPair(delegateClass);
    return delegateClass;
}

/* Delegate callback: state changed */
static void vlckit_delegate_state_changed(id self, SEL _cmd, id player) {
    (void)self; (void)_cmd; (void)player;
    if (g_state_cb) {
        /* Get state from player */
        id stateObj = objc_msgSend(player, sel_registerName(SEL_STATE));
        long state = (long)stateObj;
        g_state_cb((VLCKitMediaPlayerState)state, g_state_user_data);
    }
}

/* Delegate callback: error encountered */
static void vlckit_delegate_encountered_error(id self, SEL _cmd, id player, id error) {
    (void)self; (void)_cmd; (void)player;
    if (g_error_cb && error) {
        const char *errorStr = (const char *)objc_msgSend(error, sel_registerName("localizedDescription"));
        const char *utf8Str = (const char *)objc_msgSend((id)errorStr, sel_registerName("UTF8String"));
        g_error_cb(utf8Str ? utf8Str : "Unknown error", g_error_user_data);
    }
}

/* Initialize VLCKit framework */
int vlckit_init(void) {
    /* Load VLCKit framework */
    Class mediaPlayerClass = objc_getClass(VLCKIT_CLASS_MEDIA_PLAYER);
    if (!mediaPlayerClass) {
        fprintf(stderr, "[vlckit] VLCKit framework not loaded. Class %s not found.\n", VLCKIT_CLASS_MEDIA_PLAYER);
        return -1;
    }
    
    /* Create delegate class */
    Class delegateClass = vlckit_create_delegate_class();
    if (!delegateClass) {
        fprintf(stderr, "[vlckit] Failed to create delegate class\n");
        return -1;
    }
    
    g_vlckit_delegate = objc_msgSend((id)delegateClass, sel_registerName(SEL_INIT));
    if (!g_vlckit_delegate) {
        fprintf(stderr, "[vlckit] Failed to create delegate instance\n");
        return -1;
    }
    
    fprintf(stderr, "[vlckit] VLCKit initialized successfully\n");
    return 0;
}

/* Cleanup VLCKit */
void vlckit_deinit(void) {
    if (g_vlckit_delegate) {
        objc_msgSend(g_vlckit_delegate, sel_registerName(SEL_DEALLOC));
        g_vlckit_delegate = nil;
    }
    g_state_cb = NULL;
    g_state_user_data = NULL;
    g_error_cb = NULL;
    g_error_user_data = NULL;
    fprintf(stderr, "[vlckit] VLCKit deinitialized\n");
}

/* Create media player */
VLCKitMediaPlayer *vlckit_media_player_new(void) {
    Class mediaPlayerClass = objc_getClass(VLCKIT_CLASS_MEDIA_PLAYER);
    if (!mediaPlayerClass) {
        fprintf(stderr, "[vlckit] VLCMediaPlayer class not found\n");
        return NULL;
    }
    
    id player = objc_msgSend((id)mediaPlayerClass, sel_registerName("alloc"));
    player = objc_msgSend(player, sel_registerName(SEL_INIT));
    
    if (!player) {
        fprintf(stderr, "[vlckit] Failed to create VLCMediaPlayer\n");
        return NULL;
    }
    
    /* Set delegate */
    if (g_vlckit_delegate) {
        objc_msgSend(player, sel_registerName(SEL_SET_DELEGATE), g_vlckit_delegate);
    }
    
    fprintf(stderr, "[vlckit] Created VLCMediaPlayer: %p\n", player);
    return (VLCKitMediaPlayer *)player;
}

/* Release media player */
void vlckit_media_player_release(VLCKitMediaPlayer *player) {
    if (player) {
        objc_msgSend((id)player, sel_registerName(SEL_DEALLOC));
        fprintf(stderr, "[vlckit] Released VLCMediaPlayer: %p\n", player);
    }
}

/* Create media from URL */
VLCKitMedia *vlckit_media_new_with_url(VLCKitMediaPlayer *player, const char *url) {
    (void)player; /* Not needed for media creation */
    
    Class mediaClass = objc_getClass(VLCKIT_CLASS_MEDIA);
    if (!mediaClass) {
        fprintf(stderr, "[vlckit] VLCMedia class not found\n");
        return NULL;
    }
    
    /* Create NSURL from string */
    id nsString = objc_msgSend(objc_getClass("NSString"), sel_registerName("stringWithUTF8String:"), url);
    id nsURL = objc_msgSend(objc_getClass("NSURL"), sel_registerName("URLWithString:"), nsString);
    
    if (!nsURL) {
        fprintf(stderr, "[vlckit] Failed to create NSURL from: %s\n", url);
        return NULL;
    }
    
    id media = objc_msgSend((id)mediaClass, sel_registerName(SEL_MEDIA_WITH_URL), nsURL);
    
    if (!media) {
        fprintf(stderr, "[vlckit] Failed to create VLCMedia\n");
        return NULL;
    }
    
    fprintf(stderr, "[vlckit] Created VLCMedia for URL: %s\n", url);
    return (VLCKitMedia *)media;
}

/* Release media */
void vlckit_media_release(VLCKitMedia *media) {
    if (media) {
        objc_msgSend((id)media, sel_registerName(SEL_DEALLOC));
    }
}

/* Set media on player */
int vlckit_media_player_set_media(VLCKitMediaPlayer *player, VLCKitMedia *media) {
    if (!player || !media) return -1;
    
    objc_msgSend((id)player, sel_registerName(SEL_SET_MEDIA), (id)media);
    return 0;
}

/* Playback control */
int vlckit_media_player_play(VLCKitMediaPlayer *player) {
    if (!player) return -1;
    objc_msgSend((id)player, sel_registerName(SEL_PLAY));
    return 0;
}

void vlckit_media_player_pause(VLCKitMediaPlayer *player) {
    if (!player) return;
    objc_msgSend((id)player, sel_registerName(SEL_PAUSE));
}

void vlckit_media_player_stop(VLCKitMediaPlayer *player) {
    if (!player) return;
    objc_msgSend((id)player, sel_registerName(SEL_STOP));
}

/* Get state */
VLCKitMediaPlayerState vlckit_media_player_get_state(VLCKitMediaPlayer *player) {
    if (!player) return VLCKitMediaPlayerStateError;
    
    id stateObj = objc_msgSend((id)player, sel_registerName(SEL_STATE));
    return (VLCKitMediaPlayerState)(long)stateObj;
}

/* Set CALayer for video output */
int vlckit_media_player_set_calayer(VLCKitMediaPlayer *player, void *layer) {
    if (!player || !layer) return -1;
    
    /* On iOS/tvOS, VLCKit uses setDrawable: with a CALayer */
    objc_msgSend((id)player, sel_registerName(SEL_SET_DRAWABLE), (id)layer);
    fprintf(stderr, "[vlckit] Set CALayer drawable: %p\n", layer);
    return 0;
}

/* Set video output mode */
int vlckit_media_player_set_video_output_mode(VLCKitMediaPlayer *player, VLCKitVideoOutputMode mode) {
    if (!player) return -1;
    
    objc_msgSend((id)player, sel_registerName(SEL_SET_VIDEO_OUTPUT_MODE), (long)mode);
    return 0;
}

/* Get drawable */
void *vlckit_media_player_get_drawable(VLCKitMediaPlayer *player) {
    if (!player) return NULL;
    return (void *)objc_msgSend((id)player, sel_registerName(SEL_DRAWABLE));
}

/* Set state callback */
void vlckit_media_player_set_state_callback(VLCKitMediaPlayer *player, vlckit_state_changed_cb cb, void *user_data) {
    (void)player; /* Delegate is global */
    g_state_cb = cb;
    g_state_user_data = user_data;
}

/* Set error callback */
void vlckit_media_player_set_error_callback(VLCKitMediaPlayer *player, vlckit_error_cb cb, void *user_data) {
    (void)player;
    g_error_cb = cb;
    g_error_user_data = user_data;
}

/* Audio control */
void vlckit_media_player_set_volume(VLCKitMediaPlayer *player, float volume) {
    if (!player) return;
    objc_msgSend((id)player, sel_registerName(SEL_SET_VOLUME), volume);
}

float vlckit_media_player_get_volume(VLCKitMediaPlayer *player) {
    if (!player) return 0.0f;
    return (float)objc_msgSend((id)player, sel_registerName(SEL_VOLUME));
}

void vlckit_media_player_set_mute(VLCKitMediaPlayer *player, int mute) {
    if (!player) return;
    objc_msgSend((id)player, sel_registerName(SEL_SET_MUTED), mute);
}

int vlckit_media_player_get_mute(VLCKitMediaPlayer *player) {
    if (!player) return 0;
    return (int)(long)objc_msgSend((id)player, sel_registerName(SEL_MUTED));
}

/* Time/position */
long long vlckit_media_player_get_time(VLCKitMediaPlayer *player) {
    if (!player) return -1;
    return (long long)objc_msgSend((id)player, sel_registerName(SEL_TIME));
}

long long vlckit_media_player_get_length(VLCKitMediaPlayer *player) {
    if (!player) return -1;
    return (long long)objc_msgSend((id)player, sel_registerName(SEL_MEDIA_LENGTH));
}

float vlckit_media_player_get_position(VLCKitMediaPlayer *player) {
    if (!player) return 0.0f;
    return (float)objc_msgSend((id)player, sel_registerName(SEL_POSITION));
}

int vlckit_media_player_set_position(VLCKitMediaPlayer *player, float position) {
    if (!player) return -1;
    objc_msgSend((id)player, sel_registerName(SEL_SET_POSITION), position);
    return 0;
}

int vlckit_media_player_set_time(VLCKitMediaPlayer *player, long long time) {
    if (!player) return -1;
    objc_msgSend((id)player, sel_registerName(SEL_SET_TIME), time);
    return 0;
}

/* Video size */
int vlckit_media_player_get_video_width(VLCKitMediaPlayer *player) {
    if (!player) return 0;
    id size = objc_msgSend((id)player, sel_registerName(SEL_VIDEO_SIZE));
    if (!size) return 0;
    return (int)objc_msgSend(size, sel_registerName("width"));
}

int vlckit_media_player_get_video_height(VLCKitMediaPlayer *player) {
    if (!player) return 0;
    id size = objc_msgSend((id)player, sel_registerName(SEL_VIDEO_SIZE));
    if (!size) return 0;
    return (int)objc_msgSend(size, sel_registerName("height"));
}

/* Aspect ratio */
void vlckit_media_player_set_aspect_ratio(VLCKitMediaPlayer *player, const char *ratio) {
    if (!player || !ratio) return;
    id nsString = objc_msgSend(objc_getClass("NSString"), sel_registerName("stringWithUTF8String:"), ratio);
    objc_msgSend((id)player, sel_registerName(SEL_SET_ASPECT_RATIO), nsString);
}

const char *vlckit_media_player_get_aspect_ratio(VLCKitMediaPlayer *player) {
    if (!player) return NULL;
    id ratio = objc_msgSend((id)player, sel_registerName(SEL_ASPECT_RATIO));
    if (!ratio) return NULL;
    return (const char *)objc_msgSend(ratio, sel_registerName("UTF8String"));
}

/* Subtitle tracks */
int vlckit_media_player_get_spu_count(VLCKitMediaPlayer *player) {
    if (!player) return 0;
    return (int)(long)objc_msgSend((id)player, sel_registerName(SEL_NUMBER_OF_VIDEO_SUBTITLES_TRACKS));
}

int vlckit_media_player_get_spu(VLCKitMediaPlayer *player) {
    if (!player) return -1;
    return (int)(long)objc_msgSend((id)player, sel_registerName(SEL_CURRENT_VIDEO_SUBTITLES_TRACK));
}

int vlckit_media_player_set_spu(VLCKitMediaPlayer *player, int track) {
    if (!player) return -1;
    objc_msgSend((id)player, sel_registerName(SEL_SET_CURRENT_VIDEO_SUBTITLES_TRACK), (long)track);
    return 0;
}

/* Audio tracks */
int vlckit_media_player_get_audio_track_count(VLCKitMediaPlayer *player) {
    if (!player) return 0;
    return (int)(long)objc_msgSend((id)player, sel_registerName(SEL_NUMBER_OF_AUDIO_TRACKS));
}

int vlckit_media_player_get_audio_track(VLCKitMediaPlayer *player) {
    if (!player) return -1;
    return (int)(long)objc_msgSend((id)player, sel_registerName(SEL_CURRENT_AUDIO_TRACK));
}

int vlckit_media_player_set_audio_track(VLCKitMediaPlayer *player, int track) {
    if (!player) return -1;
    objc_msgSend((id)player, sel_registerName(SEL_SET_CURRENT_AUDIO_TRACK), (long)track);
    return 0;
}

/* Dialog provider */
void vlckit_media_player_set_dialog_provider(VLCKitMediaPlayer *player, VLCKitDialogProvider *provider) {
    if (!player) return;
    objc_msgSend((id)player, sel_registerName(SEL_SET_DIALOG_PROVIDER), (id)provider);
}