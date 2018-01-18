/// Stage (source)
/// (c) 2018 Jani Nykänen

#include "stage.h"

#include "../engine/graphics.h"
#include "../engine/transform.h"

// Grass texture
static BITMAP* bmpGrass;


// Draw a floor tile (or piece of it)
static void draw_floor_tile_small(float x, float y, float z, float w, float h, float u, float v, float uw, float vh)
{
    bind_texture(bmpGrass);

    draw_triangle_3d(vec3(x,y,z),vec3(x+w,y,z),vec3(x+w,y,z+h),
        vec2(u,v),vec2(u+uw,v),vec2(u+uw,v+vh),vec3(0,1,0));

    draw_triangle_3d(vec3(x+w,y,z+h),vec3(x,y,z+h),vec3(x,y,z),
       vec2(u+uw,v+vh),vec2(u,v+vh),vec2(u,v),vec3(0,1,0));

}

// Draw a floor tile
static void draw_floor_tile(float x, float y, float z, float w, float h, int subdivide)
{
    int dx = 0, dz = 0;
    float stepw = w / subdivide;
    float steph = h / subdivide;
    float stepuv = 1.0 / subdivide;

    for(dz = 0; dz < subdivide; ++ dz)
    {
        for(dx = 0; dx < subdivide; ++ dx)
        {
             draw_floor_tile_small(x + stepw*dx, y, z + steph*dz,stepw,steph,stepuv * dx,stepuv*dz,stepuv,stepuv);
        }
    }

}


// Draw floor
static void draw_floor(CAMERA* cam, float y)
{
    const float TILE_SIZE = 10.0f;
    const int TILE_COUNT = 8;

    int x = (int)floor(cam->pos.x / TILE_SIZE);
    int z = (int)floor(cam->pos.z / TILE_SIZE);

    int sx = x - TILE_COUNT / 2;
    int sz = z - TILE_COUNT / 2;
    int ex = sx + TILE_COUNT;
    int ez = sz + TILE_COUNT;

    int stepx = 1;
    int stepz = 1;
    int subdivide = 1;

    int tcount = 0;

    int dx, dz;
    for(dz = sz; dz <= ez; dz += stepz)
    {
        for(dx = sx; dx <= ex; dx += stepx)
        {
            subdivide = 1;

            if(dx >= sx + TILE_COUNT/2 - 2 && dx <= ex - TILE_COUNT/2 + 2
            && dz >= sz + TILE_COUNT/2 - 2 && dz <= ez - TILE_COUNT/2 + 2) 
                subdivide = 2;

            if(dx >= sx + TILE_COUNT/2 - 1 && dx <= ex - TILE_COUNT/2 + 1
            && dz >= sz + TILE_COUNT/2 - 1 && dz <= ez - TILE_COUNT/2 + 1) 
                subdivide = 4;

            draw_floor_tile(dx * TILE_SIZE,y, dz * TILE_SIZE, TILE_SIZE, TILE_SIZE,subdivide);

            tcount += subdivide*subdivide;
        }
    }
}


// Initialize stage
void init_stage(ASSET_PACK* ass)
{
    bmpGrass = (BITMAP*)get_asset(ass,"grass");
}


// Update stage
void update_stage(PLAYER* pl, float tm)
{

}


// Draw the stage
void draw_stage(CAMERA* cam)
{
    draw_floor(cam,5);
}