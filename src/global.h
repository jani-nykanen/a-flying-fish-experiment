/// Global scene (header)
/// (c) 2018 Jani Nykänen

#ifndef __GLOBAL__
#define __GLOBAL__

#include "engine/scene.h"
#include "engine/assets.h"

/// Return the global scene
/// > Scene
SCENE get_global_scene();

/// Get global asset pack
/// > Asset pack
ASSET_PACK* get_global_assets();

#endif // __GLOBAL__