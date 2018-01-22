/**
 * Mesh
 * (source file)
 *
 * @author Jani Nykänen
 * @version 1.0.0
 */

#include "mesh.h"

#include "../lib/parseword.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"


// Count occurrances of a word in a word data
static Uint32 count_word(WORDDATA* w, const char* word)
{
    Uint32 count = 0;
    int i = 0;
    for(; i < w->wordCount; ++ i)
    {
        if(strcmp(word,get_word(w,i)) == 0)
        {
            ++ count;
        }
    }

    return count;
}


// Get the first occurrance of a word
static Uint32 get_first_occurrance(WORDDATA* w, const char* word)
{
    int i = 0;
    for(; i < w->wordCount; ++ i)
    {
        if(strcmp(word,get_word(w,i)) == 0)
        {
            return i;
        }
    }

    return 0;
}


// Read three numbers to an array
static void read_num_3(WORDDATA* w, float* arr, Uint32 start, Uint32 length)
{
    int i = start +1;
    int index = 0;
    for(; index < length*3; i += 4)
    {
        arr[index] = (float)strtof(get_word(w,i),NULL);
        arr[index +1] = (float)strtof(get_word(w,i+1),NULL);
        arr[index +2] = (float)strtof(get_word(w,i+2),NULL);
        index += 3;
    }
}


// Read two numbers to an array
static void read_num_2(WORDDATA* w, float* arr, Uint32 start, Uint32 length)
{
    int i = start +1;
    int index = 0;
    for(; index < length*2; i += 3)
    {
        arr[index] = (float)strtof(get_word(w,i),NULL);
        arr[index +1] = (float)strtof(get_word(w,i+1),NULL);
        index += 2;
    }
}


// Parse "slashed" string
static void parse_slash_string(const char* word, Uint32* arr, Uint32 start)
{
    char buff[32];
    int index = 0;
    int windex = 0;

    int i = 0;
    for(; i <= strlen(word); ++i)
    {
        if(word[i] == '/' || word[i] == 0)
        {
            buff[index] = 0;
            arr[start + windex] = (int)strtol(buff,(char**)NULL,10);
            index = 0;
            ++ windex;
        }
        else
        {
            buff[index] = word[i];
            ++ index;
        }
    }
}


// Read 3 x 3 data (a/b/c format)
static void read_int_3_3(WORDDATA* w, Uint32* arr, Uint32 start, Uint32 length)
{
    int i = start +1;
    int index = 0;
    for(; index < length*3; i += 4)
    {
        parse_slash_string(get_word(w,i),arr,index * 3);
        parse_slash_string(get_word(w,i +1),arr,(index+1) * 3);
        parse_slash_string(get_word(w,i +2),arr,(index+2) * 3);

        index += 3;
    }
}


// Load mesh
MESH* load_mesh(const char* path)
{
    // Allocate memory
    WORDDATA* w = parse_file(path);
    if(w == NULL)
    {
        char err[256];
        snprintf(err,256,"Failed to load an OBJ file in %s!",path);
        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR,"Error!",err,NULL);
        return NULL;
    }

    // Count words
    Uint32 vertexCount = count_word(w,"v");
    Uint32 uvCount = count_word(w,"vt");
    Uint32 normalCount = count_word(w,"vn");
    Uint32 indexCount = count_word(w,"f");

    // Allocate memory for "raw" (non-indexed data)
    float* vertices = (float*)malloc(sizeof(float) * vertexCount * 3);
    float* uvs = (float*)malloc(sizeof(float) * uvCount * 2);
    float* normals = (float*)malloc(sizeof(float) * normalCount * 3);
    Uint32* indices = (Uint32*)malloc(sizeof(Uint32) * indexCount * 3 * 3);
    if(vertices == NULL || uvs == NULL || normals == NULL || indices == NULL)
    {
        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR,"Error!","Memory allocation error!\n",NULL);
        return NULL;
    }

    // Read data
    read_num_3(w,vertices,get_first_occurrance(w,"v"),vertexCount);
    read_num_3(w,normals,get_first_occurrance(w,"vn"),normalCount);
    read_num_2(w,uvs,get_first_occurrance(w,"vt"),uvCount);
    read_int_3_3(w,indices,get_first_occurrance(w,"f"),indexCount);

    // Allocate memory for actual mesh data
    MESH * m = (MESH*)malloc(sizeof(MESH));
    if(m == NULL)
    {
        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR,"Error!","Memory allocation error!\n",NULL);
        return NULL;
    }
    Uint32 elementCount = indexCount * 3;
    m->vertices = (float*)malloc(sizeof(float) * elementCount * 3);
    m->uvs = (float*)malloc(sizeof(float) * elementCount * 2);
    m->normals = (float*)malloc(sizeof(float) * elementCount * 3);
    m->indices = (Uint32*)malloc(sizeof(Uint32) * elementCount);
    if(m->vertices == NULL || m->uvs == NULL || m->normals == NULL || m->indices == NULL)
    {
        free(m);
        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR,"Error!","Memory allocation error!\n",NULL);
        return NULL;
    }

    m->vertexCount = elementCount * 3;
    m->uvCount = elementCount * 2;
    m->normalCount = elementCount * 3;
    m->elementCount = elementCount;

    m->minV = vec3(9999,9999,9999);
    m->maxV = vec3(-9999,-9999,-9999);

    // Store indexed data
    int i = 0;
    for(; i < elementCount; ++ i)
    {
        m->vertices[i*3] = vertices[ (indices[i*3]-1)*3 ];
        m->vertices[i*3 +1] = vertices[ (indices[i*3]-1)*3 +1];
        m->vertices[i*3 +2] = vertices[ (indices[i*3]-1)*3 +2];

        if(m->vertices[i*3] < m->minV.x) m->minV.x = m->vertices[i*3];
        if(m->vertices[i*3 +1] < m->minV.y) m->minV.y = m->vertices[i*3 +1];
        if(m->vertices[i*3 +2] < m->minV.z) m->minV.z = m->vertices[i*3 +2];

        if(m->vertices[i*3] > m->maxV.x) m->maxV.x = m->vertices[i*3];
        if(m->vertices[i*3 +1] > m->maxV.y) m->maxV.y = m->vertices[i*3 +1];
        if(m->vertices[i*3 +2] > m->maxV.z) m->maxV.z = m->vertices[i*3 +2];

        m->uvs[i*2] = uvs[ (indices[i*3 +1] -1)*2];
        m->uvs[i*2 +1] = 1.0f- uvs[ (indices[i*3 +1] -1)*2 +1];

        m->normals[i*3] = normals[ (indices[i*3 +2]-1)*3 ];
        m->normals[i*3 +1] = normals[ (indices[i*3 +2]-1)*3 +1];
        m->normals[i*3 +2] = normals[ (indices[i*3 +2]-1)*3 +2];

        m->indices[i] = i;
    }

    // Free data that is no longer needed
    free(vertices);
    free(uvs);
    free(normals);
    free(indices);

    return m;
}


// Destroy
void destroy_mesh(MESH* m)
{
    if(m == NULL) return;

    free(m->vertices);
    free(m->uvs);
    free(m->normals);
    free(m->indices);

    free(m);
}