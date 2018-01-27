/// Main file (source)
/// (c) 2018 Jani Nykänen

#define SDL_MAIN_HANDLED

#include "global.h"
#include "game/game.h"
#include "game/askquit.h"
#include "intro/intro.h"

#include "engine/app.h"
#include "engine/list.h"
#include "engine/assets.h"
#include "engine/config.h"

#include "stdlib.h"

// Main function
int main(int argc, char** argv)
{
    // Set scenes
    SCENE scenes[] = {
        get_global_scene(),
        get_quit_scene(),  
        get_game_scene(),
        get_intro_scene(),
    };
    int sceneCount = 4;

    // Load config
    CONFIG c;
    if(read_config(&c,"config.list") != 0)
    {
        return 1;
    }

    return app_run(scenes,sceneCount,c);
}
