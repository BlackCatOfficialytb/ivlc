/* Minimal objc/message.h stub for IntelliSense on Windows */
#ifndef _OBJC_MESSAGE_H
#define _OBJC_MESSAGE_H

/* On Apple platforms, include the real objc/message.h */
#if defined(__APPLE__) || defined(__MACH__)
#include <objc/message.h>
#else

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct objc_object *id;
typedef struct objc_selector *SEL;
typedef struct objc_super *objc_super_t;

/* On Windows, objc_msgSend returns id for object returns, use helpers for primitives */
id objc_msgSend(id self, SEL op, ...);
id objc_msgSendSuper(struct objc_super *super, SEL op, ...);
void objc_msgSend_void(id self, SEL op, ...);
uintptr_t objc_msgSend_fpret(id self, SEL op, ...);
uintptr_t objc_msgSend_stret(id self, SEL op, ...);

/* Helper macros for casting return values */
#define OBJC_MSGSEND_ID(self, op, ...) ((id)objc_msgSend((self), (op), ##__VA_ARGS__))
#define OBJC_MSGSEND_FLOAT(self, op, ...) (*(float*)&(uintptr_t){objc_msgSend_fpret((self), (op), ##__VA_ARGS__)})
#define OBJC_MSGSEND_DOUBLE(self, op, ...) (*(double*)&(uintptr_t){objc_msgSend_fpret((self), (op), ##__VA_ARGS__)})
#define OBJC_MSGSEND_INT(self, op, ...) ((int)objc_msgSend((self), (op), ##__VA_ARGS__))
#define OBJC_MSGSEND_LONG(self, op, ...) ((long)objc_msgSend((self), (op), ##__VA_ARGS__))
#define OBJC_MSGSEND_BOOL(self, op, ...) ((BOOL)objc_msgSend((self), (op), ##__VA_ARGS__))

#ifdef __cplusplus
}
#endif

#endif /* __APPLE__ || __MACH__ */
#endif /* _OBJC_MESSAGE_H */