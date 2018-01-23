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

#include "../lib/parseword.h"

#include "../vpad.h"
#include "../global.h"

#include "player.h"
#include "camera.h"
#include "stage.h"

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"

// Maximum fish count
#define MAX_FISH 16

// Bitmap font
static BITMAP* bmpFont;

// FPS string
static char fps[32];

// Game objects
static PLAYER player;
static CAMERA cam;

// Fish
static PLAYER fish[MAX_FISH];
static int fishCount;


// Add NPC fish to the game
static int add_fish()
{
    // Read layout file
    WORDDATA* wd = parse_file("assets/layout.txt");
    if(wd == NULL)
        return 1;

    int i = 0;
    bool begun = false;
    char* w;

    VEC3 pos = vec3(0,0,0);

    for(; i < wd->wordCount; ++ i)
    {
        w = get_word(wd,i);
        if(strcmp(w,"fish") == 0)
        {
            begun = true;
            continue;   
        }
        if(!begun) continue;

        if(strcmp(w,"pos") == 0)
        {
           pos.x = strtof(get_word(wd,i+1), NULL);
           pos.y = strtof(get_word(wd,i+2), NULL);
           pos.z = strtof(get_word(wd,i+3), NULL);
        }
        else if(strcmp(w,"add") == 0)
        {
            fish[fishCount] = pl_create(pos);
            fish[fishCount].control = false;
            

            float mod = (float)(rand() % 100) / 100.0f * 0.5f + 0.75f;
            fish[fishCount].maxSpeed.x *= mod;
            fish[fishCount].maxSpeed.y *= mod;
            fish[fishCount].maxSpeed.z *= mod;

            ++ fishCount;
        }
    }

    return 0;
}


// Init game
static int game_init()
{
    ASSET_PACK* ass = get_global_assets();
    bmpFont = (BITMAP*)get_asset(ass,"font");

    init_player(ass);
    if(init_stage(ass) == 1)
        return 1;

    player = pl_create(vec3(0.0f,0.0f,0.0f));
    cam = create_camera(player.pos);

    fishCount = 0;
    if(add_fish() == 1)
        return 1;

    return 0;
}


// Update game
static void game_update(float tm)
{
    snprintf(fps,32,"FPS: %d",(int)round(60.0f / tm));

    pl_update(&player,tm);
    stage_player_collision(&player,tm);
    cam_follow_player(&cam,&player,tm);

    // Update fish
    int i = 0;
    int i2 = 0;
    for(; i < fishCount; ++ i)
    {
        pl_update(&fish[i],tm);
        stage_player_collision(&fish[i],tm);
        player_to_player_collision(&player,&fish[i],tm);
        for(i2 = 0; i2 < fishCount; ++ i2)
        {
            if(i != i2)
                player_to_player_collision(&fish[i],&fish[i2],tm);
        }
    }

    if(get_key_state((int)SDL_SCANCODE_ESCAPE) == PRESSED)
    {
        if(ask_to_quit() == 1) app_terminate();
    }
}


// Draw game
static void game_draw()
{
    set_near_far_planes(0.025f,35.0f);

    clear_frame(0b10100010);

    clear_triangle_buffer();

    tr_identity();
    use_camera(&cam);

    toggle_lighting(false);

    draw_stage(&cam);

    // Draw other fish
    int i = 0;
    for(; i < fishCount; ++ i)
    {
        pl_draw(&fish[i]);
    }

    toggle_darkness(false);

    // Draw player
    pl_draw(&player);

    tr_identity();
    draw_triangle_buffer();

    draw_text(bmpFont,(Uint8*)fps,32,8,8,-1,0,false);

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
