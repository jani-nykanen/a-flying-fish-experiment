/// Decoration (source)
/// (c) 2018 Jani Nykänen

#include "decoration.h"

#include "../engine/transform.h"

// Create a new decoration
DECORATION new_decoration(VEC3 pos, VEC3 scale, MESH* m, BITMAP* texture)
{
    DECORATION d;
    d.pos = pos;
    d.scale = scale;
    d.mesh = m;
    d.texture = texture;
    return d;
}


// Draw a decoration
void draw_decoration(DECORATION* d)
{
    tr_translate_model(d->pos.x,d->pos.y,d->pos.z);
    tr_scale_model(d->scale.x,d->scale.y,d->scale.z);
    
    bind_texture(d->texture);

    draw_mesh(d->mesh);
}