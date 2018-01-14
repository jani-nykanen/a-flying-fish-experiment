/// Music (header)
/// (c) 2018 Jani Nykänen

#ifndef __MUSIC__
#define __MUSIC__

#include "SDL2/SDL_mixer.h"

#include "stdbool.h"

/// Music
typedef struct
{
    Mix_Music* data;
}
MUSIC;

/// Init music
int init_music();

/// Load music
/// < path File path
MUSIC* load_music(const char* path);

/// Play music
/// < mus Music to play
void play_music(MUSIC* mus, float vol);

/// Update music
/// < tm Time mul.
void update_music(float tm);

/// Set approximated music length in milliseconds
/// (needed for one fancy effect)
/// < ms Milliseconds
void set_music_length(int ms);

/// Swap music but keep the position
/// < m Music
/// < vol Volume
void swap_music(MUSIC* m, float vol);

/// Destroy music
/// < m Music
void destroy_music(MUSIC* m);

/// Stop music
void stop_music();

/// Set global music volume
/// < state State
void enable_music(bool state);

#endif // __MUSIC__