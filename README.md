# iVLC - Lightweight Media Player for Jailbroken iOS 12+

A minimal, pure C99 media player prototype for jailbroken iOS 12+ (arm64) that demonstrates end-to-end YouTube playback using **yt-dlp** for stream extraction and **LibVLC** or **VLCKit/MobileVLCKit** for hardware-accelerated decoding/rendering.

## Features

- **Pure C99** - No Swift, no Objective-C, no C++ (uses `objc_msgSend` for CALayer binding)
- **YouTube Support** - Resolves direct M3U8/MP4 URLs via `yt-dlp` (format `bv+ba/b`)
- **Dual Backend Support** - Choose between raw **LibVLC** or **VLCKit/MobileVLCKit** frameworks
- **iOS 12+ arm64** - Targets jailbroken devices (Procursus/rootless, Dopamine/Palera1n)
- **Minimal Dependencies** - Only `python3`, `ffmpeg`, `libvlc`/`libvlccore` or VLCKit framework
- **Cross-compilable** - Build on Windows/macOS/Linux with Theos/clang (`E:\llvm`)
- **CALayer Video Output** - Binds video output to `CALayer` via pure C Objective-C runtime

## Architecture

### LibVLC Backend (Default)

```txt
┌─────────────┐     ┌──────────────┐     ┌─────────────┐
│  YouTube    │────▶│   yt-dlp     │────▶│  Direct     │
│  URL        │     │  (Procursus) │     │  Stream URL │
└─────────────┘     └──────────────┘     └──────┬──────┘
                                                │
                                                ▼
┌─────────────┐     ┌──────────────┐     ┌─────────────┐
│  CALayer    │◀────│  LibVLC      │◀────│  libvlc_    │
│  (Video)    │     │  Player      │     │  media_*    │
└─────────────┘     └──────────────┘     └─────────────┘
       │                   │
       ▼                   ▼
┌─────────────────────────────────────┐
│  Hardware Acceleration              │
│  VideoToolbox (decode) + Metal (GPU)│
└─────────────────────────────────────┘
```

### VLCKit/MobileVLCKit Backend

```txt
┌─────────────┐     ┌──────────────┐     ┌─────────────┐
│  YouTube    │────▶│   yt-dlp     │────▶│  Direct     │
│  URL        │     │  (Procursus) │     │  Stream URL │
└─────────────┘     └──────────────┘     └──────┬──────┘
                                                │
                                                ▼
┌─────────────┐     ┌──────────────┐     ┌─────────────┐
│  CALayer    │◀────│  VLCKit      │◀────│  VLCMedia   │
│  (Video)    │     │  Player      │     │  Player     │
└─────────────┘     └──────────────┘     └─────────────┘
       │                   │
       ▼                   ▼
┌─────────────────────────────────────┐
│  Hardware Acceleration              │
│  VideoToolbox (decode) + Metal (GPU)│
└─────────────────────────────────────┘
```

**CALayer Binding (Pure C - LibVLC):**

```c
#include <objc/objc.h>
#include <objc/message.h>
#include <QuartzCore/QuartzCore.h>

void *layer = objc_msgSend(objc_getClass("CALayer"), sel_registerName("layer"));
libvlc_video_set_callbacks(mp, NULL, NULL, NULL, layer);
```

**CALayer Binding (Pure C - VLCKit):**

```c
#include <objc/objc.h>
#include <objc/message.h>
#include <QuartzCore/QuartzCore.h>
#include "vlckit/vlckit_wrapper.h"

void *layer = objc_msgSend(objc_getClass("CALayer"), sel_registerName("layer"));
vlckit_media_player_set_calayer(player, layer);
vlckit_media_player_set_video_output_mode(player, VLCKitVideoOutputModeCALayer);
```

## Project Structure

```txt
ivlc/
├── .vscode/
│   └── c_cpp_properties.json    # IntelliSense config (Windows cross-compile)
├── include/
│   ├── vlc/vlc.h                # LibVLC API stub (with video callbacks)
│   ├── vlckit/
│   │   ├── vlckit_wrapper.h     # Unified C API for VLCKit/MobileVLCKit/TVVLCKit
│   │   ├── MobileVLCKit.h       # MobileVLCKit framework stub (iOS)
│   │   ├── VLCKit.h             # VLCKit framework stub (macOS)
│   │   └── TVVLCKit.h           # TVVLCKit framework stub (tvOS)
│   ├── objc/objc.h              # Objective-C runtime stubs
│   ├── objc/message.h           # objc_msgSend declarations
│   ├── objc/runtime.h           # objc_getClass, sel_registerName
│   ├── QuartzCore/QuartzCore.h  # CALayer, UIColor stubs
│   ├── stdio.h, stdlib.h, ...   # C stdlib stubs for IntelliSense
│   └── sys/types.h, wait.h      # POSIX stubs
├── stdlib/
│   └── README.md                # Instructions for libvlc.a / libvlccore.a
├── Makefile                     # Theos-style build with BACKEND=libvlc|vlckit
├── control                      # Debian package metadata
├── main.c                       # Entry point: backend init → yt-dlp → CALayer
├── vlckit_wrapper.c             # VLCKit wrapper implementation (pure C)
├── ytdlp_extract.h              # API: extract_stream_url()
└── ytdlp_extract.c              # posix_spawn + pipe capture implementation
```

