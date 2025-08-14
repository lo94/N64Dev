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

ui_element_label_t toolSelectTextElements[] = {
	{.x = 70, .y = 20, .visible = true, .text = "EDIT"},
	{.x = 220, .y = 20, .visible = true, .text = "PERFORM"},
	{.x = 370, .y = 20, .visible = true, .text = "SEQUENCE"}
};

section_element_t mainHeaderSection = { 
		.textElementCount = 3,
		.textElements = toolSelectTextElements,
		.curSelection = 0,
		.isSelected = true
};

ui_element_selection_grid_t drumEditorGrid;/*{
	.height = 14,
	.width = 6,
	.navigationInput = JOYPAD_2D_STICK,
	.base = { .x = 15, .y = 52, .enabled = true, .focused = true, .id = "drum_editor_grid" },
	.curX = 0,
	.curY = 0,
	.onCooldown = false
};
*/
char *drumEditorRowNames[] = {
    "Sample",
    "S-Rate",
    "Pitch",
    "Vol",
    "Pan",
    "V-ADSR",
    "F-ADSR",
    "Depth",
    "F-Cut",
    "F-Res",
    "Start",
    "End",
    "Loop",
    "Xfade"
};

ui_element_label_t editDrumLabelTextElements[] = {
    { .x = 10,  .y = 52,  .visible = true, .text = "Sample" },
    { .x = 10,  .y = 65,  .visible = true, .text = "S-Rate" },
    { .x = 10,  .y = 78,  .visible = true, .text = "Pitch" },
    { .x = 10,  .y = 91,  .visible = true, .text = "Vol" },
    { .x = 10,  .y = 104, .visible = true, .text = "Pan" },
    { .x = 10,  .y = 117, .visible = true, .text = "V-ADSR" },
    { .x = 10,  .y = 130, .visible = true, .text = "F-ADSR" },
    { .x = 10,  .y = 143, .visible = true, .text = "Depth" },
    { .x = 10,  .y = 156, .visible = true, .text = "F-Cut" },
    { .x = 10,  .y = 169, .visible = true, .text = "F-Res" },
    { .x = 10,  .y = 182, .visible = true, .text = "Start" },
    { .x = 10,  .y = 195, .visible = true, .text = "End" },
    { .x = 10,  .y = 208, .visible = true, .text = "Loop" },
    { .x = 10,  .y = 221, .visible = true, .text = "Xfade" },
    { .x = 80,  .y = 41,  .visible = true, .text = "Voice 1" },
    { .x = 150, .y = 41,  .visible = true, .text = "Voice 2" },
    { .x = 220, .y = 41,  .visible = true, .text = "Voice 3" },
    { .x = 290, .y = 41,  .visible = true, .text = "Voice 4" },
    { .x = 360, .y = 41,  .visible = true, .text = "Voice 5" },
    { .x = 430, .y = 41,  .visible = true, .text = "Voice 6" }
};

ui_element_selection_grid_t *editDrumBodySelectionGridElements[] = {&drumEditorGrid};

section_element_t editDrumBodySection = { 
		.textElementCount = 20,
		.textElements = editDrumLabelTextElements,
		.selectionGridElementCount = 1,
		.selectionGridElements = editDrumBodySelectionGridElements,
		.curSelection = -1,
		.isSelected = true
};

ui_element_label_t performSelectTextElements[] = {

};

section_element_t performBodySection = { 
		.textElementCount = 0,
		.textElements = performSelectTextElements,
		.curSelection = -1,
		.isSelected = true
};

ui_element_label_t sequenceSelectTextElements[] = {

};

section_element_t sequenceBodySection = { 
		.textElementCount = 0,
		.textElements = sequenceSelectTextElements,
		.curSelection = -1,
		.isSelected = true
};

ui_element_label_t editSelectTextElements[] = {
    { .x = 70,  .y = 30,  .visible = true, .text = "DRUM" },
    { .x = 220, .y = 30,  .visible = true, .text = " SYNTH" },
    { .x = 370, .y = 30,  .visible = true, .text = "  MOD" }
};

section_element_t *editBodyChildren[] = {&editDrumBodySection, &performBodySection, &sequenceBodySection};

