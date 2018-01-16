/// Player (source)
/// (c) 2018 Jani Nykänen

#include "player.h"

#include "../engine/graphics.h"
#include "../engine/transform.h"

#include "math.h"
#include "stdio.h"

#include "../vpad.h"

// Fish mesh
static MESH* mFish;
// Fish texture
static BITMAP* bmpFish;


// Limit angle
static void pl_limit_angle(PLAYER* pl)
{
    if(pl->angle.x > M_PI/4)
    {
        pl->angle.x = M_PI/4;
        pl->angleSpeed.x = 0.0f;
    }
    else if(pl->angle.x < -M_PI/4)
    {
        pl->angle.x = -M_PI/4;
        pl->angleSpeed.x = 0.0f;
    }

    if(pl->angle.z > M_PI/4)
    {
        pl->angle.z = M_PI/4;
        pl->angleSpeed.z = 0.0f;
    }
    else if(pl->angle.z < -M_PI/4)
    {
        pl->angle.z = -M_PI/4;
        pl->angleSpeed.z = 0.0f;
    }
}


// Player controls
static void pl_control(PLAYER* pl)
{
    pl->angleTarget = vec3(0,0,0);

    VEC2 stick = vpad_get_stick();
    if(fabs(stick.x)  > 0.1f)
    {
        pl->angleTarget.y = stick.x * pl->angleMax.y;
        pl->angleTarget.z = stick.x * pl->angleMax.z;
    }
    else if(fabs(pl->angle.z) > 0.001f)
    {
        pl->angleTarget.z = -(pl->angle.z / (M_PI/2))* pl->angleMax.z;
    }
    
    if(fabs(stick.y)  > 0.1f)
    {
        pl->angleTarget.x = stick.y * pl->angleMax.x;
    }
    else if(fabs(pl->angle.x) > 0.001f)
    {
        pl->angleTarget.x = -(pl->angle.x / (M_PI/2))* pl->angleMax.x;
    }

    pl_limit_angle(pl);
}


// Rotate
static void pl_rotate(PLAYER* pl, float tm)
{
    if(pl->angleTarget.x > pl->angleSpeed.x)
    {
        pl->angleSpeed.x += pl->angleAcc.x * tm;
        if(pl->angleSpeed.x > pl->angleTarget.x)
            pl->angleSpeed.x = pl->angleTarget.x;
    }
    else if(pl->angleTarget.x < pl->angleSpeed.x)
    {
        pl->angleSpeed.x -= pl->angleAcc.x * tm;
        if(pl->angleSpeed.x < pl->angleTarget.x)
            pl->angleSpeed.x = pl->angleTarget.x;
    }

    if(pl->angleTarget.y > pl->angleSpeed.y)
    {
        pl->angleSpeed.y += pl->angleAcc.y * tm;
        if(pl->angleSpeed.y > pl->angleTarget.y)
            pl->angleSpeed.y = pl->angleTarget.y;
    }
    else if(pl->angleTarget.y < pl->angleSpeed.y)
    {
        pl->angleSpeed.y -= pl->angleAcc.y * tm;
        if(pl->angleSpeed.y < pl->angleTarget.y)
            pl->angleSpeed.y = pl->angleTarget.y;
    }

    if(pl->angleTarget.z > pl->angleSpeed.z)
    {
        pl->angleSpeed.z += pl->angleAcc.z * tm;
        if(pl->angleSpeed.z > pl->angleTarget.z)
            pl->angleSpeed.z = pl->angleTarget.z;
    }
    else if(pl->angleTarget.z < pl->angleSpeed.z)
    {
        pl->angleSpeed.z -= pl->angleAcc.z * tm;
        if(pl->angleSpeed.z < pl->angleTarget.z)
            pl->angleSpeed.z = pl->angleTarget.z;
    }

    pl->angle.x += pl->angleSpeed.x * tm;
    pl->angle.y += pl->angleSpeed.y * tm;
    pl->angle.z += pl->angleSpeed.z * tm;
}


// Player movement
static void pl_move(PLAYER* pl, float tm)
{
    if(pl->target.x > pl->speed.x)
    {
        pl->speed.x += pl->acc.x * tm;
        if(pl->speed.x > pl->target.x)
            pl->speed.x = pl->target.x;
    }
    else if(pl->target.x < pl->speed.x)
    {
        pl->speed.x -= pl->acc.x * tm;
        if(pl->speed.x < pl->target.x)
            pl->speed.x = pl->target.x;
    }

    if(pl->target.y > pl->speed.y)
    {
        pl->speed.y += pl->acc.y * tm;
        if(pl->speed.y > pl->target.y)
            pl->speed.y = pl->target.y;
    }
    else if(pl->target.y < pl->speed.y)
    {
        pl->speed.y -= pl->acc.y * tm;
        if(pl->speed.y < pl->target.y)
            pl->speed.y = pl->target.y;
    }

    if(pl->target.z > pl->speed.z)
    {
        pl->speed.z += pl->acc.z * tm;
        if(pl->speed.z > pl->target.z)
            pl->speed.z = pl->target.z;
    }
    else if(pl->target.z < pl->speed.z)
    {
        pl->speed.z -= pl->acc.z * tm;
        if(pl->speed.z < pl->target.z)
            pl->speed.z = pl->target.z;
    }

    pl->pos.x += pl->speed.x * tm;
    pl->pos.y += pl->speed.y * tm;
    pl->pos.z += pl->speed.z * tm;
}


// Initialize
void init_player(ASSET_PACK* ass)
{
    bmpFish = get_asset(ass,"fish_tex");
    mFish = get_asset(ass,"fish");
}


// Create
PLAYER pl_create(VEC3 pos)
{
    const float MAX_SPEED = 0.25f;
    const float MAX_ANGLE_Y = 0.025f;
    const float MAX_ANGLE_XZ = 0.05f;
    const float ACC = 0.01f;
    const float ANGLE_ACC = 0.05f;

    PLAYER pl;
    pl.pos = pos;
    pl.speed = vec3(0,0,0);
    pl.target = pl.speed;

    pl.maxSpeed = vec3(MAX_SPEED,MAX_SPEED,MAX_SPEED);
    pl.angleMax = vec3(MAX_ANGLE_XZ,MAX_ANGLE_Y,MAX_ANGLE_XZ);

    pl.angleAcc = vec3(ANGLE_ACC,ANGLE_ACC,ANGLE_ACC);
    pl.acc = vec3(ACC,ACC,ACC);

    pl.angle = vec3(0,0,0);
    pl.angleSpeed = vec3(0,0,0);
    pl.angleTarget = vec3(0,0,0);

    return pl;
}


// Update
void pl_update(PLAYER* pl, float tm)
{
    pl_control(pl);
    pl_move(pl,tm);
    pl_rotate(pl,tm);
}


// Draw
void pl_draw(PLAYER* pl)
{
    tr_translate_model(pl->pos.x,pl->pos.y,pl->pos.z);
    tr_rotate_model(-M_PI/2+pl->angle.x,M_PI+pl->angle.y,M_PI/2 + pl->angle.z);
    tr_scale_model(1.0f,1.0f,1.0f);

    bind_texture(bmpFish);
    draw_mesh(mFish);
}

