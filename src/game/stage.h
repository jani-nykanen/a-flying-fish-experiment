/// Stage (header)
/// (c) 2018 Jani Nykänen

#ifndef __STAGE__
#define __STAGE__

#include "../engine/assets.h"

#include "camera.h"
#include "player.h"

/// Initialize stage
/// < ass Asset pack
int init_stage(ASSET_PACK* ass);

/// Stage-player collision
/// < pl Player
/// < tm Time mul.
void stage_player_collision(PLAYER* pl, float tm);

/// Update stage
/// < tm Time mul.
void update_stage(float tm);

/// Draw the stage
/// < cam Camera
void draw_stage(CAMERA* cam);

/// Start the apocalypse 
void end_stage();

/// Has the stage ended & are the decorations gone
/// < True, if true (ehheh)
bool world_ended();

#endif // __STAGE__