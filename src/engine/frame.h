/// Frame (header)
/// (c) 2018 Jani Nykänen

#ifndef __FRAME__
#define __FRAME__

#include "SDL2/SDL.h"

/// Frame structure
typedef struct
{
    // This data is the same with BITMAP
    int w; /// Width
    int h; /// Height
    Uint8* colorData; /// Color data

    unsigned int size; /// Actual size in pixels
    float* depth; /// Depth buffer

    Uint8* data; /// Frame data
    SDL_Texture* tex; /// Frame texture   
}
FRAME;

/// Generate global palette
void fr_gen_palette();

/// Creates a new frame
/// < w Width
/// < h Height
/// > A pointer to a new frame
FRAME* frame_create(int w, int h);

/// Update frame texture
/// < fr Frame
void frame_update_tex(FRAME* fr);

/// Copy frame color data
/// < s Source
/// < d Destination
void copy_frame(FRAME* s, FRAME* d);

/// Invert frame
/// < f Frame
void invert_frame(FRAME* f);

#endif // __FRAME__