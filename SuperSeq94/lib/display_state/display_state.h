#ifndef H_DISPLAY_STATE
#define H_DISPLAY_STATE

#include <stdbool.h>

typedef struct display_state_s
{
	bool updateDisplay;
	bool redrawMenu;
} display_state_t;

#endif