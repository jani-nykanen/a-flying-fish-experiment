/// Vectors & matrices (header)
/// (c) 2018 Jani Nykänen

#ifndef __VECTOR__
#define __VECTOR__

/// Vector 3 type
typedef struct
{
    float x,y,z;
}
VEC3;

/// Create new vector 3
/// < x X component
/// < y Y component
/// < z Z component
/// > A new vector 3
#define vec3(x,y,z) (VEC3){x,y,z}

/// Vector 2 type
typedef struct
{
    float x,y;
}
VEC2;

/// Point
typedef struct
{
    int x,y;
}
POINT;

/// 2x2 matrix
typedef struct
{
    float m11, m21;
    float m12, m22;
}
MAT2;

/// Create new vector 2
/// < x X component
/// < y Y component
/// > A new vector 2
#define vec2(x,y) (VEC2){x,y}


/// Create new 2x2 matrix
/// < b<j,k> Value in j,k
/// > A new 2x2 matrix
#define mat2(b11,b21,b12,b22) (MAT2){b11,b21,b12,b22}

/// Cross product
/// < A Vector 1
/// < B Vector 2
/// > A new vector
VEC3 cross(VEC3 A, VEC3 B);

/// Addition operator
/// < A Vector 1
/// < B Vector 2
/// > A new vector
VEC3 add_vec3(VEC3 A, VEC3 B);

/// Decrease operator
/// < A Vector 1
/// < B Vector 2
/// > A new vector
VEC3 dec_vec3(VEC3 A, VEC3 B);

/// Normalize
/// < A vector to normalize
/// > Normalized vector
VEC3 normalize(VEC3 A);

/// Normalize a vector 2
/// < A vector to normalize
/// > Normalized vector
VEC2 vec2_normalize(VEC2 A);

/// Calculate determinant
/// < m Matrix
/// > Determinant
float mat2_det(MAT2 m);

/// Inverse matrix
/// < m Matrix to inverse
/// > Inverse matrix
MAT2 mat2_inverse(MAT2 m);

/// Matrix multiplication
/// < a Left operand
/// < b Right operand
/// > Result matrix
MAT2 mat2_mul(MAT2 a, MAT2 b);

/// Multiple matrix with a vector
/// < m Matrix
/// < v Vector
/// > Result vector
VEC2 mat2_mul_vec2(MAT2 m, VEC2 v);

#endif // __VECTOR__