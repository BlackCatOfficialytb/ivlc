/* Minimal stddef.h stub for IntelliSense on Windows */
#ifndef _STDDEF_H
#define _STDDEF_H

/* On Apple platforms, include the real stddef.h */
#if defined(__APPLE__) || defined(__MACH__)
#include <stddef.h>
#else

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long long size_t;
typedef long long ptrdiff_t;
/* wchar_t is defined by system headers on Windows, but we need it for stdlib.h */
#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif

#define NULL ((void*)0)
#define offsetof(type, member) __builtin_offsetof(type, member)

#ifdef __cplusplus
}
#endif

#endif /* __APPLE__ || __MACH__ */
#endif /* _STDDEF_H */