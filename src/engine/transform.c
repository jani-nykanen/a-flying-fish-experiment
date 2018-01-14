/// Transformation (source)
/// (c) 2018 Jani Nykänen

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "transform.h"

/// World translation
static VEC3 tr;
/// Model translation
static VEC3 modelTr;

/// World angle ("horizontal")
static float worldAngle1;
/// World angle ("vertical")
static float worldAngle2;
/// Is the world angle changed
static bool worldChanged;

/// Model angle ("horizontal")
static float modelAngle1;
/// Model angle ("vertical")
static float modelAngle2;
/// Model angle ("the third one")
static float modelAngle3;
/// Is the model angle changed
static bool modelChanged;

/// World sine cosine
static float wsc[4];
/// Model sine cosine
static float msc[6];

/// Model scale
static VEC3 modelScale;

/// FOV value (not actually fov, but presents the same thing)
static float FOVvalue = 0.75f;

/// Identitiy model matrix
void tr_identity()
{
    tr.x = 0.0f;
    tr.y = 0.0f;
    tr.z = 0.0f;

    modelTr = vec3(0.0f,0.0f,0.0f);

    worldAngle1 = 0.0f;
    worldAngle2 = 0.0f;

    modelAngle1 = 0.0f;
    modelAngle2 = 0.0f;
    modelAngle3 = 0.0f;

    worldChanged = true;
    modelChanged = true;

    modelScale.x = 1.0f;
    modelScale.y = 1.0f;
    modelScale.z = 1.0f;
}

/// Translate model matrix
void tr_translate(float x, float y, float z)
{
    tr.x = x;
    tr.y = y;
    tr.z = z;
}

/// Translate model
void tr_translate_model(float x, float y, float z)
{
    modelTr.x = x;
    modelTr.y = y;
    modelTr.z = z;
}

/// Rotate world
void tr_rotate_world(float angle1, float angle2)
{
    worldAngle1 += angle1;
    worldAngle2 += angle2;

    worldChanged = true;
}

/// Rotate a normal (or any) vector
VEC3 tr_rotate_normal(VEC3 n)
{
    if(modelChanged)
    {
        msc[0] = sin(modelAngle1);
        msc[1] = cos(modelAngle1);
        msc[2] = sin(modelAngle2);
        msc[3] = cos(modelAngle2);
        msc[4] = sin(modelAngle3);
        msc[5] = cos(modelAngle3);

        modelChanged = false;
    }

    // Rotate model
    float x = n.x;
    float z = n.z;
    float y = n.y;
    n.x = x * msc[1] - z * msc[0];
    n.z = x * msc[0] + z * msc[1] ;
    z = n.z;
    n.y = y * msc[3] - z * msc[2];
    n.z = y * msc[2] + z * msc[3] ;
    x = n.x;
    y = n.y;
    n.x = x * msc[5] - y * msc[4];
    n.y = x * msc[4] + y * msc[5] ;

    return n;
}

/// Rotate model
void tr_rotate_model(float angle1, float angle2, float angle3)
{
    modelAngle1 += angle1;
    modelAngle2 += angle2;
    modelAngle3 += angle3;

    modelChanged = true;
}

/// Scale model
void tr_scale_model(float x, float y, float z)
{
    modelScale = vec3(x,y,z);
}

/// Set FOV value
void tr_set_fov(float value)
{
    FOVvalue = value;
}

/// Return translation
VEC3 tr_get_translation()
{
    return tr;
}

/// Return rotation
float tr_get_worldAngle1()
{
    return worldAngle1;
}

/// Use transformations for vector p
VEC3 tr_use_transform(VEC3 p)
{
    // Calculate sines & cosines
    if(worldChanged)
    {
        wsc[0] = sin(worldAngle1);
        wsc[1] = cos(worldAngle1);
        wsc[2] = sin(worldAngle2);
        wsc[3] = cos(worldAngle2);

        worldChanged = false;
    }
    if(modelChanged)
    {
        msc[0] = sin(modelAngle1);
        msc[1] = cos(modelAngle1);
        msc[2] = sin(modelAngle2);
        msc[3] = cos(modelAngle2);
        msc[4] = sin(modelAngle3);
        msc[5] = cos(modelAngle3);

        modelChanged = false;
    }

    // Rotate model
    float x = p.x;
    float z = p.z;
    float y = p.y;
    p.x = x * msc[1] - z * msc[0];
    p.z = x * msc[0] + z * msc[1] ;
    z = p.z;
    p.y = y * msc[3] - z * msc[2];
    p.z = y * msc[2] + z * msc[3] ;
    x = p.x;
    y = p.y;
    p.x = x * msc[5] - y * msc[4];
    p.y = x * msc[4] + y * msc[5] ;

    p.x *= modelScale.x;
    p.y *= modelScale.y;
    p.z *= modelScale.z;

    // Translate
    VEC3 pt = vec3(p.x+modelTr.x+tr.x,p.y+modelTr.y+tr.y,p.z+modelTr.z+tr.z);

    // Rotate
    x = pt.x;
    z = pt.z;
    y = pt.y;
    pt.x = x * wsc[1] - z * wsc[0];
    pt.z = x * wsc[0] + z * wsc[1] ;
    z = pt.z;
    pt.y = y * wsc[3] - z * wsc[2];
    pt.z = y * wsc[2] + z * wsc[3] ;

    pt.z *= FOVvalue;

    return pt;
}

/// Use transform (ytrans only)
VEC3 tr_use_transform_ytrans(VEC3 p)
{
    VEC3 pt = vec3(p.x,p.y+tr.y,p.z);
    pt.z *= FOVvalue;

    return pt;
}