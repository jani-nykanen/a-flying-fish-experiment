/// Player (header)
/// (c) 2018 Jani Nykänen

#ifndef __PLAYER__
#define __PLAYER__

#include "../engine/vector.h"
#include "../engine/assets.h"
#include "../engine/mesh.h"

#include "stdbool.h"

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

    float radius;
    bool control;

    float swimAngleMod;
    float swimAngleMod2;
    float dir;
    float swimWave;
    float swimWave2;
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

/// Player-mesh collision
/// < pl Player
/// < m Mesh
/// < tr Translation
/// < sc Scale
void pl_mesh_collision(PLAYER* pl, MESH* m, VEC3 tr, VEC3 sc);

/// Player-fence collision
/// < y Y coordinate
/// < sx Start x
/// < sy Start y
/// < sz Start z
/// < ex End x
/// < ez End z
/// < skip Skip position
/// < skipl Skipping length
/// < tm Time multiplier
void pl_fence_collision(PLAYER* pl, float y, float sx, float sz, float ex, float ez, float skip, float skipl, float tm);

/// Player-to-player collision
/// < pl Dominant player
/// < o Object
/// < tm Time mul. 
void player_to_player_collision(PLAYER* pl, PLAYER* o, float tm);

#endif // __PLAYER__

