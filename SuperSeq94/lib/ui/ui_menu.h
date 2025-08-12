#ifndef H_UI_MENU
#define H_UI_MENU

#include <stdbool.h>
#include <stdint.h>
#include <libdragon.h>

enum Section {
    EDIT,
    PERFORM,
    SEQUENCE
};

typedef struct ui_menu_s {
    const char * const text;
    void (*show)(); // Function pointer for the action
    short num_submenus;
    struct ui_menu_s *submenus;
} ui_menu_t;

void menu_init();
bool menu_update();
void menu_render();

#endif