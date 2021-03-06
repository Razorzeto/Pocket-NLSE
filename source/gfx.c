#include <stdio.h>
#include <stdarg.h>

#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <sfil.h>

#include "common.h"
#include "ui.h"
#include "acres.h"
#include "gfx.h"

//menu textures
sf2d_texture* arrow;

//fonts
sftd_font* font;
sftd_font* font_bold;

static int fontheight = 11;

const int COLOR_WHITE = RGBA8(0xFF, 0xFF, 0xFF, 0xFF);
const int COLOR_BLACK = RGBA8(0x00, 0x00, 0x00, 0xFF);

pos_t current_pos;

void gfx_init(){
	sf2d_init();
	romfsInit();
	sftd_init();
	ui_init();
	acres_init();
	arrow = sfil_load_PNG_file("romfs:/arrow.png", SF2D_PLACE_RAM);
	font = sftd_load_font_file("romfs:/VeraMono.ttf");
	font_bold = sftd_load_font_file("romfs:/VeraMono-Bold.ttf");
}

void gfx_fini(){
	sftd_free_font(font);
	sftd_free_font(font_bold);
	sf2d_free_texture(arrow);
	acres_fini();
	ui_fini();
	sftd_fini();
	romfsExit();
	sf2d_fini();
}

int gfx_prompt(char* message, char* keymsg){
	if(keymsg==NULL)keymsg = "A = Yes, B = No.";

	while(aptMainLoop()){
		hidScanInput();

		if(hidKeysDown() & KEY_A)
			return 0;
		else if(hidKeysDown() & KEY_B)
			return 1;
	
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
			ui_frame(keymsg);
			sftd_draw_text_wrap(font, 0, fontheight*2, COLOR_WHITE, fontheight, 400, message);
		sf2d_end_frame();
		if(is3dsx){
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			sf2d_end_frame();
		}
		sf2d_swapbuffers();
	}

	return 0;
}

int gfx_prompt3(char* message, char* keymsg){
	while(aptMainLoop()){
		hidScanInput();

		if(hidKeysDown() & KEY_A)
			return 0;
		else if(hidKeysDown() & KEY_B)
			return 1;
		else if(hidKeysDown() & KEY_Y)
			return 2;
	
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
			ui_frame(keymsg);
			sftd_draw_text_wrap(font, 0, fontheight*2, COLOR_WHITE, fontheight, 400, message);
		sf2d_end_frame();
		if(is3dsx){
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			sf2d_end_frame();
		}
		sf2d_swapbuffers();
	}

	return 0;
}

void gfx_error(Result ret, char* file, int line){
	while(aptMainLoop()){
		hidScanInput();

		if(hidKeysDown() & KEY_A)
			break;

		sf2d_start_frame(GFX_TOP, GFX_LEFT);
			ui_frame("Press the A key to continue.");
			sftd_draw_textf(font, 0, fontheight*2, COLOR_WHITE, fontheight, "Error: 0x%X, %s:%d", ret, file, line);
		sf2d_end_frame();
		if(is3dsx){
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			sf2d_end_frame();
		}
		sf2d_swapbuffers();
	}
}
void gfx_displaymessage(char* msg, ...){
	char buffer[256];
	va_list args;
	va_start(args, msg);
	vsnprintf(buffer, 256, msg, args);
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		ui_frame("");
		sftd_draw_text_wrap(font, 0, fontheight*2, COLOR_WHITE, fontheight, 400, buffer);
	sf2d_end_frame();
	if(is3dsx){
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_end_frame();
	}
	sf2d_swapbuffers();
	va_end(args);
}

void gfx_waitmessage(char* msg, ...){
	char buffer[256];
	va_list args;
	va_start(args, msg);
	vsnprintf(buffer, 256, msg, args);
	while(aptMainLoop()){
		hidScanInput();

		if(hidKeysDown() & KEY_A)
			break;

		sf2d_start_frame(GFX_TOP, GFX_LEFT);
			ui_frame("Press the A key to continue.");
			sftd_draw_textf_wrap(font, 0, fontheight*2, COLOR_WHITE, fontheight, 400, "%s", buffer);
		sf2d_end_frame();
		if(is3dsx){
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			sf2d_end_frame();
		}
		sf2d_swapbuffers();
	}
	va_end(args);
}
