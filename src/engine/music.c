/// Music (source)
/// (c) 2018 Jani Nykänen

#include "music.h"

#include "SDL2/SDL.h"

#include "stdbool.h"
#include "time.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"

/// Global music volume
static float globalMusicVol;

/// Loop length
static int loopLength;

/// Last time
static unsigned int lastTime;

/// Is music playing
static bool playing;

/// Music enabled
static bool musicEnabled;

/// Position in ms
static unsigned int posMs;

/// Init music
int init_music()
{
    globalMusicVol = 1.0f;
    playing = false;
    loopLength = 0;
    lastTime = 0;
    posMs = 0;
    musicEnabled = true;

    // Init formats
    int flags = MIX_INIT_OGG;
    int initted = Mix_Init(flags);
    if( (initted&flags) != flags) 
    {
        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR,"Error!","Failed to init audio!\n",NULL);
        return 0;
    }

    // Open audio
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512)==-1) 
    {
        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR,"Error!","Failed to open audio!\n",NULL);
        return 1;
    }

    return 0;
}   

/// Load music
MUSIC* load_music(const char* path)
{
    MUSIC* m = (MUSIC*)malloc(sizeof(MUSIC));
    if(m == NULL)
    {
        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR,"Error!","Memory allocation error!\n",NULL);
        return NULL;
    }

    m->data = Mix_LoadMUS(path);
    if(m->data == NULL)
    {
        char err [64];
        snprintf(err,64,"Failed to load a file in %s!",path);

        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR,"Error!",err,NULL);
        return NULL;
    }
    return m;
}

/// Play music
void play_music(MUSIC* mus, float vol)
{
    if(!musicEnabled) return;

    int v = (int)(128 * vol * globalMusicVol);
    if(v > 128) v = 128;
    if(v < 0) v = 0;

    Mix_HaltMusic();
    Mix_VolumeMusic(v);
    Mix_FadeInMusic(mus->data, -1,1000);

    lastTime = (unsigned int)SDL_GetTicks();
    posMs = 0;

    playing = true;
}

/// Update music
void update_music(float tm)
{   
    if(!musicEnabled) return;

    if(!playing) return;

    unsigned int t = SDL_GetTicks();
    if( t - lastTime  > 0)
    {
        posMs += t - lastTime;
        if(posMs > loopLength)
        {
            posMs -= loopLength;
        }
    }

    lastTime = t;
}

/// Set approximated music length in milliseconds
void set_music_length(int ms)
{
    loopLength = ms;
}

/// Swap music but keep the position
void swap_music(MUSIC* m, float vol)
{
    if(!musicEnabled) return;

    int v = (int)(128 * vol * globalMusicVol);
    if(v > 128) v = 128;
    if(v < 0) v = 0;

    Mix_HaltMusic();
    Mix_VolumeMusic(v);
    Mix_PlayMusic(m->data, -1);
    Mix_SetMusicPosition( ((double)posMs) / 1000.0 );
}   

/// Destroy music
void destroy_music(MUSIC* m)
{
    if(m == NULL) return;

    Mix_FreeMusic(m->data);
    free(m);
}

/// Stop music
void stop_music()
{
    if(!musicEnabled) return;

    Mix_FadeOutMusic(1000);
}

/// Enable music
void enable_music(bool state)
{
    if(!state)
    {
        Mix_PauseMusic();
        globalMusicVol = 0.0f;
    }
    else
    {
        Mix_ResumeMusic();
        globalMusicVol = 1.0f;
    }
    musicEnabled = state;
    
}