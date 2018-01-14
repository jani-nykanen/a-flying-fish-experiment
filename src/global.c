/// Global scene (source)
/// (c) 2018 Jani Nykänen

#define SDL_MAIN_HANDLED


#include "global.h"

#include "vpad.h"
#include "engine/frame.h"
#include "engine/graphics.h"
#include "engine/assets.h"

#include "stdlib.h"
#include "math.h"
#include "stdio.h"

/// Global asset pack
static ASSET_PACK* globalAssets;


/// Initialize global scene
static int global_init()
{
    // Init vpad
    vpad_init();
    vpad_add_button(0,(int)SDL_SCANCODE_Z,2);
    vpad_add_button(1,(int)SDL_SCANCODE_X,0);
    vpad_add_button(2,(int)SDL_SCANCODE_C,3);
    vpad_add_button(3,(int)SDL_SCANCODE_SPACE,1);
    vpad_add_button(4,(int)SDL_SCANCODE_RETURN,7);
    vpad_add_button(5,(int)SDL_SCANCODE_ESCAPE,6);

    set_alpha(170);

    // Load global assets
    globalAssets = load_asset_pack("assets/global.ass");
    if(globalAssets == NULL)
    {
        return 1;
    }

    return 0;
}


/// Update global scene
static void global_update(float tm)
{
    vpad_update();
}


/// Return the global scene
SCENE get_global_scene()
{
    // Set scene functions
    SCENE s = (SCENE){global_init,global_update,NULL,NULL};
        
    // Set scene name
    set_scene_name(&s,"global");

    return s; 

}


// Get asssets
ASSET_PACK* get_global_assets()
{
    return globalAssets;
}