#ifndef H_INPUT
#define H_INPUT

#include <stdbool.h>
#include <stdint.h>
#include <libdragon.h>

joypad_buttons_t buttons_pressed;

void input_update();
bool input_direction_pressed(int navigationAxis, int direction);
joypad_buttons_t *input_buttons_pressed();

#endif