## Requirements (on Device)

### LibVLC Backend (Default)

- **Jailbroken iOS 12.0+** (arm64)
- **Procursus APT** packages:
  - `python3` (for yt-dlp)
  - `ffmpeg`
  - `yt-dlp` (installed to `/var/jb/usr/bin/yt-dlp` or `/usr/bin/yt-dlp`)
- **LibVLC static libraries**: `libvlc.a`, `libvlccore.a` (in `./lib/`)

### VLCKit/MobileVLCKit Backend

- **Jailbroken iOS 12.0+** (arm64)
- **Procursus APT** packages:
  - `python3` (for yt-dlp)
  - `ffmpeg`
  - `yt-dlp`
- **MobileVLCKit framework**: Install via CocoaPods/Carthage or copy framework to device
  - Framework must be available at link time (`-framework MobileVLCKit`)

## Building

### On Device (with Theos)

```bash
# Install Theos and iOS SDK first

# Build with LibVLC backend (default)
make clean all
# Output: ./ivlc (arm64 binary)

# Build with VLCKit/MobileVLCKit backend
make vlckit
# Output: ./ivlc (arm64 binary, linked against MobileVLCKit framework)
```

### Cross-compile from Windows (with LLVM/Clang at E:\llvm)

```bash
# Set up environment
export THEOS=/path/to/theos
export SDKROOT=/path/to/iPhoneOS14.5.sdk

# Build with LibVLC backend (default)
make clean all

# Build with VLCKit backend
make vlckit
```

### Compiler Configuration

The project uses `clang` from `E:\llvm\bin\clang.exe` for Windows-side IntelliSense and cross-compilation.

## Backend Selection

The Makefile supports two backends via the `BACKEND` variable:

| Backend | Define | Link Flags | Source Files |
|---------|--------|------------|--------------|
| LibVLC (default) | `USE_LIBVLC=1` | `-lvlc -lvlccore` | `main.c ytdlp_extract.c` |
| VLCKit | `USE_VLCKIT=1` | `-framework MobileVLCKit` | `main.c ytdlp_extract.c vlckit_wrapper.c` |

### LibVLC Backend
```bash
make libvlc
# or simply
make
```

### VLCKit/MobileVLCKit Backend
```bash
make vlckit
```

## VLCKit Framework Installation

### CocoaPods (iOS)
```ruby
target '<iOS Target>' do
    platform :ios, '12.0'
    pod 'MobileVLCKit', '~>3.3.0'
end
```

### Carthage (iOS)
```
binary "https://code.videolan.org/videolan/VLCKit/raw/master/Packaging/MobileVLCKit.json" ~> 3.3.0
```

### Manual Framework
Copy `MobileVLCKit.framework` to your project and link it in Xcode/Theos.

## Running

```bash
# On device
./ivlc "https://www.youtube.com/watch?v=dQw4w9WgXcQ"

# With custom URL
./ivlc "https://youtube.com/watch?v=YOUR_VIDEO_ID"
```

The player will:
1. Extract direct stream URL using yt-dlp
2. Initialize selected backend (LibVLC or VLCKit)
3. Create CALayer for video output
4. Start hardware-accelerated playback
5. Run until Ctrl+C or playback ends

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

## CI/CD (GitHub Actions)

### iVLC Build with Prebuilt LibVLC (from libvlc-gen)
**Workflow:** `.github/workflows/build-ivlc-prebuilt.yml`

Pulls prebuilt LibVLC from **BlackCatOfficialytb/libvlc-gen** releases (separate project):
1. Fetches latest release from `libvlc-gen` repo
2. Downloads `libvlc.a`, `libvlccore.a`, headers as artifacts
3. Builds iVLC binary using Theos
4. Creates combined release tagged with both versions

```bash
# Trigger manually
gh workflow run build-ivlc-prebuilt.yml
```

### Continuous Integration
**Workflow:** `.github/workflows/ci.yml`

Runs on every push/PR:
- **Lint**: `clang-tidy` on source files
- **Linux Build**: Native x86_64 test build with system VLC
- **iOS Simulator Build**: arm64 simulator build with Homebrew VLC

```bash
# View CI status
gh run list --workflow=ci.yml
```

### Artifacts
Each workflow run produces downloadable artifacts:
- `libvlc-ios-arm64` - Static libraries + headers (from libvlc-gen)
- `ivlc-ios-arm64` - iVLC binary

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