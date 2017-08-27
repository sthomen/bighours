#include "bounds.h"
#include "debug.h"

GRect bounds;

void bounds_init(Layer *root)
{
	bounds=layer_get_bounds(root);
}

int16_t bounds_percent_to_pixels(uint16_t max, float percent)
{
	uint16_t pixels;

	pixels=max*(percent/100.0);

	return pixels;
}
