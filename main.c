#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "ytdlp_extract.h"

/* Objective-C runtime for CALayer binding (pure C, no .m files) */
#include <objc/objc.h>
#include <objc/message.h>
#include <objc/runtime.h>
#include <QuartzCore/QuartzCore.h>

/* Backend selection via Makefile: USE_LIBVLC or USE_VLCKIT */
#if defined(USE_VLCKIT)
#include "vlckit/vlckit_wrapper.h"
#elif defined(USE_LIBVLC)
#include <vlc/vlc.h>
#else
#error "No backend selected. Define USE_LIBVLC or USE_VLCKIT"
#endif

/* Global flag for graceful shutdown */
static volatile sig_atomic_t g_running = 1;

/* Signal handler for clean exit */
static void signal_handler(int sig) {
    (void)sig;
    fprintf(stderr, "\n[main] Received signal, shutting down...\n");
    g_running = 0;
}

/* Print usage information */
static void print_usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s <youtube_url>\n", prog_name);
    fprintf(stderr, "Example: %s \"https://www.youtube.com/watch?v=dQw4w9WgXcQ\"\n", prog_name);
}

/* Create a minimal CALayer for video rendering (pure C) */
static void *create_video_layer(void) {
    Class CALayerClass = objc_getClass("CALayer");
    if (!CALayerClass) {
        fprintf(stderr, "[main] CALayer class not found\n");
        return NULL;
    }
    
    id layer = objc_msgSend((id)CALayerClass, sel_registerName("layer"));
    if (!layer) {
        fprintf(stderr, "[main] Failed to create CALayer\n");
        return NULL;
    }
    
    /* Set layer properties for video */
    objc_msgSend(layer, sel_registerName("setOpaque:"), YES);
    objc_msgSend(layer, sel_registerName("setBackgroundColor:"), 
                 objc_msgSend(objc_getClass("UIColor"), sel_registerName("blackColor"), sel_registerName("CGColor")));
    
    fprintf(stderr, "[main] Created CALayer: %p\n", layer);
    return layer;
}

/* State change callback for VLCKit */
#if defined(USE_VLCKIT)
static void vlckit_state_callback(VLCKitMediaPlayerState state, void *user_data) {
    (void)user_data;
    fprintf(stderr, "[vlckit] State changed: %d\n", state);
    if (state == VLCKitMediaPlayerStateEnded || state == VLCKitMediaPlayerStateError) {
        g_running = 0;
    }
}

static void vlckit_error_callback(const char *error, void *user_data) {
    (void)user_data;
    fprintf(stderr, "[vlckit] Error: %s\n", error);
    g_running = 0;
}
#endif

