/// Mesh (header)
/// (c) 2018 Jani Nykänen

#ifndef __MESH__
#define __MESH__

#include "SDL2/SDL.h"

/** Mesh type */
typedef struct
{
    float* vertices;
    float* uvs;
    float* normals;
    Uint32* indices;

    Uint32 vertexCount;
    Uint32 uvCount;
    Uint32 normalCount;
    Uint32 elementCount;
}
MESH;

/// Load a mesh
/// < path File path
/// > A new mesh
MESH* load_mesh(const char* path);

/// Destroy a mesh
/// < m Mesh
void destroy_mesh(MESH* m);

#endif // __MESH__