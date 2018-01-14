/// Sprite (header)
/// (c) 2018 Jani Nykänen

#ifndef __SPRITE__
#define __SPRITE__

#include "bitmap.h"

/// Sprite object
typedef struct
{
    int w; /// Width
    int h; /// Height
    int frame; /// Frame
    int row; /// Column
    float count; /// Frame change count
}
SPRITE;

/// Create a new sprite
/// < w Width
/// < h Height
/// > A new sprite
SPRITE create_sprite(int w, int h);

/// Animate a sprite
/// < s Sprite to animate
/// < row Row
/// < start Starting frame
/// < end Ending frame
/// < speed Animation speed
/// < tm Time multiplier
void spr_animate(SPRITE*s, int row, int start, int end, float speed, float tm);

/// Draw a sprite frame
/// < s Sprite to draw
/// < bmp Bitmap to use
/// < frame Sprite frame to draw
/// < row Sprite row to draw
/// < x Destination X 
/// < y Destination Y
/// < flip Flipping flag
void spr_draw_frame(SPRITE*s, BITMAP* bmp, int frame, int row, int x, int y, int flip);

/// Draw a sprite
/// < s Sprite to draw
/// < bmp Bitmap to use
/// < x Destination X 
/// < y Destination Y
/// < flip Flipping flag
void spr_draw(SPRITE* s, BITMAP* bmp, int x, int y, int flip);

#endif // __SPRITE__