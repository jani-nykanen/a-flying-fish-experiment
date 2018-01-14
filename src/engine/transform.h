/// Transformation (source)
/// (c) 2018 Jani Nykänen

#ifndef __TRANSFORM__
#define __TRANSFORM__

#include "vector.h"

/// Identity
void tr_identity();

/// Translate world
/// < x X coordinate
/// < y Y coordinate
/// < z Z coordinate
void tr_translate(float x, float y, float z);

/// Translate model
/// < x X coordinate
/// < y Y coordinate
/// < z Z coordinate
void tr_translate_model(float x, float y, float z);

/// Rotate world
/// < angle1 Rotation angle 1
/// < angle2 Rotation angle 2
void tr_rotate_world(float angle1, float angle2);

/// Rotate model
/// < angle1 Rotation angle 1
/// < angle2 Rotation angle 2
/// < angle3 Rotation angle 3
void tr_rotate_model(float angle1, float angle2, float angle3);

/// Rotate a normal (or any) vector
/// < n Vector to rotate
/// > A rotated vector
VEC3 tr_rotate_normal(VEC3 n);

/// Scale model
/// < x X scale
/// < y Y scale
/// < z Z scale
void tr_scale_model(float x, float y, float z);

/// Set FOV value
/// < value Value (0.75f is default)
void tr_set_fov(float value);

/// Return translation
/// > Translation vector
VEC3 tr_get_translation();

/// Return rotation
/// > Rotation vector
float tr_get_angle();

/// Use transformations for vector p
/// < p Vector
/// > A transformed vector
VEC3 tr_use_transform(VEC3 p);

/// Use transformation, y translation only
/// < p Vector
/// > A transformed vector
VEC3 tr_use_transform_ytrans(VEC3 p);

#endif // __TRANSFORM__