section_element_t editBodySection = { 
		DEFAULT_UI_ELEMENT_SECTION_FIELDS,
		.textElementCount = 3,
		.textElements = editSelectTextElements,
		.curSelection = 0,
		.isSelected = true,
		.childCount = 3,
		.children = editBodyChildren
};

section_element_t *bodySections[] = { &editBodySection, &performBodySection, &sequenceBodySection };

int selectionGridCount = 1;
ui_element_selection_grid_t *selectionGrids[] = { &drumEditorGrid };

ui_menu_t menu[] = {
    
};

color_t COLOR_BKG_DARK;
color_t COLOR_BKG_LIGHT;
color_t MENU_BKG;
color_t MENU_END;
int color_mode;

void drum_editor_grid_init()
{
	/*
	ui_element_selection_grid_cell_t ***gridCells = malloc(drumEditorGrid.width * sizeof(ui_element_selection_grid_cell_t **));
	
	for(int i = 0; i < 6; i++)
	{
		gridCells[i] = malloc(drumEditorGrid.height * sizeof(ui_element_selection_grid_cell_t *));
		
		char sampleId[32];
		sprintf(sampleId, "drum_sample_%d", i);
		ui_element_selection_grid_cell_t *sample = malloc(sizeof(ui_element_selection_grid_cell_t));
		*sample = (ui_element_selection_grid_cell_t){ .base = {.id = strdup(sampleId), .x = 10, .y = 10} };
		gridCells[i][0] = sample;

		char srateId[32];
		sprintf(srateId, "drum_srate_%d", i);
		ui_element_selection_grid_cell_t *srate = malloc(sizeof(ui_element_selection_grid_cell_t));
		*srate = (ui_element_selection_grid_cell_t){ .base = {.id = strdup(srateId)} };
		gridCells[i][1] = srate;

		char pitchId[32];
		char pitchInputId[40];
		sprintf(pitchId, "drum_pitch_%d", i);
		sprintf(pitchInputId, "drum_pitch_input_%d", i);
		ui_element_selection_grid_cell_t *pitch = malloc(sizeof(ui_element_selection_grid_cell_t));
		ui_element_input_int_t *pitchInput = malloc(sizeof(ui_element_input_int_t));
		*pitchInput = (ui_element_input_int_t){ .base = {.id = strdup(pitchInputId), .x = 50, .y = 50 } };
		*pitch = (ui_element_selection_grid_cell_t){
			.base.id = strdup(pitchId),
			.intInputs = pitchInput,
			.intInputCount = 1
		};
		gridCells[i][2] = pitch;

		char volId[32];
		sprintf(volId, "drum_vol_%d", i);
		ui_element_selection_grid_cell_t *vol = malloc(sizeof(ui_element_selection_grid_cell_t));
		*vol = (ui_element_selection_grid_cell_t){ .base = {.id = strdup(volId)} };
		gridCells[i][3] = vol;

		char panId[32];
		sprintf(panId, "drum_pan_%d", i);
		ui_element_selection_grid_cell_t *pan = malloc(sizeof(ui_element_selection_grid_cell_t));
		*pan = (ui_element_selection_grid_cell_t){ .base = {.id = strdup(panId)} };
		gridCells[i][4] = pan;

		char vadsrId[32];
		sprintf(vadsrId, "drum_vadsr_%d", i);
		ui_element_selection_grid_cell_t *vadsr = malloc(sizeof(ui_element_selection_grid_cell_t));
		*vadsr = (ui_element_selection_grid_cell_t){ .base = {.id = strdup(vadsrId)} };
		gridCells[i][5] = vadsr;

		char fadsrId[32];
		sprintf(fadsrId, "drum_fadsr_%d", i);
		ui_element_selection_grid_cell_t *fadsr = malloc(sizeof(ui_element_selection_grid_cell_t));
		*fadsr = (ui_element_selection_grid_cell_t){ .base = {.id = strdup(fadsrId)} };
		gridCells[i][6] = fadsr;

		char depthId[32];
		sprintf(depthId, "drum_depth_%d", i);
		ui_element_selection_grid_cell_t *depth = malloc(sizeof(ui_element_selection_grid_cell_t));
		*depth = (ui_element_selection_grid_cell_t){ .base = {.id = strdup(depthId)} };
		gridCells[i][7] = depth;

		char fcutId[32];
		sprintf(fcutId, "drum_fcut_%d", i);
		ui_element_selection_grid_cell_t *fcut = malloc(sizeof(ui_element_selection_grid_cell_t));
		*fcut = (ui_element_selection_grid_cell_t){ .base = {.id = strdup(fcutId)} };
		gridCells[i][8] = fcut;

		char fresId[32];
		sprintf(fresId, "drum_fres_%d", i);
		ui_element_selection_grid_cell_t *fres = malloc(sizeof(ui_element_selection_grid_cell_t));
		*fres = (ui_element_selection_grid_cell_t){ .base = {.id = strdup(fresId)} };
		gridCells[i][9] = fres;

		char startId[32];
		sprintf(startId, "drum_start_%d", i);
		ui_element_selection_grid_cell_t *start = malloc(sizeof(ui_element_selection_grid_cell_t));
		*start = (ui_element_selection_grid_cell_t){ .base = {.id = strdup(startId)} };
		gridCells[i][10] = start;

		char endId[32];
		sprintf(endId, "drum_end_%d", i);
		ui_element_selection_grid_cell_t *end = malloc(sizeof(ui_element_selection_grid_cell_t));
		*end = (ui_element_selection_grid_cell_t){ .base = {.id = strdup(endId)} };
		gridCells[i][11] = end;

		char loopId[32];
		sprintf(loopId, "drum_loop_%d", i);
		ui_element_selection_grid_cell_t *loop = malloc(sizeof(ui_element_selection_grid_cell_t));
		*loop = (ui_element_selection_grid_cell_t){ .base = {.id = strdup(loopId)} };
		gridCells[i][12] = loop;

		char xfadeId[32];
		sprintf(xfadeId, "drum_xfade_%d", i);
		ui_element_selection_grid_cell_t *xfade = malloc(sizeof(ui_element_selection_grid_cell_t));
		*xfade = (ui_element_selection_grid_cell_t){ .base = {.id = strdup(xfadeId)} };
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
	*/
}

