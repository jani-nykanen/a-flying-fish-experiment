/// Bitmap (header)
/// (c) 2018 Jani Nykänen

#ifndef __BITMAP__
#define __BITMAP__

#include <SDL2/SDL.h>

/// Bitmap type
typedef struct
{
    int w; /// Bitmap width
    int h; /// Bitmap height
    Uint8* data; /// Pixel data
}
BITMAP;

/// Load bitmap
/// < path Bitmap path
/// > Returns a new bitmap (pointer)
BITMAP* load_bitmap(const char* path);

/// Destroy bitmap
void destroy_bitmap(BITMAP* bmp);

/// Get pixel in bitmap
/// > b Bitmap
/// > x X coordinate
/// > y Y coordinate
/// < Color index
Uint8 bmp_get_pixel(BITMAP* b, int x, int y);

#endif // __BITMAP__