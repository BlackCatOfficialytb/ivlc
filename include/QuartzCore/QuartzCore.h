/* Minimal QuartzCore/QuartzCore.h stub for IntelliSense on Windows */
#ifndef _QUARTZCORE_QUARTZCORE_H
#define _QUARTZCORE_QUARTZCORE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Core Animation types */
typedef struct _CA_CALayer *CALayerRef;
typedef CALayerRef id;  /* Objective-C object */

typedef struct _CA_CAAnimation *CAAnimationRef;
typedef struct _CA_CATransaction *CATransactionRef;

/* Core Graphics types (subset) */
typedef struct CGColor *CGColorRef;
typedef struct CGContext *CGContextRef;

/* UIColor (UIKit) - minimal for background color */
typedef struct _UI_UIColor *UIColorRef;

/* CALayer class methods */
id CALayer_layer(void);  /* +[CALayer layer] */

/* CALayer instance methods */
void CALayer_setOpaque(id layer, int opaque);
void CALayer_setBackgroundColor(id layer, CGColorRef color);

/* UIColor class methods */
UIColorRef UIColor_blackColor(void);
CGColorRef UIColor_CGColor(UIColorRef color);

#ifdef __cplusplus
}
#endif

#endif /* _QUARTZCORE_QUARTZCORE_H */