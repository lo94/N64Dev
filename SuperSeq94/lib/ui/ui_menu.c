#include "ui_menu.h"
#include "ui_elements.h"

/*
text_element_t
input_element_t
button_element_t
*/
#define NAVIGATION_COOLDOWN 500000 // 500ms

joypad_inputs_t lastInput;
joypad_inputs_t curInput;

text_element_t toolSelectTextElements[] = {
	{70, 20, true, "EDIT"},
	{220, 20, true, "PERFORM"},
	{370, 20, true, "SEQUENCE"}
};

section_element_t mainHeaderSection = { 
		.textElementCount = 3,
		.textElements = toolSelectTextElements,
		.curSelection = 0,
		.isSelected = true
};

selection_grid_t drumEditorGrid = {
	.height = 14,
	.width = 6,
	.navigationInput = JOYPAD_2D_STICK,
	.base = { .x = 15, .y = 52, .enabled = true, .focused = true, .id = "drum_editor_grid" },
	.curX = 0,
	.curY = 0,
	.onCooldown = false
};

text_element_t editDrumLabelTextElements[] = {
	{10, 52, true, "Sample"},
	{10, 65, true, "S-Rate"},
	{10, 78, true, "Pitch"},
	{10, 91, true, "Vol"},
	{10, 104, true, "Pan"},
	{10, 117, true, "V-ADSR"},
	{10, 130, true, "F-ADSR"},
	{10, 143, true, "Depth"},
	{10, 156, true, "F-Cut"},
	{10, 169, true, "F-Res"},
	{10, 182, true, "Start"},
	{10, 195, true, "End"},
	{10, 208, true, "Loop"},
	{10, 221, true, "Xfade"},
	{80, 41, true, "Voice 1"},
	{150, 41, true, "Voice 2"},
	{220, 41, true, "Voice 3"},
	{290, 41, true, "Voice 4"},
	{360, 41, true, "Voice 5"},
	{430, 41, true, "Voice 6"}
};

selection_grid_t *editDrumBodySelectionGridElements[] = {&drumEditorGrid};

section_element_t editDrumBodySection = { 
		.textElementCount = 20,
		.textElements = editDrumLabelTextElements,
		.selectionGridElementCount = 1,
		.selectionGridElements = editDrumBodySelectionGridElements,
		.curSelection = -1,
		.isSelected = true
};

text_element_t performSelectTextElements[] = {

};

section_element_t performBodySection = { 
		.textElementCount = 0,
		.textElements = performSelectTextElements,
		.curSelection = -1,
		.isSelected = true
};

text_element_t sequenceSelectTextElements[] = {

};

section_element_t sequenceBodySection = { 
		.textElementCount = 0,
		.textElements = sequenceSelectTextElements,
		.curSelection = -1,
		.isSelected = true
};

text_element_t editSelectTextElements[] = {
	{70, 30, true, "DRUM"},
	{220, 30, true, " SYNTH"},
	{370, 30, true, "  MOD"}
};

section_element_t *editBodyChildren[] = {&editDrumBodySection, &performBodySection, &sequenceBodySection};

section_element_t editBodySection = { 
		.textElementCount = 3,
		.textElements = editSelectTextElements,
		.curSelection = 0,
		.isSelected = true,
		.childCount = 3,
		.children = editBodyChildren
};

section_element_t *bodySections[] = { &editBodySection, &performBodySection, &sequenceBodySection };

int selectionGridCount = 1;
selection_grid_t *selectionGrids[] = { &drumEditorGrid };

ui_menu_t menu[] = {
    
};

color_t COLOR_BKG_DARK;
color_t COLOR_BKG_LIGHT;
color_t MENU_BKG;
color_t MENU_END;
int color_mode;

