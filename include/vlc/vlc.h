/*
 * Minimal LibVLC header stub for IntelliSense / compilation reference.
 * Actual libvlc.a / libvlccore.a provide the real implementation at link time.
 * This mirrors the public API subset used in main.c.
 */

#ifndef VLC_VLC_H
#define VLC_VLC_H

#ifdef __cplusplus
extern "C" {
#endif

/* LibVLC version */
#define LIBVLC_VERSION "3.0.0"
#define LIBVLC_VERSION_INT 0x03000000

/* Error handling */
typedef struct libvlc_instance_t libvlc_instance_t;
typedef struct libvlc_media_t libvlc_media_t;
typedef struct libvlc_media_player_t libvlc_media_player_t;

/* Instance creation / destruction */
libvlc_instance_t *libvlc_new(int argc, const char *const *argv);
void libvlc_release(libvlc_instance_t *p_instance);

/* Media creation / destruction */
libvlc_media_t *libvlc_media_new_location(libvlc_instance_t *p_instance, const char *psz_mrl);
void libvlc_media_release(libvlc_media_t *p_md);

/* Media player creation / destruction / control */
libvlc_media_player_t *libvlc_media_player_new_from_media(libvlc_media_t *p_md);
void libvlc_media_player_release(libvlc_media_player_t *p_mi);
int libvlc_media_player_play(libvlc_media_player_t *p_mi);
void libvlc_media_player_stop(libvlc_media_player_t *p_mi);

/* Media player state */
typedef enum libvlc_state_t {
    libvlc_NothingSpecial = 0,
    libvlc_Opening,
    libvlc_Buffering,
    libvlc_Playing,
    libvlc_Paused,
    libvlc_Stopped,
    libvlc_Ended,
    libvlc_Error
} libvlc_state_t;

libvlc_state_t libvlc_media_player_get_state(libvlc_media_player_t *p_mi);

/* Event manager (optional, for advanced use) */
typedef struct libvlc_event_manager_t libvlc_event_manager_t;
libvlc_event_manager_t *libvlc_media_player_event_manager(libvlc_media_player_t *p_mi);

#ifdef __cplusplus
}
#endif

#endif /* VLC_VLC_H */