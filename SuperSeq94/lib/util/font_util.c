#include "font_util.h"

void fonts_init()
{
	int color_mode = 0;

	rdpq_font_t *fnt1 = rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_MONO);
	rdpq_text_register_font(1, fnt1);

	rdpq_font_style(fnt1, 0, &(rdpq_fontstyle_t){
		.color = RGBA32(255, 255, 255, 255),
		.outline_color = RGBA32(0, 0, 0, 255),
	});

	rdpq_font_style(fnt1, 1, &(rdpq_fontstyle_t){
		.color = RGBA32(255, 255, 0, 255),
		.outline_color = RGBA32(0, 0, 0, 255),
	});
}