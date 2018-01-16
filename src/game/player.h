/// Player (header)
/// (c) 2018 Jani Nykänen

#ifndef __PLAYER__
#define __PLAYER__

#include "../engine/vector.h"
#include "../engine/assets.h"

/// Player type
typedef struct
{
    VEC3 pos;
    VEC3 speed;
    VEC3 target;
    VEC3 acc;
    VEC3 maxSpeed;

    VEC3 angle;
    VEC3 angleSpeed;
    VEC3 angleTarget;
    VEC3 angleMax;
    VEC3 angleAcc;
}
PLAYER;

/// Init global player data
/// < ass Asset pack
void init_player(ASSET_PACK* ass);

/// Create a player object
/// < pos Position
/// > A player object
PLAYER pl_create(VEC3 pos);

/// Update player
/// < pl Player
/// < tm Time mul
void pl_update(PLAYER* pl, float tm);

/// Draw player
/// < pl Player
void pl_draw(PLAYER* pl);

#endif // __PLAYER__

