/// Vectors (source)
/// (c) 2018 Jani Nykänen

#include "vector.h"

#include "math.h"

/// Cross product
VEC3 cross(VEC3 A, VEC3 B)
{
    return vec3(
        A.y*B.z - A.z*B.y,
        -(A.x*B.z-A.z*B.x),
        A.x*B.y - A.y*B.x
    );
}

/// Addition operator
VEC3 add_vec3(VEC3 A, VEC3 B)
{
    return vec3(A.x+B.x,A.y+B.y,A.z+B.z);
}

/// Decrease operator
VEC3 dec_vec3(VEC3 A, VEC3 B)
{
    return vec3(A.x-B.x,A.y-B.y,A.z-B.z);
}

/// Normalize
VEC3 normalize(VEC3 A)
{
    float l = sqrt(A.x*A.x + A.y*A.y + A.z*A.z);
    return vec3(A.x/l,A.y/l,A.z/l);
}

/// Normalize a vector 2
VEC2 vec2_normalize(VEC2 A)
{
    float l = sqrt(A.x*A.x + A.y*A.y);
    return vec2(A.x/l,A.y/l);
}

/// Calculate determinant
float mat2_det(MAT2 m)
{
    return m.m11 * m.m22 - m.m12 * m.m21;
}

/// Inverse matrix
MAT2 mat2_inverse(MAT2 m)
{
    float d = 1.0f / mat2_det(m);
    MAT2 i;
    i.m11 = d * (m.m22); i.m21 = d * -m.m21;
    i.m12 = d * -(m.m12); i.m22 = d * m.m11;
    return i;
}

/// Matrix multiplication
MAT2 mat2_mul(MAT2 a, MAT2 b)
{
    MAT2 r;
    r.m11 =  a.m11 * b.m11 + a.m21 * b.m12;
    r.m21 =  a.m11 * b.m21 + a.m21 * b.m22;
    r.m12 =  a.m12 * b.m11 + a.m22 * b.m12;
    r.m22 =  a.m12 * b.m21 + a.m22 * b.m22;
    return r;
}

/// Multiple matrix with a vector
VEC2 mat2_mul_vec2(MAT2 m, VEC2 v)
{
    VEC2 r;
    r.x =  m.m11 * v.x + m.m21 * v.y;
    r.y =  m.m12 * v.x + m.m22 * v.y;

    return r;
}