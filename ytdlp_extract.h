#ifndef YT_DLP_EXTRACT_H
#define YT_DLP_EXTRACT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Extract direct stream URL from YouTube link using yt-dlp
 * 
 * Uses format selector "bv+ba/b" to prefer pre-muxed video+audio streams
 * (avoids separate video/audio URLs for 1080p+). Falls back to best single stream.
 * 
 * @param youtube_url The YouTube video URL (e.g., "https://youtube.com/watch?v=...")
 * @return Dynamically allocated string containing the direct M3U8/MP4 URL.
 *         Caller must free() the returned pointer. Returns NULL on failure.
 */
char *extract_stream_url(const char *youtube_url);

#ifdef __cplusplus
}
#endif

#endif /* YT_DLP_EXTRACT_H */