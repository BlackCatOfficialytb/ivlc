/* Minimal objc/runtime.h stub for IntelliSense on Windows */
#ifndef _OBJC_RUNTIME_H
#define _OBJC_RUNTIME_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct objc_class *Class;
typedef struct objc_object *id;
typedef struct objc_selector *SEL;
typedef struct objc_method *Method;
typedef struct objc_ivar *Ivar;
typedef struct objc_property *objc_property_t;

Class objc_getClass(const char *name);
Class objc_getMetaClass(const char *name);
Class objc_allocateClassPair(Class superclass, const char *name, size_t extraBytes);
void objc_registerClassPair(Class cls);
void objc_disposeClassPair(Class cls);

SEL sel_registerName(const char *str);
const char *sel_getName(SEL sel);

id class_createInstance(Class cls, size_t extraBytes);
void *object_getIvar(id obj, Ivar ivar);
void object_setIvar(id obj, Ivar ivar, void *value);

Method class_getInstanceMethod(Class cls, SEL sel);
Method class_getClassMethod(Class cls, SEL sel);
IMP method_getImplementation(Method m);
const char *method_getTypeEncoding(Method m);

#ifdef __cplusplus
}
#endif

#endif /* _OBJC_RUNTIME_H */