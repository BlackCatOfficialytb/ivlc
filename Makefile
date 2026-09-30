# iVLC - Lightweight Media Player for Jailbroken iOS 12+
# Theos-style Makefile for pure C compilation
# Supports both LibVLC (raw) and VLCKit/MobileVLCKit backends

TARGET = iphone:clang:14.5:12.0
ARCHS = arm64

# Backend selection: libvlc (default) or vlckit
BACKEND ?= libvlc

# Compiler and linker
CC = $(THEOS)/toolchain/Xcode.xctoolchain/usr/bin/clang
CFLAGS = -std=c99 -Wall -I./include -isysroot $(THEOS)/sdks/iPhoneOS14.5.sdk

# Common frameworks
COMMON_FRAMEWORKS = -framework CoreGraphics -framework QuartzCore \
                    -framework UIKit -framework Foundation \
                    -framework VideoToolbox -framework Metal -framework AudioToolbox

# Backend-specific configuration
ifeq ($(BACKEND),vlckit)
    # VLCKit/MobileVLCKit backend
    CFLAGS += -DUSE_VLCKIT=1
    LDFLAGS = $(COMMON_FRAMEWORKS) \
              -framework MobileVLCKit \
              -isysroot $(THEOS)/sdks/iPhoneOS14.5.sdk
    SRCS = main.c ytdlp_extract.c vlckit_wrapper.c
else
    # LibVLC raw backend (default)
    CFLAGS += -DUSE_LIBVLC=1
    LDFLAGS = -L./lib -lvlc -lvlccore -liconv -lz -lm \
              $(COMMON_FRAMEWORKS) \
              -isysroot $(THEOS)/sdks/iPhoneOS14.5.sdk
    SRCS = main.c ytdlp_extract.c
endif

OBJS = $(SRCS:.c=.o)

# Output
TARGET_NAME = ivlc
OUTPUT = $(TARGET_NAME)

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OUTPUT)

install: $(OUTPUT)
	# Installation handled by Debian package

# Build with VLCKit backend
vlckit: clean
	$(MAKE) BACKEND=vlckit

# Build with LibVLC backend (default)
libvlc: clean
	$(MAKE) BACKEND=libvlc

.PHONY: all clean install vlckit libvlc