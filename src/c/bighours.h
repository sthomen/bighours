#ifndef _BIGHOURS_H_
#define _BIGHOURS_H_

#include <pebble.h>

#define BACKGROUND GColorBlack
#define FOREGROUND GColorWhite

#ifdef PBL_COLOR
#define HIGHLIGHT GColorYellow
#else
#define HIGHLIGHT GColorWhite
#endif

#endif /* _BIGHOURS_H_ */
