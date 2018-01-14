/// Bitmap (source)
/// (c) 2018 Jani Nykänen

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

#include "bitmap.h"
#include "graphics.h"

#include "stdlib.h"
#include "math.h"
#include "stdio.h"


/// Load bitmap
BITMAP* load_bitmap(const char* path)
{
    // Allocate memory
    BITMAP* bmp = (BITMAP*)malloc(sizeof(BITMAP));
    if(bmp == NULL)
    {
        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR,"Error!","Failed to allocate memory for a bitmap!\n",NULL);
        return NULL;
    }

    int comp;
    // Load image
    Uint8* pdata = stbi_load(path,&bmp->w,&bmp->h,&comp,4);
    if(pdata == NULL)
    {
        char err[256];
        snprintf(err,256,"Failed to load a bitmap in %s!\n",path);
         SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR,"Error!",err,NULL);
        return NULL;
    }

    unsigned int pixelCount = bmp->w * bmp->h;

    // Allocate image and temp buffer data
    bmp->data = (Uint8*)malloc(sizeof(Uint8) * pixelCount);
    if(bmp->data == NULL) 
    {
        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR,"Error!","Failed to allocate memory for a bitmap!\n",NULL);
        return NULL;
    }

    // Go through the data
    int i = 0;
    Uint8 pixel;
    Uint8 r,g,b,a;
    Uint8 er,eg,eb;
    for(; i < pixelCount; i++)
    {
        a = pdata[i*4 +3];
        if(a < 255)
        {
            bmp->data[i] = get_alpha();
            continue;
        }

        r = pdata[i*4 +2];
        g = pdata[i*4 +1];
        b = pdata[i*4 ];

        er = (Uint8) (r / 36.428f);
        if(er > 7) r = 7;
        er = er << 5;
        eg = (Uint8) (g / 36.428f);
        if(eg > 7) eg = 7;
        eg = eg << 2;
        eb = (b / 85);

        pixel = er | eg | eb;

        bmp->data[i] = pixel;
    }

    // Free data
    stbi_image_free(pdata);

    return bmp;
}

/// Destroy bitmap
void destroy_bitmap(BITMAP* bmp)
{
    if(bmp == NULL) return;

    if(bmp->data != NULL) free(bmp->data);
    free(bmp);
}

/// Get pixel in bitmap
Uint8 bmp_get_pixel(BITMAP* b, int x, int y)
{
    if(x < 0 || y < 0 || x >= b->w || y >= b->h) return 0;

    return b->data[y * b->w + x];
}