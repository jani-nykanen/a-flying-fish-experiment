/**
 * Configuration
 * (source file)
 *
 * @author Jani Nykänen
 * @version 1.0.0
 */

#include "config.h"

#include "SDL2/SDL.h"

#include "../lib/parseword.h"
#include "error.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

// Key
static char* key;
// Value
static char* value;


// Read config
int read_config(CONFIG* c, const char* path)
{
    // Open config file
    WORDDATA* w = parse_file(path);
    if(w == NULL)
    {
        char err[256];
        snprintf(err,256,"Failed to open and/or parse a file in %s",path);
        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR,"Error!",err,NULL);
        return 1;
    }

    // Read words
    int count = 0;
    int i = 0;
    for(; i < w->wordCount; ++ i)
    {
        if(count == 0)
            key = get_word(w,i);
        else
        {
            value = get_word(w,i);

            if(strcmp(key,"title") == 0)
            {
                strcpy(c->title,value);
            }
            else if(strcmp(key,"window_width") == 0)
            {
                c->winWidth = (int)strtol(value,NULL,10);
            }
            else if(strcmp(key,"window_height") == 0)
            {
                c->winHeight = (int)strtol(value,NULL,10);
            }
            else if(strcmp(key,"canvas_width") == 0)
            {
                c->canvasWidth = (int)strtol(value,NULL,10);
            }
            else if(strcmp(key,"canvas_height") == 0)
            {
                c->canvasHeight = (int)strtol(value,NULL,10);
            }
            else if(strcmp(key,"fullscreen") == 0)
            {
                c->fullscreen = (bool)strtol(value,NULL,10);
            }
            else if(strcmp(key,"fps") == 0)
            {
                c->fps = (int)strtol(value,NULL,10);
            }
        }

        count = !count;
    }

    // Free memory
    destroy_word_data(w);

    return 0;
}