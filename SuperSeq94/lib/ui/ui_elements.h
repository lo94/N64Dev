#ifndef H_UI_ELEMENTS
#define H_UI_ELEMENTS

#include <stdbool.h>
#include <stdint.h>
#include <libdragon.h>

typedef enum {
    UI_ELEMENT_BASE_TYPE,
    UI_ELEMENT_LABEL_TYPE,
    UI_ELEMENT_SECTION_TYPE,
	UI_ELEMENT_SELECTION_GRID_TYPE,
	UI_ELEMENT_SELECTION_GRID_CELL_TYPE,
	UI_ELEMENT_INPUT_TEXT_TYPE,
	UI_ELEMENT_INPUT_INT_TYPE,
	UI_ELEMENT_INPUT_OPTION_TYPE
    // ... add more as needed
} ui_element_type_t;

// TODO : Replace other elements values with this
typedef struct ui_element_base_s {
	ui_element_type_t type;
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
	bool (*update)(void *self);
	void (*render)(void *self);
} ui_element_base_t;

#define DEFAULT_ELEMENT_BASE_FIELDS \
    .type = UI_ELEMENT_BASE_TYPE

#define DEFAULT_ELEMENT_BASE {DEFAULT_ELEMENT_BASE_FIELDS}

typedef struct ui_element_label_s
{
	ui_element_type_t type;
	int x, y;
	bool visible;
	const char * const text;
	ui_element_base_t base;
} ui_element_label_t;

#define DEFAULT_UI_ELEMENT_LABEL_FIELDS \
    .type = UI_ELEMENT_LABEL_TYPE

#define DEFAULT_UI_ELEMENT_LABEL {DEFAULT_UI_ELEMENT_LABEL_FIELDS}

typedef struct ui_element_input_text_s
{
	ui_element_type_t type;
	ui_element_base_t base;
	char * text;
	char * defaultText;
} ui_element_input_text_t;

#define DEFAULT_UI_ELEMENT_INPUT_TEXT_FIELDS \
    .type = UI_ELEMENT_INPUT_TEXT_TYPE

#define DEFAULT_UI_ELEMENT_INPUT_TEXT {DEFAULT_UI_ELEMENT_INPUT_TEXT_FIELDS}

typedef struct ui_element_input_int_s
{
	ui_element_type_t type;
	ui_element_base_t base;
	int value;
	int defaultValue;
	int minValue;
	int maxValue;
} ui_element_input_int_t;

ui_element_input_int_t input_int(char * id, int defaultValue, int minValue, int maxValue);

#define DEFAULT_UI_ELEMENT_INPUT_INT_FIELDS \
    .type = UI_ELEMENT_INPUT_INT_TYPE

#define DEFAULT_UI_ELEMENT_INPUT_INT {DEFAULT_UI_ELEMENT_INPUT_INT_FIELDS}

typedef struct ui_element_input_option_s
{
	ui_element_type_t type;
	ui_element_base_t base;
	char * choices;
} ui_element_input_option_t;

#define DEFAULT_UI_ELEMENT_INPUT_OPTION_FIELDS \
    .type = UI_ELEMENT_INPUT_OPTION_TYPE

#define DEFAULT_UI_ELEMENT_INPUT_OPTION {DEFAULT_UI_ELEMENT_INPUT_OPTION_FIELDS}

/*
	Note: Elements within the grid cell's x/y coordinates are relative to the grid cell
	0 is the far left (x) and 100 is the far right
	0 is the top (y) and 100 is the bottom
*/
typedef struct ui_element_selection_grid_cell_s
{
	ui_element_type_t type;
	ui_element_base_t base;
	bool selected;
	ui_element_input_option_t *textChoiceSelectors;
	ui_element_label_t *textElements;
	int textElementCount;
	ui_element_input_int_t **intInputs;
	int intInputCount;
	void (*select)();
	void (*deselect)();
} ui_element_selection_grid_cell_t;

#define DEFAULT_UI_ELEMENT_SELECTION_GRID_CELL_FIELDS \
    .type = UI_ELEMENT_SELECTION_GRID_CELL_TYPE, \
	.intInputCount = 0

#define DEFAULT_UI_ELEMENT_SELECTION_GRID_CELL {DEFAULT_UI_ELEMENT_SELECTION_GRID_CELL_FIELDS}

typedef struct ui_element_selection_grid_s
{
	ui_element_type_t type;
	ui_element_base_t base;
	int width;
	int height;
	int curX, curY;
	// Use enums from joypad_2d_t to assign navigation controls
	int navigationInput;
	int navigationScrollDelay; //Time before scroll activates on hold
	int navigationCooldown; // in microseconds (500000 = half second)
	bool onCooldown;
	void (*navigate)();

	ui_element_selection_grid_cell_t ***gridCells;
} ui_element_selection_grid_t;

ui_element_selection_grid_t selection_grid(char * id, int x, int y, int width, int height, int navigationInput);
bool selection_grid_update(void *self);
void selection_grid_cells_init(ui_element_selection_grid_t *grid, char **rowNames, int rowNamesCount, char **colNames, int colNamesCount, int xStartPos, int xPosIncrement, int yStartPos, int yPosIncrement, int cellWidth, int cellHeight);
ui_element_selection_grid_cell_t* selection_grid_get_cell(ui_element_selection_grid_t *grid, int xIndex, int yIndex);
void selection_grid_add_int_input_to_cell_by_index(ui_element_selection_grid_t *grid, int xIndex, int yIndex, int defaultValue, int minValue, int maxValue);

#define DEFAULT_UI_ELEMENT_SELECTION_GRID_FIELDS \
    .type = UI_ELEMENT_SELECTION_GRID_TYPE, .base = { .update = selection_grid_update }

#define DEFAULT_UI_ELEMENT_SELECTION_GRID {DEFAULT_UI_ELEMENT_SELECTION_GRID_FIELDS}

typedef struct section_element_s section_element_t;

struct section_element_s
{
	ui_element_type_t type;
	int textElementCount;
	ui_element_label_t *textElements;
	int selectionGridElementCount;
	ui_element_selection_grid_t **selectionGridElements;
	bool isSelected;
	int curSelection;
	bool refresh;
    bool visible;
	int childCount;
	section_element_t **children;
};

#define DEFAULT_UI_ELEMENT_SECTION_FIELDS \
    .type = UI_ELEMENT_SECTION_TYPE

#define DEFAULT_UI_ELEMENT_SECTION {DEFAULT_UI_ELEMENT_SECTION_FIELDS}

#endif