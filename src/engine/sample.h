/// Sample (header)
/// (c) 2018 Jani Nykänen

#ifndef __SAMPLE__
#define __SAMPLE__

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

#include "stdbool.h"

/// Sound effect type
typedef struct
{
    Mix_Chunk* chunk; /// Chunk
    int channel; /// Channel
    bool played; /// Has the sound been played at least once
}
SAMPLE;

/// Init sample
void init_samples();

/// Set global sample volume
/// < vol Volume
void set_global_sample_volume(float vol);

/// Load a sample
/// < path Path
/// > A new sound
SAMPLE* load_sample(const char* path);

/// Play a sample
/// < s Sample to play
/// < vol Volume
void play_sample(SAMPLE* s, float vol);

/// Stop all samples
void stop_all_samples();

/// Destroy a sample & free memory
/// < s Sample to destroy
void destroy_sample(SAMPLE* s);

/// Enable/disable samples
void enable_samples(bool state);

#endif // __SAMPLE__
