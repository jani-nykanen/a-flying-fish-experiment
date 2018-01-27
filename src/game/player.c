/// Player (source)
/// (c) 2018 Jani Nykänen

#include "player.h"

#include "../engine/graphics.h"
#include "../engine/transform.h"
#include "../engine/mathext.h"

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"

#include "../vpad.h"

#include "game.h"
#include "stage.h"


// Max values (default)
const float MAX_SPEED = 0.1f;
const float MAX_ANGLE_Y = 0.025f;
const float MAX_ANGLE_XZ = 0.05f;
const float ACC = 0.01f;
const float ANGLE_ACC = 0.05f;
const float SWIMV_MIN = 0.001f;
const float SWIMV_MAX = 0.020f;

// Fish mesh
static MESH* mFish;
// Fish textures
static BITMAP* bmpFish;
static BITMAP* bmpFish2;


// Limit player area
static void pl_limit(PLAYER* pl)
{
    const float UPPER_LIMIT = 9.0f;
    const float LOWER_LIMIT = -4.0f;

    if(pl->speed.y < 0.0f && pl->pos.y < -UPPER_LIMIT)
    {
        pl->pos.y = -UPPER_LIMIT;
        pl->angleTarget.x = 0.0f;
        pl->speed.y = 0.0f;

        if(!pl->control)
        {
            pl->angle.x += M_PI/2;
            pl->speed.y *= -1;
            pl->target.y = fabs(pl->target.y);
        }
    }
    else if(pl->speed.y > 0.0f && pl->pos.y > -LOWER_LIMIT)
    {
        pl->pos.y = -LOWER_LIMIT;
        pl->angleTarget.x = 0.0f;
        pl->speed.y = 0.0f;

        if(!pl->control)
        {
            pl->angle.x -= M_PI/2;
            pl->speed.y *= -1;
            pl->target.y = -fabs(pl->target.y);
        }
    }

}


// Speed delta
static void pl_speed_delta(float* speed, float target, float acc, float tm)
{
    if(target > *speed)
    {
        *speed += acc * tm;
        if(*speed > target)
            *speed = target;
            
    }
    else if(target < *speed)
    {
        *speed -= acc * tm;
        if(*speed < target)
            *speed = target;
    }
}


// Limit angle
static void pl_limit_angle(PLAYER* pl)
{
    if(pl->angle.x > M_PI/4)
    {
        pl->angle.x = M_PI/4;
        pl->angleSpeed.x = 0.0f;

        if(!pl->control)
        {
            pl->swimAngleMod2 -= M_PI;
        }
    }
    else if(pl->angle.x < -M_PI/4)
    {
        pl->angle.x = -M_PI/4;
        pl->angleSpeed.x = 0.0f;

        if(!pl->control)
        {
            pl->swimAngleMod2 -= M_PI;
        }
    }
}


// AI control for NPC fish
static void pl_ai_control(PLAYER* pl, float tm)
{
    const float MAX_SWIM = 2.0f * M_PI;

    pl->swimAngleMod += pl->swimWave * tm;
    pl->swimAngleMod2 += pl->swimWave2 * tm;
    
    if(pl->swimAngleMod >= MAX_SWIM)
    {
        pl->swimAngleMod -= MAX_SWIM;
        pl->swimWave = (float)(rand() % 1000) / 1000.0f * SWIMV_MAX + SWIMV_MIN;
        pl->dir = (rand() % 2 == 0 ? 1.0f : -1.0f);

        float mod = (float)(rand() % 100) / 100.0f * 0.5f + 0.75f;
        pl->maxSpeed.x *= mod;
        pl->maxSpeed.y *= mod;
        pl->maxSpeed.z *= mod;
    }
    if(pl->swimAngleMod2 >= MAX_SWIM)
    {
        pl->swimAngleMod2 -= MAX_SWIM;
        pl->swimWave2 = ( (float)(rand() % 1000) / 1000.0f * SWIMV_MAX + SWIMV_MIN );
    }

    pl->angle.y += 0.01f* pl->dir *sin( pl->swimAngleMod) * M_PI;
    pl->angle.x += 0.025f* -1.0f * pl->dir *cos(pl->swimAngleMod2) * (M_PI/4.0f);

    pl_limit_angle(pl);

    pl->target.x = sin(pl->angle.y) * pl->maxSpeed.x;
    pl->target.z = cos(pl->angle.y) * pl->maxSpeed.z;
    pl->target.y = sin(pl->angle.x) * pl->maxSpeed.y;
}


