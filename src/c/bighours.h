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

#define FONT_BIG	FONT_KEY_ROBOTO_BOLD_SUBSET_49
#define FONT_MEDIUM	FONT_KEY_GOTHIC_28_BOLD
#define FONT_SMALL	FONT_KEY_GOTHIC_28_BOLD

#endif /* _BIGHOURS_H_ */
