/* Minimal stddef.h stub for IntelliSense on Windows */
#ifndef _STDDEF_H
#define _STDDEF_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long long size_t;
typedef long long ptrdiff_t;
typedef int wchar_t;

#define NULL ((void*)0)
#define offsetof(type, member) __builtin_offsetof(type, member)

#ifdef __cplusplus
}
#endif

#endif /* _STDDEF_H */