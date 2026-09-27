#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <vlc/vlc.h>
#include "ytdlp_extract.h"

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

    /* Step 2: Initialize LibVLC instance */
    /* LibVLC arguments for iOS: verbose logging, no video title, no OSD */
    const char *vlc_args[] = {
        "-v",                    /* Verbose logging */
        "--no-video-title-show", /* Don't show video title */
        "--no-osd",              /* No on-screen display */
        "--quiet",               /* Reduce log noise */
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

    /* Step 3: Create media from the direct URL */
    libvlc_media_t *media = libvlc_media_new_location(inst, direct_url);
    free(direct_url);  /* No longer needed after media creation */
    if (!media) {
        fprintf(stderr, "[main] ERROR: Failed to create media\n");
        libvlc_release(inst);
        return EXIT_FAILURE;
    }
    fprintf(stderr, "[main] Media created\n");

    /* Step 4: Create media player */
    libvlc_media_player_t *mp = libvlc_media_player_new_from_media(media);
    libvlc_media_release(media);  /* Media is retained by player */
    if (!mp) {
        fprintf(stderr, "[main] ERROR: Failed to create media player\n");
        libvlc_release(inst);
        return EXIT_FAILURE;
    }
    fprintf(stderr, "[main] Media player created\n");

    /* Step 5: Start playback */
    if (libvlc_media_player_play(mp) != 0) {
        fprintf(stderr, "[main] ERROR: Failed to start playback\n");
        libvlc_media_player_release(mp);
        libvlc_release(inst);
        return EXIT_FAILURE;
    }
    fprintf(stderr, "[main] Playback started. Press Ctrl+C to stop.\n");

    /* Step 6: Keep process alive - event loop */
    /* On iOS, we'd normally integrate with UIKit runloop.
     * For this prototype, we use a simple sleep loop with signal handling. */
    while (g_running) {
        /* Check playback state */
        libvlc_state_t state = libvlc_media_player_get_state(mp);
        if (state == libvlc_Ended || state == libvlc_Error) {
            fprintf(stderr, "[main] Playback ended or error (state: %d)\n", state);
            break;
        }

        /* Sleep briefly to avoid busy-waiting */
        sleep(1);
    }

    /* Cleanup */
    fprintf(stderr, "[main] Stopping playback...\n");
    libvlc_media_player_stop(mp);
    libvlc_media_player_release(mp);
    libvlc_release(inst);

    fprintf(stderr, "[main] iVLC exited cleanly\n");
    return EXIT_SUCCESS;
}