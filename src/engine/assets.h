/// Asset pack (header)
/// (c) 2018 Jani Nykänen

#ifndef __ASSET_PACK__
#define __ASSET_PACK__

#include "SDL2/SDL.h"

/// Asset buffer size
#define NAME_BUFFER_SIZE 32

/// Any asset type aka void pointer
typedef void* ANY;

/// Name structure 
typedef struct
{
    char data[NAME_BUFFER_SIZE];
}
NAME;

/// Asset pack type
typedef struct
{
    int* types;
    ANY* objects;
    NAME* names;
    Uint32 assetCount;
}
ASSET_PACK;

/// Load an asset pack
/// < path Asset list path
/// > A new asset pack
ASSET_PACK* load_asset_pack(const char* path);

/// Get an asset by its name
/// < p Asset pack
/// < name Asset name
ANY get_asset(ASSET_PACK* p, const char* name);

/// Destroy an asset pack
/// < p Asset pack
void destroy_asset_pack(ASSET_PACK* p);

#endif // __ASSET_PACK__
 