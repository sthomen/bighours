#include "bighours.h"
#include "bounds.h"
#include "utils.h"
#include "debug.h"

Window *window_main;

enum {
	HOURS		=0,
	MINUTES	=1,
	AMPM		=2,
	LINE		=3,
	DATE		=4,
	LAYERS_MAX=5
};

enum FontSize {
  FONT_SMALL,
  FONT_MEDIUM,
  FONT_BIG
};

Layer *layers[LAYERS_MAX];

/*
 * Note that the x, y, width, height numbers are in percent
 * as used by the VH() and VW() macros
 */
struct layerdata {
	uint8_t x;
	uint8_t y;
	uint8_t width;
	uint8_t height;
	char *text;
	size_t text_len;
	enum FontSize fontsize;
	bool highlight;
} layerdata[LAYERS_MAX] = {
	{   0,  15,  65,  55, NULL,  3, FONT_BIG,	false },	/* hours */
	{  65,  30,  21,  20, NULL,  3, FONT_MEDIUM,	true },		/* minutes */
	{  65,  45,  21,  20, NULL,  3, FONT_MEDIUM,	true },		/* ampm */
	{   0,  66, 100,   1, NULL,  0, FONT_SMALL,	false },	/* line */
	{   0,  67, 100,  30, NULL, 15, FONT_SMALL,	true }		/* date */
};

static void update_line(Layer *this, GContext *ctx)
{
	GRect lb=layer_get_bounds(this);

	graphics_context_set_stroke_color(ctx, HIGHLIGHT);
	graphics_draw_line(ctx, GPoint(0, lb.size.h/2), GPoint(lb.size.w, lb.size.h/2));
}

/*
 * XXX use units_changed here to only update the required layer
 */
static void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
	char *fmt;

	for (int i=0;i<LAYERS_MAX;i++) {
		fmt=NULL;

		switch (i) {
			case HOURS:
				fmt="%I";
				break;
			case MINUTES:
				fmt="%M";
				break;
			case AMPM:
				fmt="%p";
				break;
			case DATE:
				fmt="%b %d %a";
				break;
		}

		if (fmt != NULL) {
			strftime(layerdata[i].text, layerdata[i].text_len, fmt, tick_time);

			strupper(layerdata[i].text, layerdata[i].text_len);
		}
	}

	layer_mark_dirty(window_get_root_layer(window_main));
}

static void window_main_load(Window *window)
{
	/* handlers */

	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

	/* setup windows & layers */

	Layer *root=window_get_root_layer(window_main);
	// Initialize bounds from bounds.h
	bounds_init(root);

	for (int i=0;i<LAYERS_MAX;i++) {
		GRect lb=GRect(
			VW(layerdata[i].x),
			VH(layerdata[i].y),
			VW(layerdata[i].width),
			VH(layerdata[i].height));

		Layer *current;
		TextLayer *tl;
		BitmapLayer *bl;
		GTextAlignment tl_align=GTextAlignmentRight;
		GFont font=NULL;

		switch (i) {
			// text layers
			case HOURS:
			case MINUTES:
			case AMPM:
			case DATE:
				tl=text_layer_create(lb);

				if (layerdata[i].highlight) {
					text_layer_set_text_color(tl, HIGHLIGHT);
				} else {
					text_layer_set_text_color(tl, FOREGROUND);
				}

				text_layer_set_background_color(tl, GColorClear);

				switch (layerdata[i].fontsize) {
					case FONT_SMALL:
						font=fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
						break;
					case FONT_MEDIUM:
						font=fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
						break;
					case FONT_BIG:
						font=fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DROID_SANS_NUMBERS_74));
						break;
				}

				text_layer_set_font(tl, font);
				layerdata[i].text=(char *)malloc(layerdata[i].text_len);
				text_layer_set_text(tl, layerdata[i].text);

				if (i == DATE)
					tl_align=GTextAlignmentCenter;

				text_layer_set_text_alignment(tl, tl_align);

				current=text_layer_get_layer(tl);
				break;
			// this one is a bitmap layer
			case LINE:
				bl=bitmap_layer_create(lb);
				current=bitmap_layer_get_layer(bl);
				layer_set_update_proc(current, update_line);
				break;
		}

		layer_add_child(root, current);
	}
}

static void window_main_unload(Window *window)
{
	for (int i=0;i<LAYERS_MAX;i++) {
		switch (i) {
			case HOURS:
			case MINUTES:
			case AMPM:
			case DATE:
				free(layerdata[i].text);
				text_layer_destroy((TextLayer *)layers[i]);
				break;
			case LINE:
				bitmap_layer_destroy((BitmapLayer *)layers[i]);
				break;
		}
	}
}

int main(void) {
	static WindowHandlers window_handlers={
		.load = window_main_load,
		.unload = window_main_unload
	};

	window_main = window_create();
	window_set_window_handlers(window_main, window_handlers);
	window_set_background_color(window_main, BACKGROUND);
	window_stack_push(window_main, true);

	app_event_loop();

	window_destroy(window_main);
	tick_timer_service_unsubscribe();
}