void drum_editor_grid_init()
{
	selection_grid_cell_t ***gridCells = malloc(6 * sizeof(selection_grid_cell_t **));
	
	for(int i = 0; i < 6; i++)
	{
		gridCells[i] = malloc(14 * sizeof(selection_grid_cell_t *));
		
		char sampleId[32];
		sprintf(sampleId, "drum_sample_%d", i);
		selection_grid_cell_t *sample = malloc(sizeof(selection_grid_cell_t));
		*sample = (selection_grid_cell_t){ .base = {.id = strdup(sampleId), .x = 10, .y = 10} };
		gridCells[i][0] = sample;

		char srateId[32];
		sprintf(srateId, "drum_srate_%d", i);
		selection_grid_cell_t *srate = malloc(sizeof(selection_grid_cell_t));
		*srate = (selection_grid_cell_t){ .base = {.id = strdup(srateId)} };
		gridCells[i][1] = srate;

		char pitchId[32];
		char pitchInputId[40];
		sprintf(pitchId, "drum_pitch_%d", i);
		sprintf(pitchInputId, "drum_pitch_input_%d", i);
		selection_grid_cell_t *pitch = malloc(sizeof(selection_grid_cell_t));
		int_input_element_t *pitchInput = malloc(sizeof(int_input_element_t));
		*pitchInput = (int_input_element_t){ .base = {.id = strdup(pitchInputId), .x = 50, .y = 50 } };
		*pitch = (selection_grid_cell_t){
			.base.id = strdup(pitchId),
			.intInputs = pitchInput,
			.intInputCount = 1
		};
		gridCells[i][2] = pitch;

		char volId[32];
		sprintf(volId, "drum_vol_%d", i);
		selection_grid_cell_t *vol = malloc(sizeof(selection_grid_cell_t));
		*vol = (selection_grid_cell_t){ .base = {.id = strdup(volId)} };
		gridCells[i][3] = vol;

		char panId[32];
		sprintf(panId, "drum_pan_%d", i);
		selection_grid_cell_t *pan = malloc(sizeof(selection_grid_cell_t));
		*pan = (selection_grid_cell_t){ .base = {.id = strdup(panId)} };
		gridCells[i][4] = pan;

		char vadsrId[32];
		sprintf(vadsrId, "drum_vadsr_%d", i);
		selection_grid_cell_t *vadsr = malloc(sizeof(selection_grid_cell_t));
		*vadsr = (selection_grid_cell_t){ .base = {.id = strdup(vadsrId)} };
		gridCells[i][5] = vadsr;

		char fadsrId[32];
		sprintf(fadsrId, "drum_fadsr_%d", i);
		selection_grid_cell_t *fadsr = malloc(sizeof(selection_grid_cell_t));
		*fadsr = (selection_grid_cell_t){ .base = {.id = strdup(fadsrId)} };
		gridCells[i][6] = fadsr;

		char depthId[32];
		sprintf(depthId, "drum_depth_%d", i);
		selection_grid_cell_t *depth = malloc(sizeof(selection_grid_cell_t));
		*depth = (selection_grid_cell_t){ .base = {.id = strdup(depthId)} };
		gridCells[i][7] = depth;

		char fcutId[32];
		sprintf(fcutId, "drum_fcut_%d", i);
		selection_grid_cell_t *fcut = malloc(sizeof(selection_grid_cell_t));
		*fcut = (selection_grid_cell_t){ .base = {.id = strdup(fcutId)} };
		gridCells[i][8] = fcut;

		char fresId[32];
		sprintf(fresId, "drum_fres_%d", i);
		selection_grid_cell_t *fres = malloc(sizeof(selection_grid_cell_t));
		*fres = (selection_grid_cell_t){ .base = {.id = strdup(fresId)} };
		gridCells[i][9] = fres;

		char startId[32];
		sprintf(startId, "drum_start_%d", i);
		selection_grid_cell_t *start = malloc(sizeof(selection_grid_cell_t));
		*start = (selection_grid_cell_t){ .base = {.id = strdup(startId)} };
		gridCells[i][10] = start;

		char endId[32];
		sprintf(endId, "drum_end_%d", i);
		selection_grid_cell_t *end = malloc(sizeof(selection_grid_cell_t));
		*end = (selection_grid_cell_t){ .base = {.id = strdup(endId)} };
		gridCells[i][11] = end;

		char loopId[32];
		sprintf(loopId, "drum_loop_%d", i);
		selection_grid_cell_t *loop = malloc(sizeof(selection_grid_cell_t));
		*loop = (selection_grid_cell_t){ .base = {.id = strdup(loopId)} };
		gridCells[i][12] = loop;

		char xfadeId[32];
		sprintf(xfadeId, "drum_xfade_%d", i);
		selection_grid_cell_t *xfade = malloc(sizeof(selection_grid_cell_t));
		*xfade = (selection_grid_cell_t){ .base = {.id = strdup(xfadeId)} };
		gridCells[i][13] = xfade;
	}

	// TODO go through and set each cell's x and y positions
	for(int i = 0; i < drumEditorGrid.width; i++)
	{
		for(int j = 0; j < drumEditorGrid.height; j++)
		{
			gridCells[i][j]->base.x = 70 + (i * 70);
			gridCells[i][j]->base.y = 44 + (j * 13);
			gridCells[i][j]->base.width = 65;
			gridCells[i][j]->base.height = 11;
			gridCells[i][j]->base.visible = true;
			gridCells[i][j]->base.enabled = true;
			if(i == 0 && j == 0)
			{
				gridCells[i][j]->base.selected = true;
			}
			else
			{
				gridCells[i][j]->base.selected = false;
			}
			//gridCells[i][j]->base.backgroundColor = RGBA32(0x00, 0x00, 0xFF, 0xFF);
		}
	}
		
	drumEditorGrid.gridCells = gridCells;
}

