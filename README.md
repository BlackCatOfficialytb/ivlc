# iVLC - Lightweight Media Player for Jailbroken iOS 12+

A minimal, pure C99 media player prototype for jailbroken iOS 12+ (arm64) that demonstrates end-to-end YouTube playback using **yt-dlp** for stream extraction and **LibVLC** for hardware-accelerated decoding/rendering.

## Features

- **Pure C99** - No Swift, no Objective-C, no C++
- **YouTube Support** - Resolves direct M3U8/MP4 URLs via `yt-dlp`
- **LibVLC Engine** - Hardware-accelerated video/audio playback
- **iOS 12+ arm64** - Targets jailbroken devices (Procursus/rootless)
- **Minimal Dependencies** - Only `python3`, `ffmpeg`, `libvlc`, `libvlccore`
- **Cross-compilable** - Build on Windows/macOS/Linux with Theos/clang

## Architecture

```
┌─────────────┐     ┌──────────────┐     ┌─────────────┐
│  YouTube    │────▶│   yt-dlp     │────▶│  Direct     │
│  URL        │     │  (Procursus) │     │  Stream URL │
└─────────────┘     └──────────────┘     └──────┬──────┘
                                                │
                                                ▼
┌─────────────┐     ┌──────────────┐     ┌─────────────┐
│  Video      │◀────│  LibVLC      │◀────│  libvlc_    │
│  Output     │     │  Player      │     │  media_*    │
└─────────────┘     └──────────────┘     └─────────────┘
```

## Project Structure

```
ivlc/
├── .vscode/
│   └── c_cpp_properties.json    # IntelliSense config (Windows cross-compile)
├── include/
│   ├── vlc/vlc.h                # LibVLC API stub
│   ├── stdio.h, stdlib.h, ...   # C stdlib stubs for IntelliSense
│   └── sys/types.h, wait.h      # POSIX stubs
├── lib/
│   └── README.md                # Instructions for libvlc.a / libvlccore.a
├── Makefile                     # Theos-style build (iphone:clang:14.5:12.0)
├── control                      # Debian package metadata
├── main.c                       # Entry point: LibVLC init → yt-dlp → playback
├── ytdlp_extract.h              # API: extract_stream_url()
└── ytdlp_extract.c              # posix_spawn + pipe capture implementation
```

## Requirements (on Device)

- **Jailbroken iOS 12.0+** (arm64)
- **Procursus APT** packages:
  - `python3` (for yt-dlp)
  - `ffmpeg`
  - `yt-dlp` (installed to `/var/jb/usr/bin/yt-dlp` or `/usr/bin/yt-dlp`)
- **LibVLC static libraries**: `libvlc.a`, `libvlccore.a` (in `./lib/`)

## Building

### On Device (with Theos)
```bash
# Install Theos and iOS SDK first
make clean all
# Output: ./ivlc (arm64 binary)
```

### Cross-compile from Windows (with LLVM/Clang at E:\llvm)
```bash
# Set up environment
export THEOS=/path/to/theos
export SDKROOT=/path/to/iPhoneOS14.5.sdk

# Build
make clean all
```

### Compiler Configuration
The project uses `clang` from `E:\llvm\bin\clang.exe` for Windows-side IntelliSense and cross-compilation.

## Running

```bash
# On jailbroken device
./ivlc "https://www.youtube.com/watch?v=dQw4w9WgXcQ"

# Or use default test URL (Rick Astley)
./ivlc
```

Press `Ctrl+C` to stop playback.

## How It Works

### 1. Stream Extraction (`ytdlp_extract.c`)
- Uses `posix_spawn()` for efficient process creation on iOS
- Executes: `yt-dlp -g -f "bv+ba/b" "<youtube_url>"`
- `-g` = get URL only (no download)
- `-f "bv+ba/b"` = prefer pre-muxed video+audio (`bv+ba`), fallback to best single stream (`b`)
  - This avoids separate video/audio URLs for 1080p+ where YouTube serves split streams
- Captures stdout via pipe, returns allocated URL string

### 2. LibVLC Pipeline (`main.c`)
```c
// Initialize LibVLC
libvlc_instance_t *inst = libvlc_new(argc, vlc_args);

// Extract direct URL
char *direct_url = extract_stream_url(youtube_url);

// Create media from URL
libvlc_media_t *media = libvlc_media_new_location(inst, direct_url);

// Create and start player
libvlc_media_player_t *mp = libvlc_media_player_new_from_media(media);
libvlc_media_player_play(mp);

// Event loop with signal handling
while (g_running) { sleep(1); }
```

## Debian Package

Build `.deb` for installation on device:
```bash
dpkg-deb -b . net.blackcat.ivlc_1.0.0-1_iphoneos-arm64.deb
```

Install on device:
```bash
dpkg -i net.blackcat.ivlc_1.0.0-1_iphoneos-arm64.deb
```

## License

MIT License - See LICENSE file for details.

## Repository

**Homepage**: https://github.com/BlackCatOfficialytb/ivlc

## Credits

- [VideoLAN](https://www.videolan.org/) - LibVLC
- [yt-dlp](https://github.com/yt-dlp/yt-dlp) - YouTube extraction
- [Procursus](https://procursus.sileo.app/) - iOS package manager
- [Theos](https://github.com/theos/theos) - iOS build system