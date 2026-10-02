/* Minimal sys/types.h stub for IntelliSense on Windows */
#ifndef _SYS_TYPES_H
#define _SYS_TYPES_H

/* On Apple platforms, include the real sys/types.h */
#if defined(__APPLE__) || defined(__MACH__)
#include <sys/types.h>
#else

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef long ssize_t;
typedef int pid_t;
typedef unsigned int uid_t;
typedef unsigned int gid_t;
typedef long off_t;
typedef long time_t;
typedef int mode_t;

#ifdef __cplusplus
}
#endif

#endif /* __APPLE__ || __MACH__ */
#endif /* _SYS_TYPES_H */