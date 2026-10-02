/* Minimal objc/objc.h stub for IntelliSense on Windows */
#ifndef _OBJC_OBJC_H
#define _OBJC_OBJC_H

/* On Apple platforms, include the real objc/objc.h */
#if defined(__APPLE__) || defined(__MACH__)
#include <objc/objc.h>
#else

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct objc_class *Class;
typedef struct objc_object {
    Class isa;
} *id;

typedef struct objc_selector *SEL;
typedef id (*IMP)(id, SEL, ...);

#define YES 1
#define NO 0
#define nil ((id)0)

/* Basic types */
typedef signed char BOOL;
typedef unsigned int uint;
typedef unsigned long ulong;

#ifdef __cplusplus
}
#endif

#endif /* __APPLE__ || __MACH__ */
#endif /* _OBJC_OBJC_H */