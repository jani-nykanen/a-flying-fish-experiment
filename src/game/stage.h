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

/// Update stage
/// < pl Player
/// < tm Time multiplier
void update_stage(PLAYER* pl, float tm);

/// Draw the stage
/// < cam Camera
void draw_stage(CAMERA* cam);

#endif // __STAGE__