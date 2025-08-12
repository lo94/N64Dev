#ifndef H_UI_ELEMENTS
#define H_UI_ELEMENTS

#include <stdbool.h>
#include <stdint.h>
#include <libdragon.h>

// TODO : Replace other elements values with this
typedef struct base_element_s {
	char * id;
	int x, y, width, height;
	bool visible;
	bool enabled;
	bool selected;
	bool focused;
	color_t backgroundColor;
	color_t foregroundColor;
	color_t borderColor;
	// Maybe have function pointers for selection and stuff?
	// Also might be useful for save functions
	void (*selectAction)();
} base_element_t;

typedef struct text_element_s
{
	int x, y;
	bool visible;
	const char * const text;
	base_element_t base;
} text_element_t;

typedef struct text_input_element_s
{
	base_element_t base;
	char * text;
	char * defaultText;
} text_input_element_t;

typedef struct int_input_element_s
{
	base_element_t base;
	int value;
	int defaultValue;
	int minValue;
	int maxValue;
} int_input_element_t;

typedef struct text_option_selector_s
{
	base_element_t base;
	char * choices;
} text_option_selector_t;

/*
	Note: Elements within the grid cell's x/y coordinates are relative to the grid cell
	0 is the far left (x) and 100 is the far right
	0 is the top (y) and 100 is the bottom
*/
typedef struct selection_grid_cell_s
{
	base_element_t base;
	bool selected;
	text_option_selector_t *textChoiceSelectors;
	text_element_t *textElements;
	int textElementCount;
	int_input_element_t *intInputs;
	int intInputCount;
	void (*select)();
	void (*deselect)();
} selection_grid_cell_t;

typedef struct selection_grid_s
{
	base_element_t base;
	int width;
	int height;
	int curX, curY;
	// Use enums from joypad_2d_t to assign navigation controls
	int navigationInput;
	int navigationCooldown; // in microseconds (500000 = half second)
	bool onCooldown;
	void (*navigate)();

	selection_grid_cell_t ***gridCells;
} selection_grid_t;

typedef struct section_element_s section_element_t;

struct section_element_s
{
	int textElementCount;
	text_element_t *textElements;
	int selectionGridElementCount;
	selection_grid_t **selectionGridElements;
	bool isSelected;
	int curSelection;
	bool refresh;
    bool visible;
	int childCount;
	section_element_t **children;
};

#endif