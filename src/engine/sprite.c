/// Sprite (source)
/// (c) 2018 Jani Nykänen

#include "sprite.h"

#include "graphics.h"

#include "stdlib.h"
#include "math.h"
#include "stdio.h"


/// Create a new sprite
SPRITE create_sprite(int w, int h)
{
    return (SPRITE){w,h,0,0,0.0f};
}

/// Animate a sprite
void spr_animate(SPRITE*s, int row, int start, int end, float speed, float tm)
{
    if(start == end)
    {
		s->count = 0;
		s->frame = start;
		s->row = row;
		return;
	}

	if(s->row != row)
    {
		s->count = 0;
		s->frame = end > start ? start : end;
		s->row = row;
	}

	if(start < end && s->frame < start)
    {
		s->frame = start;
    }
    else if(end < start && s->frame < end)
    {
        s->frame = end;
    }

	s->count += 1.0f *tm;
	if(s->count > speed)
    {
        if(start < end)
        {
            s->frame++;
            if(s->frame > end)
            {
                s->frame = start;
            }
        }
        else
        {
            s->frame --;
            if(s->frame < end)
            {
                s->frame = start;
            }
        }

		s->count -= speed;
	}
}

/// Draw a sprite frame
void spr_draw_frame(SPRITE*s, BITMAP* bmp, int frame, int row, int x, int y, int flip)
{
    draw_bitmap_region(bmp,s->w*frame,s->h*row,s->w,s->h,x,y,flip);
}

/// Draw a sprite
void spr_draw(SPRITE* s, BITMAP* bmp, int x, int y, int flip)
{
    spr_draw_frame(s,bmp,s->frame,s->row,x,y,flip);
}
