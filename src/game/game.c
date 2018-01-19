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

#include "player.h"
#include "camera.h"
#include "stage.h"

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"

// Bitmap font
static BITMAP* bmpFont;

// FPS string
static char fps[32];

// Game objects
static PLAYER player;
static CAMERA cam;

// Init game
static int game_init()
{
    ASSET_PACK* ass = get_global_assets();
    bmpFont = (BITMAP*)get_asset(ass,"font");

    init_player(ass);
    init_stage(ass);

    player = pl_create(vec3(0.0f,0.0f,0.0f));
    cam = create_camera(player.pos);

    return 0;
}


// Update game
static void game_update(float tm)
{
    snprintf(fps,32,"FPS: %d",(int)round(60.0f / tm));

    pl_update(&player,tm);
    update_stage(&player,tm);
    cam_follow_player(&cam,&player,tm);
}


// Draw game
static void game_draw()
{
    set_near_far_planes(0.025f,35.0f);

    clear_frame(0b10100010);
    
    // draw_bitmap((BITMAP*)get_asset(get_global_assets(),"test"),0,0,0);
    draw_text(bmpFont,(Uint8*)fps,32,8,8,-1,0,false);

    clear_triangle_buffer();

    tr_identity();
    use_camera(&cam);

    toggle_lighting(false);

    toggle_darkness(true);
    set_darkness(10.0f,35.0f);
    draw_stage(&cam);
    toggle_darkness(false);

    pl_draw(&player);

    tr_identity();
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
