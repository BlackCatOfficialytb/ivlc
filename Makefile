# iVLC - Lightweight Media Player for Jailbroken iOS 12+
# Theos-style Makefile for pure C compilation

TARGET = iphone:clang:14.5:12.0
ARCHS = arm64

# Compiler and linker
CC = $(THEOS)/toolchain/Xcode.xctoolchain/usr/bin/clang
CFLAGS = -std=c99 -Wall -I./include -isysroot $(THEOS)/sdks/iPhoneOS14.5.sdk
LDFLAGS = -L./lib -lvlc -lvlccore -liconv -lz -lm \
          -framework CoreGraphics -framework QuartzCore \
          -framework UIKit -framework Foundation \
          -framework VideoToolbox -framework Metal -framework AudioToolbox \
          -isysroot $(THEOS)/sdks/iPhoneOS14.5.sdk

# Source files
SRCS = main.c ytdlp_extract.c
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

.PHONY: all clean install