// Player controls
static void pl_control(PLAYER* pl)
{
    const float END_BORDER = 40.0f;

    if(!pl->canControl)
    {
        pl->target.x = sin(pl->angle.y) * pl->maxSpeed.x;
        pl->target.z = cos(pl->angle.y) * pl->maxSpeed.z;
        return;
    }

    pl->angleTarget = vec3(0,0,0);
    pl->target.y = 0.0f;

    VEC2 stick = vpad_get_stick();
    if(!pl->outsideCamera)
    {
        if(fabs(stick.x)  > 0.1f)
        {
            pl->angleTarget.y = stick.x * pl->angleMax.y;
            pl->angleTarget.z = stick.x * pl->angleMax.z;
        }
        else if(fabs(pl->angle.z) > 0.001f)
        {
            pl->angleTarget.z = -2* (pl->angle.z / (M_PI/2))* pl->angleMax.z;
        }
        
        if(fabs(stick.y)  > 0.1f)
        {
            pl->angleTarget.x = stick.y * pl->angleMax.x;
            pl->target.y = pl->maxSpeed.y * stick.y;
        }
        else if(fabs(pl->angle.x) > 0.001f)
        {
            pl->angleTarget.x = -2* (pl->angle.x / (M_PI/2))* pl->angleMax.x;
        }
    }

    pl_limit_angle(pl);

    pl->target.x = sin(pl->angle.y) * pl->maxSpeed.x;
    pl->target.z = cos(pl->angle.y) * pl->maxSpeed.z;

    if(!world_ended() && (pl->pos.z < -END_BORDER || pl->pos.z > END_BORDER || pl->pos.x < -END_BORDER || pl->pos.x > END_BORDER))
    {
        game_start_fading();
    }

}


// Rotate
static void pl_rotate(PLAYER* pl, float tm)
{
    pl_speed_delta(&pl->angleSpeed.x,pl->angleTarget.x,pl->angleAcc.x,tm);
    pl_speed_delta(&pl->angleSpeed.y,pl->angleTarget.y,pl->angleAcc.y,tm);
    pl_speed_delta(&pl->angleSpeed.z,pl->angleTarget.z,pl->angleAcc.z,tm);

    pl->angle.x += pl->angleSpeed.x * tm;
    pl->angle.y += pl->angleSpeed.y * tm;
    pl->angle.z += pl->angleSpeed.z * tm;
}


// Player movement
static void pl_move(PLAYER* pl, float tm)
{
    pl_speed_delta(&pl->speed.x,pl->target.x,pl->acc.x,tm);
    pl_speed_delta(&pl->speed.y,pl->target.y,pl->acc.y,tm);
    pl_speed_delta(&pl->speed.z,pl->target.z,pl->acc.z,tm);

    pl->pos.x += pl->speed.x * tm;
    pl->pos.y += pl->speed.y * tm;
    pl->pos.z += pl->speed.z * tm;
}


// Player-triangle collision
static void pl_triangle_collision(PLAYER* pl, VEC3 A, VEC3 B, VEC3 C, VEC3 N)
{
    if(!(inside_triangle(pl->pos.x,pl->pos.y,A.x,A.y,B.x,B.y,C.x,C.y)
    || inside_triangle(pl->pos.x,pl->pos.z,A.x,A.z,B.x,B.z,C.x,C.z)
    || inside_triangle(pl->pos.y,pl->pos.z,A.y,A.z,B.y,B.z,C.y,C.z)))
    {
        return;
    }

    float d = -(N.x * A.x + N.y * A.y + N.z * A.z);
    float dist = (pl->pos.x * N.x + pl->pos.y * N.y + pl->pos.z * N.z + d) 
            / sqrt( N.x*N.x + N.y*N.y + N.z*N.z);

    float dir = dist > 0 ? 1.0f : -1.0f;

    if(fabs(dist) < pl->radius)
    {
        pl->pos.x -= dir* N.x * (fabs(dist)-pl->radius);
        pl->pos.y -= dir* N.y * (fabs(dist)-pl->radius);
        pl->pos.z -= dir* N.z * (fabs(dist)-pl->radius);

        pl->speed.x += dir * N.x  * pl->maxSpeed.x * 0.5f;
        pl->speed.y += dir * N.y  * pl->maxSpeed.y * 0.5f;
        pl->speed.z += dir * N.z  * pl->maxSpeed.z * 0.5f;
    }
}


