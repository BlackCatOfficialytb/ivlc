#include "ytdlp_extract.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <spawn.h>
#include <sys/wait.h>
#include <errno.h>

extern char **environ;

/**
 * Execute yt-dlp and capture stdout to get direct stream URL
 * 
 * Uses posix_spawn for efficient process creation on iOS.
 * Tries /var/jb/usr/bin/yt-dlp first (Procursus), falls back to /usr/bin/yt-dlp.
 * 
 * @param youtube_url YouTube video URL
 * @return Allocated string with direct URL, or NULL on failure
 */
char *extract_stream_url(const char *youtube_url) {
    if (!youtube_url) {
        fprintf(stderr, "[ytdlp] Error: NULL youtube_url provided\n");
        return NULL;
    }

    // yt-dlp binary paths to try (Procursus APT default first)
    const char *yt_dlp_paths[] = {
        "/var/jb/usr/bin/yt-dlp",
        "/usr/bin/yt-dlp",
        NULL
    };

    // Command: yt-dlp -g -f "bv+ba/b" "<url>"
    // -g: get URL only (don't download)
    // -f "bv+ba/b": prefer pre-muxed (bv+ba = best video+audio merged, fallback to best single)
    // This avoids separate video/audio URLs for 1080p+ by preferring merged formats
    const char *args[] = { "yt-dlp", "-g", "-f", "bv+ba/b", youtube_url, NULL };

    pid_t pid;
    int pipefd[2];
    char *result = NULL;
    size_t result_size = 0;
    size_t result_len = 0;

    // Try each yt-dlp path
    for (int i = 0; yt_dlp_paths[i]; i++) {
        const char *yt_dlp_path = yt_dlp_paths[i];

        // Check if binary exists
        if (access(yt_dlp_path, X_OK) != 0) {
            fprintf(stderr, "[ytdlp] Binary not found or not executable: %s\n", yt_dlp_path);
            continue;
        }

        fprintf(stderr, "[ytdlp] Using: %s\n", yt_dlp_path);

        // Create pipe for capturing stdout
        if (pipe(pipefd) == -1) {
            fprintf(stderr, "[ytdlp] pipe() failed: %s\n", strerror(errno));
            continue;
        }

        // Prepare posix_spawn attributes
        posix_spawn_file_actions_t actions;
        posix_spawn_file_actions_init(&actions);
        
        // Redirect stdout to pipe write end
        posix_spawn_file_actions_adddup2(&actions, pipefd[1], STDOUT_FILENO);
        // Redirect stderr to /dev/null (or keep it for debugging)
        posix_spawn_file_actions_addopen(&actions, STDERR_FILENO, "/dev/null", O_WRONLY, 0);
        // Close unused pipe read end in child
        posix_spawn_file_actions_addclose(&actions, pipefd[0]);

        // Spawn the process
        int ret = posix_spawn(&pid, yt_dlp_path, &actions, NULL, (char *const *)args, environ);
        posix_spawn_file_actions_destroy(&actions);

        // Close write end in parent (we only read)
        close(pipefd[1]);
        pipefd[1] = -1;

        if (ret != 0) {
            fprintf(stderr, "[ytdlp] posix_spawn failed: %s\n", strerror(ret));
            close(pipefd[0]);
            continue;
        }

        // Read output from pipe
        char buffer[4096];
        ssize_t bytes_read;
        
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0';
            
            // Resize result buffer if needed
            if (result_len + bytes_read + 1 > result_size) {
                result_size = result_len + bytes_read + 4096;
                char *new_result = realloc(result, result_size);
                if (!new_result) {
                    fprintf(stderr, "[ytdlp] realloc failed\n");
                    free(result);
                    close(pipefd[0]);
                    result = NULL;
                    goto wait_child;
                }
                result = new_result;
            }
            
            memcpy(result + result_len, buffer, bytes_read);
            result_len += bytes_read;
            result[result_len] = '\0';
        }

        close(pipefd[0]);
        pipefd[0] = -1;

wait_child:
        // Wait for child process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0 && result && result_len > 0) {
            // Success - strip trailing newlines
            while (result_len > 0 && (result[result_len - 1] == '\n' || result[result_len - 1] == '\r')) {
                result[--result_len] = '\0';
            }
            
            fprintf(stderr, "[ytdlp] Extracted URL (%zu bytes): %.100s...\n", result_len, result);
            return result;
        }

        // Clean up on failure
        if (result) {
            free(result);
            result = NULL;
            result_len = 0;
            result_size = 0;
        }

        fprintf(stderr, "[ytdlp] yt-dlp exited with status %d, trying next path...\n", 
                WIFEXITED(status) ? WEXITSTATUS(status) : -1);
    }

    fprintf(stderr, "[ytdlp] All yt-dlp paths failed\n");
    return NULL;
}