void menu_init()
{
    COLOR_BKG_DARK = RGBA32(0x21,0x21,0x21,0xFF);
    COLOR_BKG_LIGHT = RGBA32(0xA9,0xAF,0xD1,0xFF);
    MENU_BKG = RGBA32(0x17,0x43,0x4E,0xFF);
    MENU_END = RGBA32(0x5C,0x07,0x44,0xFF);
    color_mode = 0;
	drum_editor_grid_init();
    menu_render();
}

bool direction_pressed(int navigationAxis, int direction)
{
	joypad_buttons_t dir = joypad_get_buttons_pressed(JOYPAD_PORT_1);
	if(direction == JOYPAD_8WAY_UP) return dir.d_up;
	else if(direction == JOYPAD_8WAY_DOWN) return dir.d_down;
	else if(direction == JOYPAD_8WAY_LEFT) return dir.d_left;
	else if(direction == JOYPAD_8WAY_RIGHT) return dir.d_right;
	return false;
	//return direction == dir;
	/*
	if(navigationAxis == JOYPAD_2D_STICK)
	{
		if(direction == JOYPAD_UP && joypad2d.up) return true;
		else if(direction == JOYPAD_DOWN && joypad2d.down) return true;
		else if(direction == JOYPAD_LEFT && joypad2d.left) return true;
		else if(direction == JOYPAD_RIGHT && joypad2d.right) return true;
	}
	else if(navigationAxis == JOYPAD_DPAD)
	{
		if(direction == JOYPAD_UP && joypadButtons.d_up) return true;
		else if(direction == JOYPAD_DOWN && joypadButtons.d_down) return true;
		else if(direction == JOYPAD_LEFT && joypadButtons.d_left) return true;
		else if(direction == JOYPAD_RIGHT && joypadButtons.d_right) return true;
	}
	*/
}

selection_grid_cell_t* selection_grid_get_cur_cell(selection_grid_t *grid)
{
	if(grid->curX < 0 || grid->curY < 0 || grid->curX >= grid->width || grid->curY >= grid->height)
		return NULL;
	return grid->gridCells[grid->curX][grid->curY];
}

void selection_grid_select_cur_cell(selection_grid_t *grid)
{
	selection_grid_cell_t *cell = selection_grid_get_cur_cell(grid);
	if(cell)
	{
		cell->base.selected = true;
	}
}

bool menu_update()
{
    bool changed = false;
	joypad_buttons_t buttons = joypad_get_buttons_pressed(JOYPAD_PORT_1);
	lastInput = curInput;
	curInput = joypad_get_inputs(JOYPAD_PORT_1);
	
	if(buttons.l && mainHeaderSection.curSelection > 0)
	{
		mainHeaderSection.curSelection--;
		changed = true;
	}
	else if(buttons.r && mainHeaderSection.curSelection < 2)
	{
		mainHeaderSection.curSelection++;
		changed = true;
	}

    if(buttons.c_left && mainHeaderSection.curSelection == EDIT && editBodySection.curSelection > 0)
    {
        editBodySection.curSelection--;
        changed = true;
    }
    if(buttons.c_right && mainHeaderSection.curSelection == EDIT && editBodySection.curSelection < 2)
    {
        editBodySection.curSelection++;
        changed = true;
    }
	
	for(int i = 0; i < selectionGridCount; i++)
	{
		if(selectionGrids[i]->base.focused)
		{
			selection_grid_cell_t *lastCell = selection_grid_get_cur_cell(selectionGrids[i]);
			if(direction_pressed(selectionGrids[i]->navigationInput, JOYPAD_8WAY_UP))
			{
				selectionGrids[i]->curY--;
				if(selectionGrids[i]->curY < 0)
				{
					selectionGrids[i]->curY = selectionGrids[i]->height - 1;
				}
				changed = true;
			}
			else if(direction_pressed(selectionGrids[i]->navigationInput, JOYPAD_8WAY_DOWN))
			{
				selectionGrids[i]->curY++;
				if(selectionGrids[i]->curY >= selectionGrids[i]->height)
				{
					selectionGrids[i]->curY = 0;
				}
				changed = true;
			}
			else if(direction_pressed(selectionGrids[i]->navigationInput, JOYPAD_8WAY_LEFT))
			{
				selectionGrids[i]->curX--;
				
				if(selectionGrids[i]->curX < 0)
				{
					selectionGrids[i]->curX = selectionGrids[i]->width - 1;
				}
				changed = true;
			}
			else if(direction_pressed(selectionGrids[i]->navigationInput, JOYPAD_8WAY_RIGHT))
			{
				selectionGrids[i]->curX++;
				if(selectionGrids[i]->curX >= selectionGrids[i]->width)
				{
					selectionGrids[i]->curX = 0;
				}
				changed = true;
			}

			if(lastCell != selection_grid_get_cur_cell(selectionGrids[i]))
			{
				lastCell->base.selected = false;
				selection_grid_select_cur_cell(selectionGrids[i]);
			}
		}
		
	}
		

    return changed;
}