// Initialize
void init_player(ASSET_PACK* ass)
{
    bmpFish = (BITMAP*)get_asset(ass,"fish_tex");
    bmpFish2 = (BITMAP*)get_asset(ass,"fish_tex2");
    mFish = (MESH*)get_asset(ass,"fish");
}


// Create
PLAYER pl_create(VEC3 pos)
{
    PLAYER pl;
    pl.pos = pos;
    pl.speed = vec3(0,0,0);
    pl.target = pl.speed;

    pl.radius = 0.5f;

    pl.maxSpeed = vec3(MAX_SPEED,MAX_SPEED,MAX_SPEED);
    pl.angleMax = vec3(MAX_ANGLE_XZ,MAX_ANGLE_Y,MAX_ANGLE_XZ);

    pl.angleAcc = vec3(ANGLE_ACC,ANGLE_ACC,ANGLE_ACC);
    pl.acc = vec3(ACC,ACC,ACC);

    pl.angle = vec3(0,0,0);
    pl.angleSpeed = vec3(0,0,0);
    pl.angleTarget = vec3(0,0,0);
    pl.swimAngleMod = (float)(rand() % 1000)/1000.0f * M_PI*2;;

    pl.swimWave = (float)(rand() % 1000) / 1000.0f * SWIMV_MAX + SWIMV_MIN;
    pl.swimWave2 = ( (float)(rand() % 1000) / 1000.0f * SWIMV_MAX + SWIMV_MIN );
    pl.dir = (rand() % 2 == 0 ? 1.0f : -1.0f);

    pl.control = true;
    pl.outsideCamera = false;
    pl.canControl = false;

    return pl;
}


// Update
void pl_update(PLAYER* pl, float tm)
{
    if(pl->control)
        pl_control(pl);
    else
        pl_ai_control(pl,tm);

    pl_limit(pl);
    pl_move(pl,tm);
    pl_rotate(pl,tm);
}


// Draw
void pl_draw(PLAYER* pl)
{
    tr_translate_model(pl->pos.x,pl->pos.y,pl->pos.z);
    tr_rotate_model(-M_PI/2+pl->angle.x,M_PI+pl->angle.y,M_PI/2);
    tr_scale_model(1.0f,1.0f,1.0f);

    bind_texture(pl->control ? bmpFish : bmpFish2);
    draw_mesh(mFish);
}


