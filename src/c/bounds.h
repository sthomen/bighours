#ifndef _BOUNDS_H_
#define _BOUNDS_H_

#include <pebble.h>

#define VW(percent) bounds_percent_to_pixels(bounds.size.w, percent)
#define VH(percent) bounds_percent_to_pixels(bounds.size.h, percent)

extern GRect bounds;

void bounds_init(Layer *root);
int16_t bounds_percent_to_pixels(uint16_t max, float percent);

#endif /* _BOUNDS_H_ */
