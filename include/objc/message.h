/* Minimal objc/message.h stub for IntelliSense on Windows */
#ifndef _OBJC_MESSAGE_H
#define _OBJC_MESSAGE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct objc_object *id;
typedef struct objc_selector *SEL;

id objc_msgSend(id self, SEL op, ...);
id objc_msgSendSuper(struct objc_super *super, SEL op, ...);
void objc_msgSend_void(id self, SEL op, ...);
void objc_msgSend_fpret(id self, SEL op, ...);

#ifdef __cplusplus
}
#endif

#endif /* _OBJC_MESSAGE_H */