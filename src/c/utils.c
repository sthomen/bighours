#include "utils.h"

void strupper(char *string, size_t len)
{
	uint8_t i;
	for (i=0;i<len;i++) {
		if ((uint8_t)string[i] > 97 && (uint8_t)string[i] < 123)
			string[i]-=32;
	}
}

