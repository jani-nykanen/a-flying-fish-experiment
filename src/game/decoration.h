/// Decoration (header)
/// (c) 2018 Jani Nykänen

#ifndef __DECORATION__
#define __DECORATION__

#include "../engine/vector.h"
#include "../engine/graphics.h"

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

#endif // __DECORATION__