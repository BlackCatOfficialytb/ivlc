# LibVLC Static Libraries for iOS arm64

Place the following static libraries in this directory (`./lib/`):

- `libvlc.a` - LibVLC core library
- `libvlccore.a` - LibVLC core dependencies

## Building LibVLC for iOS arm64

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

## Procursus / Jailbroken iOS

On a jailbroken device with Procursus, you can also install the prebuilt libraries:

```bash
# Install via APT (if available in repos)
apt install libvlc-dev

# Or copy from system
cp /var/jb/usr/lib/libvlc.a ./lib/
cp /var/jb/usr/lib/libvlccore.a ./lib/
```

## Required Headers

Headers are in `../include/vlc/` (vlc.h stub provided for IntelliSense).
Real headers come with the libvlc-dev package or VLC source tree.