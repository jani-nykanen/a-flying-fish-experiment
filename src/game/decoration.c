/// Decoration (source)
/// (c) 2018 Jani Nykänen

#include "decoration.h"

#include "../engine/transform.h"

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"

#define IS(s,x) strcmp(s,x) == 0

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


// Read decorations from a layout file
int read_decoration_from_layout(ASSET_PACK* ass, WORDDATA* wd, DECORATION* dec)
{
    MESH* m = NULL;
    BITMAP* bmp = NULL;
    VEC3 pos =  vec3(0,0,0);
    VEC3 scale = vec3(1,1,1);
    char* w = NULL;

    int decCount = 0;

    int i = 0;
    for(; i < wd->wordCount; ++ i)
    {
        w = get_word(wd,i);
        if(IS(w,"mesh"))
        {
            m = (MESH*)get_asset(ass,get_word(wd,i+1));
        }
        else if(IS(w,"tex"))
        {
           bmp = (BITMAP*)get_asset(ass,get_word(wd,i+1));
        }
        else if(IS(w,"pos"))
        {
           pos.x = strtof(get_word(wd,i+1), NULL);
           pos.y = strtof(get_word(wd,i+2), NULL);
           pos.z = strtof(get_word(wd,i+3), NULL);
        }
        else if(IS(w,"scale"))
        {
           scale.x = strtof(get_word(wd,i+1), NULL);
           scale.y = strtof(get_word(wd,i+2), NULL);
           scale.z = strtof(get_word(wd,i+3), NULL);
        }
        else if(IS(w,"add"))
        {
            dec[decCount] = new_decoration(pos,scale,m,bmp);
            ++ decCount;
        }

    }

    return decCount;
}


// Player-decorations collisions
void player_decoration_collision(PLAYER* pl, DECORATION* dec, int count)
{
    int i = 0;
    for(; i < count; ++ i)
    {
        pl_mesh_collision(pl,dec[i].mesh,dec[i].pos,dec[i].scale);
    }
}