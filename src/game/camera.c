/// Camera (source file)
/// (c) 2018 Jani Nykänen

#include "camera.h"

#include "../engine/transform.h"

#include "stage.h"

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"


// Angle reached
static bool angleReached;


// Limit camera
static void limit_camera(CAMERA* cam)
{
    const float UPPER_LIMIT = 5.0f;
    const float LOWER_LIMIT = -3.0f;

    if(cam->pos.y < -UPPER_LIMIT)
    {
        cam->pos.y = -UPPER_LIMIT;
    }
    else if(cam->pos.y > -LOWER_LIMIT)
    {
        cam->pos.y = -LOWER_LIMIT;
    }

}


// Move around
static void cam_move_around(CAMERA* cam, PLAYER*pl, float tm)
{
    cam->pos = pl->pos;
    cam->angle.y += 0.025f * tm;
    if(cam->angle.y >= M_PI*2)
    {
        cam->angle.y = 0.0f;
        cam->movedAround = true;
    }
}


// Create camera
CAMERA create_camera(VEC3 pos)
{
    angleReached = false;

    CAMERA cam;
    cam.pos = pos;
    cam.dist = 4.75f;
    cam.angle = vec3(0,0,0);
    cam.movedAround = false;
    return cam;
}


// Follow player
void cam_follow_player(CAMERA* cam, PLAYER* pl, float tm)
{
    const float RESTRICT = 30.0f;

    if(!cam->movedAround)
    {
        cam_move_around(cam,pl,tm);
        return;
    }

    if(!world_ended() && (pl->pos.z < -RESTRICT || pl->pos.z > RESTRICT || pl->pos.x < -RESTRICT || pl->pos.x > RESTRICT))
    {
        pl->outsideCamera = true;
        return;
    }
    pl->outsideCamera = false;

    float dist = hypot(pl->pos.x-cam->pos.x,pl->pos.z-cam->pos.z);
    float angle = atan2(pl->pos.z-cam->pos.z,pl->pos.x-cam->pos.x);

    if(dist > 0.001f)
    {
        float d = dist - 0.001f;

        cam->pos.x += cos(angle) * (d/12.0f) *tm;
        cam->pos.z += sin(angle) * (d/12.0f) *tm;
        cam->pos.y += (pl->pos.y - cam->pos.y) / 12.0f * tm;
    }    

    float targetAngle;
    if(world_ended())
    {
        targetAngle = (M_PI + atan2(cam->pos.x,cam->pos.z));
        if(!angleReached)
        {
            cam->angle.y += (targetAngle - cam->angle.y)/24.0f * tm;
            if(fabs(cam->angle.y-targetAngle) < 0.01f*tm)
            {
                angleReached = true;
            }
        }
        else
        {
            cam->angle.y = targetAngle;
        }
        
    }
    else
    {
        targetAngle = pl->angle.y;
        cam->angle.y += (targetAngle - cam->angle.y)/24.0f * tm;
    }


    limit_camera(cam);
}


// Use camera
void use_camera(CAMERA* cam)
{
    cam->vpos.x = cam->pos.x - cos(cam->angle.y - M_PI/2.0f) * cam->dist;
    cam->vpos.y = cam->pos.y - 1.0f;
    cam->vpos.z = cam->pos.z + sin(cam->angle.y - M_PI/2.0f) * cam->dist;

    tr_translate(-cam->vpos.x,-cam->vpos.y,-cam->vpos.z);
    tr_rotate_world(cam->angle.y,cam->angle.x);
}