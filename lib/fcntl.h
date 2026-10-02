/* Minimal fcntl.h stub for IntelliSense on Windows */
#ifndef _FCNTL_H
#define _FCNTL_H

/* On Apple platforms, include the real fcntl.h */
#if defined(__APPLE__) || defined(__MACH__)
#include <fcntl.h>
#else

#ifdef __cplusplus
extern "C" {
#endif

#define O_RDONLY    0x0000
#define O_WRONLY    0x0001
#define O_RDWR      0x0002
#define O_CREAT     0x0100
#define O_EXCL      0x0200
#define O_TRUNC     0x0400
#define O_APPEND    0x0800
#define O_NONBLOCK  0x1000

int open(const char *pathname, int flags, ...);
int fcntl(int fd, int cmd, ...);

#ifdef __cplusplus
}
#endif

#endif /* __APPLE__ || __MACH__ */
#endif /* _FCNTL_H */