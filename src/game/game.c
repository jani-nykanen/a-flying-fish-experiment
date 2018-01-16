/// Game scene (source)
/// (c) 2018 Jani Nykänen

#include "game.h"

#include "../engine/app.h"
#include "../engine/graphics.h"
#include "../engine/bitmap.h"
#include "../engine/controls.h"
#include "../engine/assets.h"
#include "../engine/transform.h"
#include "../engine/mesh.h"

#include "../vpad.h"
#include "../global.h"

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"

// Bitmap font
static BITMAP* bmpFont;

// Fish mesh
static MESH* mFish;
// Fish texture
static BITMAP* bmpFish;

// Angle
static float angle;

// FPS string
static char fps[32];

// Init game
static int game_init()
{
    ASSET_PACK* ass = get_global_assets();
    bmpFont = (BITMAP*)get_asset(ass,"font");

    bmpFish = (BITMAP*)get_asset(ass,"fish_tex");
    mFish = (MESH*)get_asset(ass,"fish");

    angle = 0.0f;

    return 0;
}


// Update game
static void game_update(float tm)
{
    angle += 0.025f * tm;
    snprintf(fps,32,"FPS: %d",(int)round(60.0f / tm));
}


// Draw game
static void game_draw()
{
    clear_frame(0b10110110);
    
    draw_bitmap((BITMAP*)get_asset(get_global_assets(),"test"),0,0,0);
    draw_text(bmpFont,(Uint8*)fps,32,8,8,-1,0,false);

    clear_triangle_buffer();

    tr_identity();
    tr_translate(0.0f,0,4.25f);
    tr_rotate_model(angle/2,angle,angle);
    tr_scale_model(1.0f,1.0f,1.0f);

    bind_texture(bmpFish);


    toggle_lighting(false);
    set_ligthing(vec3(0,0,-1),0.75f);

    draw_mesh(mFish);
    draw_triangle_buffer();

}


// Destroy game
static void game_destroy()
{

}


// Scene swapped
static void game_on_swap()
{

}


// Get game scene
SCENE get_game_scene()
{
    // Set scene functions
    SCENE s = (SCENE){game_init,game_update,game_draw,game_destroy,game_on_swap};

    // Set scene name
    set_scene_name(&s,"game");

    return s;
}