int main(int argc, char *argv[]) {
    /* Default test URL (Rick Astley - Never Gonna Give You Up) */
    const char *default_url = "https://www.youtube.com/watch?v=dQw4w9WgXcQ";
    const char *youtube_url = default_url;

    /* Parse command line argument */
    if (argc > 1) {
        youtube_url = argv[1];
    } else {
        fprintf(stderr, "[main] No URL provided, using default: %s\n", default_url);
    }

    /* Set up signal handlers for graceful shutdown */
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    fprintf(stderr, "[main] iVLC starting - YouTube URL: %s\n", youtube_url);

    /* Step 1: Extract direct stream URL using yt-dlp */
    fprintf(stderr, "[main] Extracting direct stream URL...\n");
    char *direct_url = extract_stream_url(youtube_url);
    if (!direct_url) {
        fprintf(stderr, "[main] ERROR: Failed to extract stream URL\n");
        return EXIT_FAILURE;
    }

    fprintf(stderr, "[main] Direct URL: %s\n", direct_url);

    /* Step 2: Initialize backend */
#if defined(USE_LIBVLC)
    /* LibVLC backend */
    const char *vlc_args[] = {
        "-v",
        "--no-video-title-show",
        "--no-osd",
        "--quiet",
        NULL
    };
    int vlc_argc = sizeof(vlc_args) / sizeof(vlc_args[0]) - 1;

    libvlc_instance_t *inst = libvlc_new(vlc_argc, vlc_args);
    if (!inst) {
        fprintf(stderr, "[main] ERROR: Failed to create LibVLC instance\n");
        free(direct_url);
        return EXIT_FAILURE;
    }
    fprintf(stderr, "[main] LibVLC instance created\n");

    libvlc_media_t *media = libvlc_media_new_location(inst, direct_url);
    free(direct_url);
    if (!media) {
        fprintf(stderr, "[main] ERROR: Failed to create media\n");
        libvlc_release(inst);
        return EXIT_FAILURE;
    }
    fprintf(stderr, "[main] Media created\n");

    libvlc_media_player_t *mp = libvlc_media_player_new_from_media(media);
    libvlc_media_release(media);
    if (!mp) {
        fprintf(stderr, "[main] ERROR: Failed to create media player\n");
        libvlc_release(inst);
        return EXIT_FAILURE;
    }
    fprintf(stderr, "[main] Media player created\n");

    void *video_layer = create_video_layer();
    if (video_layer) {
        libvlc_video_set_callbacks(mp, NULL, NULL, NULL, video_layer);
        libvlc_video_set_format(mp, "RV32", 0, 0, 0);
        fprintf(stderr, "[main] Bound LibVLC video output to CALayer: %p\n", video_layer);
    } else {
        fprintf(stderr, "[main] WARNING: Could not create CALayer, using default vout\n");
    }

    if (libvlc_media_player_play(mp) != 0) {
        fprintf(stderr, "[main] ERROR: Failed to start playback\n");
        libvlc_media_player_release(mp);
        libvlc_release(inst);
        return EXIT_FAILURE;
    }
    fprintf(stderr, "[main] Playback started. Press Ctrl+C to stop.\n");

    while (g_running) {
        libvlc_state_t state = libvlc_media_player_get_state(mp);
        if (state == libvlc_Ended || state == libvlc_Error) {
            fprintf(stderr, "[main] Playback ended or error (state: %d)\n", state);
            break;
        }
        sleep(1);
    }

    fprintf(stderr, "[main] Stopping playback...\n");
    libvlc_media_player_stop(mp);
    libvlc_media_player_release(mp);
    libvlc_release(inst);

#elif defined(USE_VLCKIT)
    /* VLCKit/MobileVLCKit backend */
    if (vlckit_init() != 0) {
        fprintf(stderr, "[main] ERROR: Failed to initialize VLCKit\n");
        free(direct_url);
        return EXIT_FAILURE;
    }
    fprintf(stderr, "[main] VLCKit initialized\n");

    VLCKitMediaPlayer *player = vlckit_media_player_new();
    if (!player) {
        fprintf(stderr, "[main] ERROR: Failed to create VLCKit media player\n");
        vlckit_deinit();
        free(direct_url);
        return EXIT_FAILURE;
    }
    fprintf(stderr, "[main] VLCKit media player created\n");

    VLCKitMedia *media = vlckit_media_new_with_url(player, direct_url);
    free(direct_url);
    if (!media) {
        fprintf(stderr, "[main] ERROR: Failed to create VLCKit media\n");
        vlckit_media_player_release(player);
        vlckit_deinit();
        return EXIT_FAILURE;
    }
    fprintf(stderr, "[main] VLCKit media created\n");

    if (vlckit_media_player_set_media(player, media) != 0) {
        fprintf(stderr, "[main] ERROR: Failed to set media on player\n");
        vlckit_media_release(media);
        vlckit_media_player_release(player);
        vlckit_deinit();
        return EXIT_FAILURE;
    }
    vlckit_media_release(media);

    void *video_layer = create_video_layer();
    if (video_layer) {
        vlckit_media_player_set_calayer(player, video_layer);
        vlckit_media_player_set_video_output_mode(player, VLCKitVideoOutputModeCALayer);
    } else {
        fprintf(stderr, "[main] WARNING: Could not create CALayer\n");
    }

    vlckit_media_player_set_state_callback(player, vlckit_state_callback, NULL);
    vlckit_media_player_set_error_callback(player, vlckit_error_callback, NULL);

    if (vlckit_media_player_play(player) != 0) {
        fprintf(stderr, "[main] ERROR: Failed to start playback\n");
        vlckit_media_player_release(player);
        vlckit_deinit();
        return EXIT_FAILURE;
    }
    fprintf(stderr, "[main] Playback started. Press Ctrl+C to stop.\n");

    while (g_running) {
        sleep(1);
    }

    fprintf(stderr, "[main] Stopping playback...\n");
    vlckit_media_player_stop(player);
    vlckit_media_player_release(player);
    vlckit_deinit();
#endif

    fprintf(stderr, "[main] iVLC exited cleanly\n");
    return EXIT_SUCCESS;
}