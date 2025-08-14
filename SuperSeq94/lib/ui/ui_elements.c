#include "ui_elements.h"
#include "../input/input.h"

// selection_grid_t constructor
ui_element_selection_grid_t selection_grid(char * id, int x, int y, int width, int height, int navigationInput)
{
    ui_element_selection_grid_t grid = DEFAULT_UI_ELEMENT_SELECTION_GRID;
    grid.base.id = id;
    grid.base.x = x;
    grid.base.y = y;
    grid.width = width;
    grid.height = height;
    grid.navigationInput = navigationInput;
    //selection_grid_cells_init(&grid, rowNames, colNames, xStartPos, xPosIncrement, yStartPos, yPosIncrement, cellWidth, cellHeight)
    return grid;
}

// Initialize the grid cells of a selection grid and set them to its gridCells
void selection_grid_cells_init(ui_element_selection_grid_t *grid, char **rowNames, int rowNamesCount, char **colNames, int colNamesCount, int xStartPos, int xPosIncrement, int yStartPos, int yPosIncrement, int cellWidth, int cellHeight)
{
	ui_element_selection_grid_cell_t ***gridCells = malloc(grid->width * sizeof(ui_element_selection_grid_cell_t **));
	
	for(int i = 0; i < grid->width; i++)
	{
        gridCells[i] = malloc(grid->height * sizeof(ui_element_selection_grid_cell_t *));
		char *colName = colNamesCount == 1 ? colNames[0] : colNames[i];
        for(int j = 0; j < grid->height; j++)
        {
			char *rowName = rowNamesCount == 1 ? rowNames[0] : rowNames[j];
            int len = snprintf(
                NULL, 0, "%s_%d_%s_%d", colName, i, rowName, j
            );
            char *cellId = malloc(len + 1); // +1 for null terminator
            sprintf(cellId, "%s_%d_%s_%d", colName, i, rowName, j);
            ui_element_selection_grid_cell_t *cell = malloc(sizeof(ui_element_selection_grid_cell_t));
		    *cell = (ui_element_selection_grid_cell_t)
            { 
                DEFAULT_UI_ELEMENT_SELECTION_GRID_CELL_FIELDS, 
                .base = 
                {
                    .id = strdup(cellId), 
                    .x = xStartPos + (i * xPosIncrement), 
                    .y = yStartPos + (j * yPosIncrement), 
                    .width = cellWidth, 
                    .height = cellHeight,
                    .visible = true,
                    .enabled = true,
                    .selected = (i == 0 && j == 0) ? true : false
                } 
            };
            gridCells[i][j] = cell;
        }
	}
    grid->gridCells = gridCells;
}

ui_element_input_int_t input_int(char *id, int defaultValue, int minValue, int maxValue)
{
	ui_element_input_int_t element = DEFAULT_UI_ELEMENT_INPUT_INT;
    element.base.id = id;
	element.value = defaultValue;
	element.defaultValue = defaultValue;
	element.minValue = minValue;
	element.maxValue = maxValue;
    
    return element;
}

void selection_grid_add_int_input_to_cell_by_index(ui_element_selection_grid_t *grid, int xIndex, int yIndex, int defaultValue, int minValue, int maxValue)
{
	ui_element_selection_grid_cell_t *cell = selection_grid_get_cell(grid, xIndex, yIndex);
	int len = snprintf(
		NULL, 0, "%s_int_input", cell->base.id
	);
	char *inputId = malloc(len + 1); // +1 for null terminator
	sprintf(inputId, "%s_int_input", cell->base.id);

	ui_element_input_int_t *newInput = malloc(sizeof(ui_element_input_int_t));
    *newInput = input_int(inputId, defaultValue, minValue, maxValue);

	cell->intInputs = realloc(cell->intInputs, (cell->intInputCount + 1) * sizeof(ui_element_input_int_t *));
	cell->intInputs[cell->intInputCount] = newInput;
	cell->intInputCount++;
}

ui_element_selection_grid_cell_t* selection_grid_get_cell(ui_element_selection_grid_t *grid, int xIndex, int yIndex)
{
	if(xIndex < 0 || yIndex < 0 || xIndex >= grid->width || yIndex >= grid->height)
		return NULL;
	return grid->gridCells[xIndex][yIndex];
}

ui_element_selection_grid_cell_t* selection_grid_get_cur_cell(ui_element_selection_grid_t *grid)
{
	if(grid->curX < 0 || grid->curY < 0 || grid->curX >= grid->width || grid->curY >= grid->height)
		return NULL;
	return grid->gridCells[grid->curX][grid->curY];
}

