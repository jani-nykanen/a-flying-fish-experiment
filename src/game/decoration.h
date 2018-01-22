/// Decoration (header)
/// (c) 2018 Jani Nykänen

#ifndef __DECORATION__
#define __DECORATION__

#include "../engine/vector.h"
#include "../engine/graphics.h"
#include "../engine/assets.h"

#include "../lib/parseword.h"

#include "player.h"

/// Decoration type
typedef struct
{
    VEC3 pos;
    VEC3 scale;
    MESH* mesh;
    BITMAP* texture;
}
DECORATION;

/// Create a new decoration
/// < pos Position
/// < scale Scaling
/// < m Mesh
/// < texture Texture
DECORATION new_decoration(VEC3 pos, VEC3 scale, MESH* m, BITMAP* texture);

/// Draw a decoration
/// < dec Decoration
void draw_decoration(DECORATION* dec);

/// Read decorations from a layout file
/// < ass Asset pack
/// < wd Word data
/// < dec An array of decorations
/// > Decoration count
int read_decoration_from_layout(ASSET_PACK* ass, WORDDATA* wd, DECORATION* dec);

/// Player-decorations collisions
/// < pl Player
/// < dec Decorations
/// < count Decoration count
void player_decoration_collision(PLAYER* pl, DECORATION* dec, int count);

#endif // __DECORATION__