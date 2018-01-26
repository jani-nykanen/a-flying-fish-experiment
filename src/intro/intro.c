/// Intro scene (source)
/// (c) 2018 Jani Nykänen

#include "intro.h"

#include "../engine/app.h"
#include "../engine/assets.h"
#include "../engine/graphics.h"
#include "../engine/transform.h"
#include "../vpad.h"

#include "../global.h"

// Phase
static int phase;
// Phase timer
static float timer;

// Creator bitmap
static BITMAP* bmpCreator;
// Avatar bitmap
static BITMAP* bmpAvatar;
// Cube mesh
static MESH* mCube;

// Logo bitmap
static BITMAP* bmpLogo;
// Fish bitmap
static BITMAP* bmpFish;
// Fish mesh
static MESH* mFish;

// Bitmap font
static BITMAP* bmpFont;

// Avatar cube angle
static float angle;
// Press any key -timer
static float pressTimer;

// Initialize
static int intro_init()
{
    ASSET_PACK* ass = get_global_assets();

    phase = 0;
    timer = 0;

    bmpCreator = (BITMAP*)get_asset(ass,"creator");
    bmpAvatar = (BITMAP*)get_asset(ass,"avatar");
    mCube = (MESH*)get_asset(ass,"cube");

    bmpLogo = (BITMAP*)get_asset(ass,"logo");
    bmpFish = (BITMAP*)get_asset(ass,"fish_tex");
    mFish = (MESH*)get_asset(ass,"fish");

    bmpFont = (BITMAP*)get_asset(ass,"font");

    return 0;
}


// Update
static void intro_update(float tm)
{
    angle += 0.0125 * tm;
    if(phase == 0)
    {
        timer += 1.0f * tm;
        if(timer >= 240.0f)
        {
            ++ phase;
            timer = 0.0f;
        }

    }
    else if(phase == 1)
    {
        pressTimer += 1.0f * tm;
        if(pressTimer >= 30.0f) 
            pressTimer -= 30.0f;

        if(timer < 60.0f || timer >= 180.0f)
        {
            timer += 1.0f * tm;
        }
        
        if(timer >= 240.0f)
        {
            app_swap_scene("game");
            phase = 2;
            timer = 0.0f;
            return;
        }
        
        if(timer >= 60.0f && timer < 180.0f && any_pressed())
        {
            timer = 180.0f + 0.01f;
        }
    }
    else if(phase == 2)
    {
        if(timer < 60.0f || timer >= 180.0f)
        {
            timer += 1.0f * tm;
        }
        
        if(timer >= 240.0f)
        {
            app_terminate();
            return;
        }
        
        if(timer >= 60.0f && timer < 180.0f && any_pressed())
        {
            timer = 180.0f + 0.01f;
        }
    }

    
}


// Draw
static void intro_draw()
{
    clear_frame(0);

    if(phase == 0)
    {
        draw_bitmap(bmpCreator,128 - bmpCreator->w / 2,112,0);

        float y = -1;
        if(timer < 60.0f)
            y = -2 + timer/60.0f;
        else if(timer > 180.0f)
            y = -2 + ((240.0f-timer)/60.0f);

        tr_identity();
        tr_translate(0,y,5);
        tr_rotate_model(angle,angle/2,-angle);

        clear_triangle_buffer();
        bind_texture(bmpAvatar);
        draw_mesh(mCube);
        draw_triangle_buffer();

    }
    else if(phase == 1)
    {
        draw_bitmap(bmpLogo,128-bmpLogo->w/2,8,0);

        tr_identity();
        tr_translate(0,0.5f,4);
        
        float scale = 1.0f;
        if(timer < 60.0f)
            scale = timer/60.0f;
        tr_scale_model(scale,scale,scale);
        

        tr_rotate_model(-M_PI/2 + sin(angle)*M_PI/6,angle,M_PI/2);

        clear_triangle_buffer();
        bind_texture(bmpFish);
        draw_mesh(mFish);
        draw_triangle_buffer();

        if(timer >= 60.0f && timer < 180.0f && (int)floor(angle*80) % 60 < 30)
        {
            draw_text(bmpFont,(Uint8*)"PRESS ANY KEY",13,128,160,-1,0,true);
        }

        draw_text(bmpFont,(Uint8*)"(c)2018 Jani Nyk*nen",20,128,182,-1,0,true);
    }
    else if(phase == 2)
    {
        draw_bitmap(bmpLogo,128-bmpLogo->w/2,96-bmpLogo->h,0);
    }

    if(timer < 60.0f)
    {
        int amount = (int)floor((1.0f-timer/60.0f) * MAX_DARKNESS_VALUE);
        darken_frame(amount);
    }
    else if(timer > 180.0f)
    {
        int amount = (int)floor((timer-180.0f)/60.0f * MAX_DARKNESS_VALUE);
        darken_frame(amount);
    }
}


// Get intro scene
SCENE get_intro_scene()
{
    // Set scene functions
    SCENE s = (SCENE){intro_init,intro_update,intro_draw,NULL,NULL};

    // Set scene name
    set_scene_name(&s,"intro");

    return s;
}