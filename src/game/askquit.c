/// "Ask quit" dialogue (source)
/// (c) 2018 Jani Nykänen

#include "askquit.h"

#include "../engine/app.h"
#include "../engine/assets.h"

#include "../vpad.h"
#include "../global.h"

// Cursor pos
static int cursorPos;

// Bitmap font
static BITMAP* bmpFont;
// Cursor bitmap
static BITMAP* bmpCursor;


// Initialize
static int init_quit_screen()
{
    ASSET_PACK* ass = get_global_assets();
    bmpFont = (BITMAP*)get_asset(ass,"font");
    bmpCursor = (BITMAP*)get_asset(ass,"cursor");

    return 0;
}


// Quit input
static void quit_input(float tm)
{
    if( (vpad_get_delta().x > 0.1f && vpad_get_stick().x > 0.1f)
    || (vpad_get_delta().x < -0.1f && vpad_get_stick().x < -0.1f))
        cursorPos = !cursorPos;

    if(vpad_get_button(0) == PRESSED || vpad_get_button(1) == PRESSED)
    {
        if(cursorPos == 0)
        {
            app_terminate();
        }
        else
        {
            app_swap_scene("game");
        }
    }

    if(vpad_get_button(2) == PRESSED)
    {
        app_swap_scene("game");
    }
}


// Draw quit dialogue
static void quit_draw()
{
    fill_rect(128-64,96-24,128,48,0);
    fill_rect(128-64+1,96-24+1,128-2,48-2,255);
    fill_rect(128-64+2,96-24+2,128-4,48-4,0);

    draw_text(bmpFont,(Uint8*)"Leaving already?",64,128-64+4,96-24 + 8,-1,0,false);
    draw_text(bmpFont,(Uint8*)"   Yeah     Nope",64,128-64+4,96,-1,0,false);

    draw_bitmap(bmpCursor,128-64+12 + 60*cursorPos,92,0);
}


// Scene swapped
static void quit_on_swap()
{
    cursorPos = 1;
}


// Destroy the scene (unused)
static void quit_destroy()
{

}


// Get quit scene
SCENE get_quit_scene()
{
    SCENE s = (SCENE){init_quit_screen,quit_input,quit_draw, quit_destroy, quit_on_swap};
    set_scene_name(&s,"quit");

    return s;
}