void menu_init()
{
    COLOR_BKG_DARK = RGBA32(0x21,0x21,0x21,0xFF);
    COLOR_BKG_LIGHT = RGBA32(0xA9,0xAF,0xD1,0xFF);
    MENU_BKG = RGBA32(0x17,0x43,0x4E,0xFF);
    MENU_END = RGBA32(0x5C,0x07,0x44,0xFF);
    color_mode = 0;


	drumEditorGrid = selection_grid("drum_editor_grid", 15, 52, 6, 14, JOYPAD_2D_STICK);
	selection_grid_cells_init(&drumEditorGrid, drumEditorRowNames, 14, (char *[]){"Voice"}, 1, 70, 70, 44, 13, 65, 11);
	for(int i = 0; i < drumEditorGrid.width; i++)
	{
		selection_grid_add_int_input_to_cell_by_index(&drumEditorGrid, i, 2, 0, -12, 12);
	}
	//drum_editor_grid_init();
    menu_render();
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
		if(selectionGrids[i]->base.update(selectionGrids[i]))
		{
			changed = true;
		}
	}
    return changed;
}

void menu_render_text_element(ui_element_label_t *textElement)
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

void menu_render_selection_grid_cell(ui_element_selection_grid_cell_t *cell)
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
		int styleId = cell->base.focused ? 1 : 0;
		debugf("%s\n", cell->intInputs[i]->base.id);
		char valueStr[16];
    	snprintf(valueStr, sizeof(valueStr), "%d", cell->intInputs[i]->value);
		rdpq_text_print(&(rdpq_textparms_t){
			.width = cell->base.width,
			.style_id = styleId
		}, 1, cell->base.x + ((cell->base.width/2) - 3), cell->base.y + (cell->base.height - 2), valueStr);
	}
}

void menu_render_selection_grid(ui_element_selection_grid_t *grid)
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
		for(int i = 0; i < s->childCount; i++)
		{
			if(i == s->curSelection)
			{
				menu_render_section(s->children[i]);
			}
			else
			{
				menu_defocus_section(s->children[i]);
			}
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