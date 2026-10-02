# Backend Libraries for iOS arm64

This directory contains backend-specific libraries. The project supports two backends:

## 1. LibVLC Backend (Default)

Place the following static libraries in this directory (`./lib/`):

- `libvlc.a` - LibVLC core library
- `libvlccore.a` - LibVLC core dependencies

### Building LibVLC for iOS arm64

On a macOS machine with Xcode and the iOS SDK:

```bash
# Clone VLC source
git clone https://code.videolan.org/videolan/vlc.git
cd vlc

# Configure for iOS arm64
./bootstrap
mkdir build-ios-arm64 && cd build-ios-arm64
../extras/package/ios/build.sh -a arm64 -s "12.0" -d

# Or use the contrib system directly
cd ../contrib
mkdir build-ios-arm64 && cd build-ios-arm64
../../configure --host=aarch64-apple-darwin --prefix=$(pwd)/install
make -j$(sysctl -n hw.ncpu)
make install

# The static libraries will be in install/lib/
cp install/lib/libvlc.a ../../../lib/
cp install/lib/libvlccore.a ../../../lib/
```

### Procursus / Jailbroken iOS

On a jailbroken device with Procursus, you can also install the prebuilt libraries:

```bash
# Install via APT (if available in repos)
apt install libvlc-dev

# Or copy from system
cp /var/jb/usr/lib/libvlc.a ./lib/
cp /var/jb/usr/lib/libvlccore.a ./lib/
```

### Required Headers

Headers are in `../include/vlc/` (vlc.h stub provided for IntelliSense).
Real headers come with the libvlc-dev package or VLC source tree.

---

## 2. VLCKit/MobileVLCKit Backend

For the VLCKit backend, you need the **MobileVLCKit.framework** (iOS) or **VLCKit.framework** (macOS) or **TVVLCKit.framework** (tvOS).

### Installation Options

#### CocoaPods (iOS)
```ruby
target '<iOS Target>' do
    platform :ios, '12.0'
    pod 'MobileVLCKit', '~>3.3.0'
end
```

#### Carthage (iOS)
```
binary "https://code.videolan.org/videolan/VLCKit/raw/master/Packaging/MobileVLCKit.json" ~> 3.3.0
```

#### Manual Framework
Download the framework from VideoLAN releases and place it in your project:
- iOS: `MobileVLCKit.framework`
- macOS: `VLCKit.framework`
- tvOS: `TVVLCKit.framework`

### Linking

The Makefile uses `-framework MobileVLCKit` for iOS builds. Ensure the framework is in your framework search paths.

### Required Headers

Framework stubs for IntelliSense are in `../include/vlckit/`:
- `MobileVLCKit.h` - iOS framework stub
- `VLCKit.h` - macOS framework stub
- `TVVLCKit.h` - tvOS framework stub
- `vlckit_wrapper.h` - Unified C API wrapper

Real headers come with the framework distribution.