// Player-mesh collision
void pl_mesh_collision(PLAYER* pl, MESH* m, VEC3 tr, VEC3 sc)
{
    if(pl == NULL || m == NULL) return;

    VEC3 minV = m->minV;
    VEC3 maxV = m->maxV;

    minV.x *= sc.x; minV.x += tr.x;
    minV.y *= sc.y; minV.y += tr.y;
    minV.z *= sc.z; minV.z += tr.z;

    maxV.x *= sc.x; maxV.x += tr.x;
    maxV.y *= sc.y; maxV.y += tr.y;
    maxV.z *= sc.z; maxV.z += tr.z;
    
    minV.x -= pl->radius*2; minV.y -= pl->radius*2; minV.z -= pl->radius*2;
    maxV.x += pl->radius*2; maxV.y += pl->radius*2; maxV.z += pl->radius*2;

    if(pl->pos.x < minV.x || pl->pos.x > maxV.x
    || pl->pos.y < minV.y || pl->pos.y > maxV.y
    || pl->pos.z < minV.z || pl->pos.z > maxV.z) 
        return;

    VEC3 A;
    VEC3 B;
    VEC3 C;
    VEC3 N;

    int i = 0;
    for(; i < m->elementCount; i += 3)
    {
        A = vec3(m->vertices[m->indices[i]*3],m->vertices[m->indices[i]*3 +1],m->vertices[m->indices[i]*3+2]);
        B = vec3(m->vertices[m->indices[i]*3 +3],m->vertices[m->indices[i]*3 +4],m->vertices[m->indices[i]*3+5]);
        C = vec3(m->vertices[m->indices[i]*3 +6],m->vertices[m->indices[i]*3 +7],m->vertices[m->indices[i]*3+8]);

        N = vec3(m->normals[m->indices[i]*3],m->normals[m->indices[i]*3 +1],m->normals[m->indices[i]*3+2]);

        A.x *= sc.x; A.x += tr.x;
        A.y *= sc.y; A.y += tr.y;
        A.z *= sc.z; A.z += tr.z;

        B.x *= sc.x; B.x += tr.x;
        B.y *= sc.y; B.y += tr.y;
        B.z *= sc.z; B.z += tr.z;

        C.x *= sc.x; C.x += tr.x;
        C.y *= sc.y; C.y += tr.y;
        C.z *= sc.z; C.z += tr.z;

        pl_triangle_collision(pl,A,B,C,N);
    }
}


// Player-fence collision
void pl_fence_collision(PLAYER* pl,float y,float sx, float sz, float ex, float ez, float skip, float skipl, float tm)
{
    if(pl->control && pl->pos.y - pl->radius < y)
        return;

    if(pl->pos.z+pl->radius > sz && pl->pos.z-pl->radius < ez)
    {
        if( (pl->speed.x < 0.0f && pl->pos.x-pl->radius < sx && pl->pos.x-pl->radius > sx - 0.25f*tm)
        || (pl->speed.x > 0.0f && pl->pos.x+pl->radius > ex && pl->pos.x+pl->radius < ex + 0.25f*tm))
        {
            if(pl->control)
            {
                pl->speed.x = 0.0f;
            }
            else
            {
                pl->speed.x *= -1;
                pl->target.x *= -1;
                pl->angle.y += M_PI;
            }

            pl->pos.x = pl->pos.x < 0.0f ? sx+pl->radius : ex-pl->radius;
        }
    }
    
    if(pl->pos.x+pl->radius > sz && pl->pos.x-pl->radius < ex)
    {
        if(pl->control && pl->pos.x-pl->radius > skip && pl->pos.x + pl->radius < skip+skipl) return;

        if( (pl->speed.z < 0.0f && pl->pos.z-pl->radius < sz && pl->pos.z-pl->radius > sz - 0.25f*tm)
        || (pl->speed.z > 0.0f && pl->pos.z+pl->radius > ez && pl->pos.z+pl->radius < ez + 0.25f*tm))
        {
            if(pl->control)
            {
                pl->speed.z = 0.0f;
            }
            else
            {
                pl->speed.z *= -1;
                pl->target.z *= -1;
                pl->angle.y += M_PI;
            }
            pl->pos.z = pl->pos.z < 0.0f ? (sz+pl->radius) : (ez-pl->radius);
        }
    }
}

// Player-to-player collision
void player_to_player_collision(PLAYER* pl, PLAYER* o, float tm)
{
    float dist = sqrt( (pl->pos.x-o->pos.x)*(pl->pos.x-o->pos.x) + (pl->pos.y-o->pos.y)*(pl->pos.y-o->pos.y) + (pl->pos.z-o->pos.z)*(pl->pos.z-o->pos.z));
    float r = (pl->radius+o->radius)*1.6f;
    if(dist < r)
    {
        float angle = atan2(pl->pos.z-o->pos.z,pl->pos.x-o->pos.x);
        
        o->pos.x += sin(angle) * (r-dist) / 2;
        o->pos.z += cos(angle) * (r-dist) / 2;

        pl->pos.x -= sin(angle) * (r-dist) / 2;
        pl->pos.z -= cos(angle) * (r-dist) / 2;

        float delta = (pl->pos.y-o->pos.y);

        o->pos.y -= delta / 4;
        pl->pos.y += delta / 4;
    }
}