/// Camera (source file)
/// (c) 2018 Jani Nykänen

#include "camera.h"

#include "../engine/transform.h"

#include "math.h"


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


// Create camera
CAMERA create_camera(VEC3 pos)
{
    CAMERA cam;
    cam.pos = pos;
    cam.dist = 4.75f;
    cam.angle = vec3(0,0,0);
    return cam;
}


// Follow player
void cam_follow_player(CAMERA* cam, PLAYER* pl, float tm)
{
    const float RESTRICT = 30.0f;

    if(pl->pos.z < -RESTRICT || pl->pos.z > RESTRICT || pl->pos.x < -RESTRICT || pl->pos.x > RESTRICT)
    {
        return;
    }

    float dist = hypot(pl->pos.x-cam->pos.x,pl->pos.z-cam->pos.z);
    float angle = atan2(pl->pos.z-cam->pos.z,pl->pos.x-cam->pos.x);

    if(dist > 0.001f)
    {
        float d = dist - 0.001f;

        cam->pos.x += cos(angle) * (d/12.0f) *tm;
        cam->pos.z += sin(angle) * (d/12.0f) *tm;
        cam->pos.y += (pl->pos.y - cam->pos.y) / 12.0f * tm;
    }    

    cam->angle.y += (pl->angle.y - cam->angle.y)/24.0f * tm;


    cam->vpos.x = cam->pos.x - cos(cam->angle.y - M_PI/2.0f) * cam->dist;
    cam->vpos.y = cam->pos.y - 1.0f;
    cam->vpos.z = cam->pos.z + sin(cam->angle.y - M_PI/2.0f) * cam->dist;

    limit_camera(cam);
}


// Use camera
void use_camera(CAMERA* cam)
{

    tr_translate(-cam->vpos.x,-cam->vpos.y,-cam->vpos.z);
    tr_rotate_world(cam->angle.y,cam->angle.x);
}