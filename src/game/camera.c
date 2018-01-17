/// Camera (source file)
/// (c) 2018 Jani Nykänen

#include "camera.h"

#include "../engine/transform.h"

#include "math.h"


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
    float dist = hypot(pl->pos.x-cam->pos.x,pl->pos.z-cam->pos.z);
    float angle = atan2(pl->pos.z-cam->pos.z,pl->pos.x-cam->pos.x);

    if(dist > 0.001f)
    {
        float d = dist - 0.001f;

        cam->pos.x += cos(angle) * (d/6.0f);
        cam->pos.z += sin(angle) * (d/6.0f);
    }    
}


// Use camera
void use_camera(CAMERA* cam)
{
    VEC3 p = vec3(0,0,0);

    p.x = cam->pos.x + cos(cam->angle.y - M_PI/2.0f) * cam->dist;
    p.y = cam->pos.y - 1.0f;
    p.z = cam->pos.z + sin(cam->angle.y - M_PI/2.0f) * cam->dist;

    tr_translate(-p.x,-p.y,-p.z);
}