#include "input.h"

joypad_buttons_t buttons_pressed;

void input_update()
{
	joypad_poll();
	buttons_pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);
}

bool input_direction_pressed(int navigationAxis, int direction)
{
	if(direction == JOYPAD_8WAY_UP) return buttons_pressed.d_up;
	else if(direction == JOYPAD_8WAY_DOWN) return buttons_pressed.d_down;
	else if(direction == JOYPAD_8WAY_LEFT) return buttons_pressed.d_left;
	else if(direction == JOYPAD_8WAY_RIGHT) return buttons_pressed.d_right;
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

joypad_buttons_t *input_buttons_pressed()
{
	return &buttons_pressed;
}
