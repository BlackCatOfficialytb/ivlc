/*
 * VLCKit/MobileVLCKit wrapper header for iVLC
 * Provides a unified C API over VLCKit's Objective-C interface
 * Supports both VLCKit (macOS) and MobileVLCKit (iOS/tvOS)
 */

#ifndef VLCKIT_WRAPPER_H
#define VLCKIT_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Opaque types mirroring VLCKit classes */
typedef struct VLCKitMediaPlayer VLCKitMediaPlayer;
typedef struct VLCKitMedia VLCKitMedia;
typedef struct VLCKitDialogProvider VLCKitDialogProvider;

/* VLCKit media player state (matches VLCMediaPlayerState) */
typedef enum {
    VLCKitMediaPlayerStateStopped = 0,
    VLCKitMediaPlayerStatePlaying,
    VLCKitMediaPlayerStatePaused,
    VLCKitMediaPlayerStateBuffering,
    VLCKitMediaPlayerStateEnded,
    VLCKitMediaPlayerStateError
} VLCKitMediaPlayerState;

/* Video output mode */
typedef enum {
    VLCKitVideoOutputModeDefault = 0,
    VLCKitVideoOutputModeOpenGLES,
    VLCKitVideoOutputModeMetal,
    VLCKitVideoOutputModeCALayer
} VLCKitVideoOutputMode;

/* Initialize VLCKit (loads framework, sets up audio session) */
int vlckit_init(void);

/* Cleanup VLCKit */
void vlckit_deinit(void);

/* Create media player instance */
VLCKitMediaPlayer *vlckit_media_player_new(void);

/* Release media player */
void vlckit_media_player_release(VLCKitMediaPlayer *player);

/* Create media from URL */
VLCKitMedia *vlckit_media_new_with_url(VLCKitMediaPlayer *player, const char *url);

/* Release media */
void vlckit_media_release(VLCKitMedia *media);

/* Set media on player */
int vlckit_media_player_set_media(VLCKitMediaPlayer *player, VLCKitMedia *media);

/* Playback control */
int vlckit_media_player_play(VLCKitMediaPlayer *player);
void vlckit_media_player_pause(VLCKitMediaPlayer *player);
void vlckit_media_player_stop(VLCKitMediaPlayer *player);

/* Get current state */
VLCKitMediaPlayerState vlckit_media_player_get_state(VLCKitMediaPlayer *player);

/* Video output - bind to CALayer (iOS/tvOS) */
int vlckit_media_player_set_calayer(VLCKitMediaPlayer *player, void *layer);

/* Video output - set video output mode */
int vlckit_media_player_set_video_output_mode(VLCKitMediaPlayer *player, VLCKitVideoOutputMode mode);

/* Get drawable (CALayer) for custom rendering */
void *vlckit_media_player_get_drawable(VLCKitMediaPlayer *player);

/* Set delegate for events (optional) */
typedef void (*vlckit_state_changed_cb)(VLCKitMediaPlayerState state, void *user_data);
typedef void (*vlckit_error_cb)(const char *error, void *user_data);

void vlckit_media_player_set_state_callback(VLCKitMediaPlayer *player, vlckit_state_changed_cb cb, void *user_data);
void vlckit_media_player_set_error_callback(VLCKitMediaPlayer *player, vlckit_error_cb cb, void *user_data);

/* Audio control */
void vlckit_media_player_set_volume(VLCKitMediaPlayer *player, float volume);
float vlckit_media_player_get_volume(VLCKitMediaPlayer *player);
void vlckit_media_player_set_mute(VLCKitMediaPlayer *player, int mute);
int vlckit_media_player_get_mute(VLCKitMediaPlayer *player);

/* Time/position control */
long long vlckit_media_player_get_time(VLCKitMediaPlayer *player);
long long vlckit_media_player_get_length(VLCKitMediaPlayer *player);
float vlckit_media_player_get_position(VLCKitMediaPlayer *player);
int vlckit_media_player_set_position(VLCKitMediaPlayer *player, float position);
int vlckit_media_player_set_time(VLCKitMediaPlayer *player, long long time);

/* Video size */
int vlckit_media_player_get_video_width(VLCKitMediaPlayer *player);
int vlckit_media_player_get_video_height(VLCKitMediaPlayer *player);

/* Aspect ratio */
void vlckit_media_player_set_aspect_ratio(VLCKitMediaPlayer *player, const char *ratio);
const char *vlckit_media_player_get_aspect_ratio(VLCKitMediaPlayer *player);

/* Subtitle/audio track selection */
int vlckit_media_player_get_spu_count(VLCKitMediaPlayer *player);
int vlckit_media_player_get_spu(VLCKitMediaPlayer *player);
int vlckit_media_player_set_spu(VLCKitMediaPlayer *player, int track);
int vlckit_media_player_get_audio_track_count(VLCKitMediaPlayer *player);
int vlckit_media_player_get_audio_track(VLCKitMediaPlayer *player);
int vlckit_media_player_set_audio_track(VLCKitMediaPlayer *player, int track);

/* Dialog provider (for authentication, etc.) */
void vlckit_media_player_set_dialog_provider(VLCKitMediaPlayer *player, VLCKitDialogProvider *provider);

#ifdef __cplusplus
}
#endif

#endif /* VLCKIT_WRAPPER_H */