bool selection_grid_cur_cell_focused(ui_element_selection_grid_t *grid)
{
	ui_element_selection_grid_cell_t *cell = selection_grid_get_cur_cell(grid);
	if(cell)
	{
		return cell->base.focused;
	}
	return false;
}

void selection_grid_focus_cur_cell(ui_element_selection_grid_t *grid)
{
	ui_element_selection_grid_cell_t *cell = selection_grid_get_cur_cell(grid);
	if(cell)
	{
		cell->base.focused = true;
	}
}

void selection_grid_defocus_cur_cell(ui_element_selection_grid_t *grid)
{
	ui_element_selection_grid_cell_t *cell = selection_grid_get_cur_cell(grid);
	if(cell)
	{
		cell->base.focused = true;
	}
}

void selection_grid_select_cur_cell(ui_element_selection_grid_t *grid)
{
	ui_element_selection_grid_cell_t *cell = selection_grid_get_cur_cell(grid);
	if(cell)
	{
		cell->base.selected = true;
	}
}

void input_int_focus(void *self)
{
	ui_element_input_int_t *input = (ui_element_input_int_t *)self;
	input->base.focused = true;
}

void selection_grid_cell_focus(void *self)
{
	ui_element_selection_grid_cell_t *cell = (ui_element_selection_grid_cell_t *)self;
	cell->base.focused = true;
	cell->curInput = 0;
	if(cell->inputCount == 1)
	{
		ui_element_base_t *baseElement = ((ui_element_base_t *)cell->inputs[0]);
		baseElement->focus(&baseElement);
	}
}

bool selection_grid_cell_update(void *self)
{
	ui_element_selection_grid_cell_t *cell = (ui_element_selection_grid_cell_t *)self;
	bool changed = false;
	
	if(input_buttons_pressed()->b)
	{
		cell->base.focused = false;
		changed = true;
		return changed;
	}
	
	if(input_direction_pressed(0, JOYPAD_8WAY_LEFT))
	{
		
	}
	else if(input_direction_pressed(0, JOYPAD_8WAY_RIGHT))
	{
		
	}
	else if(input_direction_pressed(0, JOYPAD_8WAY_UP))
	{

	}
	else if(input_direction_pressed(0, JOYPAD_8WAY_DOWN))
	{
		
	}

	return changed;
}

bool selection_grid_update(void *self)
{
    ui_element_selection_grid_t *grid = (ui_element_selection_grid_t *)self;
    bool changed = false;
	if(grid->base.focused)
	{
		if(!selection_grid_cur_cell_focused(grid))
		{
			ui_element_selection_grid_cell_t *lastCell = selection_grid_get_cur_cell(grid);
			if(input_direction_pressed(grid->navigationInput, JOYPAD_8WAY_UP))
			{
				grid->curY--;
				if(grid->curY < 0)
				{
					grid->curY = grid->height - 1;
				}
				changed = true;
			}
			else if(input_direction_pressed(grid->navigationInput, JOYPAD_8WAY_DOWN))
			{
				grid->curY++;
				if(grid->curY >= grid->height)
				{
					grid->curY = 0;
				}
				changed = true;
			}
			else if(input_direction_pressed(grid->navigationInput, JOYPAD_8WAY_LEFT))
			{
				grid->curX--;
				
				if(grid->curX < 0)
				{
					grid->curX = grid->width - 1;
				}
				changed = true;
			}
			else if(input_direction_pressed(grid->navigationInput, JOYPAD_8WAY_RIGHT))
			{
				grid->curX++;
				if(grid->curX >= grid->width)
				{
					grid->curX = 0;
				}
				changed = true;
			}

			if(lastCell != selection_grid_get_cur_cell(grid))
			{
				lastCell->base.selected = false;
				selection_grid_select_cur_cell(grid);
			}
		}
		else
		{
			if(input_buttons_pressed()->b)
			{
				selection_grid_defocus_cur_cell(grid);
				changed = true;
			}
			ui_element_selection_grid_cell_t *curCell = selection_grid_get_cur_cell(grid);
			if(selection_grid_cell_update(curCell))
			{
				changed = true;
			}
		}

		if(input_buttons_pressed()->a)
		{
			selection_grid_focus_cur_cell(grid);
			changed = true;
		}
	}
	return changed;
}