void menu_render_text_element(text_element_t *textElement)
{
	int styleId = textElement->base.selected ? 1 : 0;
	rdpq_text_print(&(rdpq_textparms_t){
		.width = 100,
		.style_id = styleId
	}, 1, textElement->x, textElement->y, textElement->text);
}

void menu_render_rectangle(int x, int y, int width, int height, color_t color)
{
	rdpq_set_mode_fill(color);
	rdpq_fill_rectangle(x, y, width, height);
}

void menu_render_selection_grid_cell(selection_grid_cell_t *cell)
{
	
	if(cell->base.selected)
	{
		// If selected first render a slightly larger rectangle behind it as a border
		menu_render_rectangle(cell->base.x - 1, cell->base.y - 1, cell->base.x + cell->base.width + 1, cell->base.y + cell->base.height + 1, RGBA32(255, 255, 0, 255));
	}
	//debugf("RENDERING CELL %s: X1:%d Y1:%d w:%d h:%d \n", cell->base.id, cell->base.x, cell->base.y, cell->base.width, cell->base.height);
	menu_render_rectangle(cell->base.x, cell->base.y, cell->base.x + cell->base.width, cell->base.y + cell->base.height, MENU_BKG);
	
	// Render the square for the cell
	// Highlight the cell edge if selected

	//cell->base.selected = (cell->base.id == drumEditorGrid.gridCells[drumEditorGrid.curSelection / drumEditorGrid.width][drumEditorGrid.curSelection % drumEditorGrid.width]->base.id);
	//menu_render_text_element(&cell->base);
	
	for(int i = 0; i < cell->intInputCount; i++)
	{
		
	}
}

void menu_render_selection_grid(selection_grid_t *grid)
{
	for(int i = 0; i < grid->width; i++)
	{
		for(int j = 0; j < grid->height; j++)
		{
			//debugf("BEFORE GRID CELL RENDER: %d, %d\n", i, j);
			menu_render_selection_grid_cell(grid->gridCells[i][j]);
		}
	}
}

void menu_render_section(section_element_t *s)
{
	// TODO update this, remove this loop, should only call render on children
	// Text menu selection should be a selection_grid_t
    for(int i = 0; i < s->textElementCount; i++)
	{
        s->textElements[i].base.selected = s->curSelection == i ? true : false;
        menu_render_text_element(&s->textElements[i]);
	}
	for(int i = 0; i < s->selectionGridElementCount; i++)
	{
		s->selectionGridElements[i]->base.focused = true;
        menu_render_selection_grid(s->selectionGridElements[i]);
	}
	if(s->childCount > 0)
	{
		menu_render_section(s->children[s->curSelection]);
	}
}

void menu_defocus_section(section_element_t *s)
{
	//s->isSelected = false;
	//s->curSelection = -1;
	for(int i = 0; i < s->textElementCount; i++)
	{
		//s->textElements[i].base.selected = false;
	}
	for(int i = 0; i < s->selectionGridElementCount; i++)
	{
		s->selectionGridElements[i]->base.focused = false;
	}
	if(s->childCount > 0)
	{
		for(int i = 0; i < s->childCount; i++)
		{
			menu_defocus_section(s->children[i]);
		}
	}
}



void menu_render()
{
	surface_t *disp = display_get();
	rdpq_attach(disp, NULL);

	rdpq_clear(color_mode & 1 ? COLOR_BKG_LIGHT : COLOR_BKG_DARK);
	
	rdpq_set_mode_fill(MENU_BKG);
	rdpq_fill_rectangle(0, 0, 512, 20);
	for (int i=0; i<4; i++) {
		color_t c;
		c.r = MENU_BKG.r * (8-i) / 8;
		c.g = MENU_BKG.g * (8-i) / 8;
		c.b = MENU_BKG.b * (8-i) / 8;
		c.a = 0xFF;
		rdpq_set_fill_color(c);
		rdpq_fill_rectangle(0, 20+((3*i)+1), 512, 20+((3*i)+3));
	}

	rdpq_set_fill_color(MENU_END);
	
	menu_render_section(&mainHeaderSection);
	for(int i = 0; i < 3; i++)
	{
		if(i == mainHeaderSection.curSelection)
		{
    		menu_render_section(bodySections[mainHeaderSection.curSelection]);
		}
		else
		{
			menu_defocus_section(bodySections[i]);
		}
	}
	//menu_render_rectangle(67, 187, 367, 487, RGBA32(0xFF, 0xFF, 0xFF, 0xFF));
	rdpq_detach_show();
}