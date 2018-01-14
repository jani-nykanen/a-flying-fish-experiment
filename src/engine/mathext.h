/// Math extension (header)
/// (c) 2018 Jani Nykänen

#ifndef __MATH_EXT__
#define __MATH_EXT__

#include "math.h"
#include "stdbool.h"

/// Determine which one is bigger, a or b
/// a Number a
/// b Number b
/// Returns the bigger number
float maxf(float a, float b);

/// Same as maxf, but with integers
int max(int a, int b);

/// Determine which one is smaller, a or b
/// a Number a
/// b Number b
/// Returns the smaller number
float minf(float a, float b);

/// Same as minf, but with integers
int min(int a, int b);

/// Is point inside a triangle
/// < px Point x
/// < py Point y
/// < x1 X coordinate of the first point
/// < y1 Y coordinate of the first point
/// < x2 X coordinate of the second point
/// < y2 Y coordinate of the second point
/// < x3 X coordinate of the third point
/// < y3 Y coordinate of the third point
/// > True, if inside
bool inside_triangle(float px, float py, float x1, float y1, float x2, float y2, float x3, float y3);

#endif // __MATH_EXT__