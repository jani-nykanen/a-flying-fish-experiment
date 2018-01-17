/// Camera (header file)
/// (c) 2018 Jani Nykänen

#ifndef __CAMERA__
#define __CAMERA__

#include "../engine/vector.h"

#include "player.h"

/// Camera type
typedef struct
{
    VEC3 pos;
    float dist;
    VEC3 angle;
}
CAMERA;

/// Create a new camera
/// < pos Position
/// > A new camera
CAMERA create_camera(VEC3 pos);

/// Follow player
/// < cam Camera
/// < pl Player
/// < tm Time multiplier
void cam_follow_player(CAMERA* cam, PLAYER* pl, float tm);

/// Use camera
void use_camera(CAMERA* cam);

#endif // __CAMERA__