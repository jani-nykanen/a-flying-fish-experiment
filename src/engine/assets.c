/// Assets (source)
/// (c) 2018 Jani Nykänen

#include "assets.h"

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

#include "../lib/parseword.h"
#include "../lib/tmxc.h"

#include "bitmap.h"
#include "mesh.h"

/// Asset type enum
enum
{
    T_BITMAP = 0,
    T_TILEMAP = 1,
    T_MESH = 2,
};

// Global file path
static char* filePath;
// Current type
static int assetType;

// Calculate assets
static Uint32 calculate_assets(WORDDATA* w)
{
    Uint32 count = 0;
    int i = 0;
    char* word;
    bool begun = false;
    int index = 0;
    for(; i < w->wordCount; ++i)
    {
        word = get_word(w,i);
        if(!begun &&strcmp(word,"{") == 0)
        {
            begun = true;
            continue;
        }
        else if(begun &&strcmp(word,"}") == 0)
        {
            begun = false;
            continue;
        }

        if(begun)
        {
            if(index == 0) ++ count;
            index = !index;
        }
    }

    return count;
}

// Parse command type
static void parse_command_type(char* w1, char* w2)
{
    if(strcmp(w1,"@path") == 0)
    {
        filePath = w2;
        
    }
    else if(strcmp(w1,"@type") == 0)
    {
        if(strcmp(w2,"bitmap") == 0)
        {
            assetType = T_BITMAP;
        }
        else if(strcmp(w2,"tilemap") == 0)
        {
            assetType = T_TILEMAP;
        }
        else if(strcmp(w2,"mesh") == 0)
        {
            assetType = T_MESH;
        }
    }
}

// Load
ASSET_PACK* load_asset_pack(const char* path)
{
    // Allocate memory
    ASSET_PACK* p = malloc(sizeof(ASSET_PACK));
    if(p == NULL)
    {
        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR,"Error!","Memory allocation error!\n",NULL);
        return NULL;
    }

    // Load word data
    WORDDATA* w = parse_file(path);
    if(w == NULL)
    {
        free(p);
        return NULL;
    }

    filePath = NULL;
    assetType = 0;

    // Calculate assets
    p->assetCount = calculate_assets(w);
    
    // Allocate more memory
    p->names = (NAME*)malloc(sizeof(NAME) * p->assetCount);
    p->objects = (ANY*)malloc(sizeof(ANY) * p->assetCount);
    p->types = (int*)malloc(sizeof(int) * p->assetCount);
    if(p->names == NULL || p->objects == NULL || p->types == NULL)
    {
        free(p);
        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR,"Error!","Memory allocation error!\n",NULL);
        return NULL;
    }

    // Read words
    int i = 0;
    char* w1,*w2;
    char* op[2];
    int opIndex = 0;
    bool begun = false;
    int index = 0;
    
    for(; i < w->wordCount; ++ i)
    {   
        w1 = get_word(w,i);
        if(!begun)
        {
            if(w1[0] == '@')
            {
                w2 = get_word(w,++i);
                parse_command_type(w1,w2);
            }
            else if(strcmp(w1,"{") == 0)
            {
                begun = true;
            }
        }
        else
        {
            if(strcmp(w1,"}") == 0)
            {
                begun = false;
                continue;
            }
            
            op[opIndex] = w1;
            opIndex = !opIndex;
            if(opIndex == 0)
            {
                char path[1024];
                snprintf(path,1024,"%s%s",filePath,op[1]);

                if(assetType == T_BITMAP)
                {
                    p->objects[index] = (ANY)load_bitmap(path);
                }
                else if(assetType == T_TILEMAP)
                {
                    p->objects[index] = (ANY)load_tilemap(path);
                }
                else if(assetType == T_MESH)
                {
                    p->objects[index] = (ANY)load_mesh(path);
                }
                
                p->types[index] = assetType;
                strcpy(p->names[index].data,op[0]);
                if(p->objects[index] == NULL)
                {
                    free(p->objects);
                    free(p->names);
                    free(p->types);
                    free(p);
                    return NULL;
                }
                ++ index;
            }
        }
    }

    return p;
}


// Get asset
ANY get_asset(ASSET_PACK* p, const char* name)
{
    int i = 0;
    for(; i < p->assetCount; ++ i)
    {
        if(strcmp(name,p->names[i].data) == 0)
        {
            return p->objects[i];
        }
    }

    return NULL;
}


// Destroy
void destroy_asset_pack(ASSET_PACK* p)
{
    int i = 0;
    ANY obj;
    for(; i < p->assetCount; ++ i)
    {   
        obj = p->objects[i];
        switch(p->types[i])
        {
        case T_BITMAP:
            destroy_bitmap((BITMAP*)obj);
            break;
        case T_TILEMAP:
            destroy_tilemap((TILEMAP*)obj);
            break;
        case T_MESH:
            destroy_mesh((MESH*)obj);
            break;

        default:
            break;
        }
    }
}