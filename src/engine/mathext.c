/// Math extension (source)
/// (c) 2018 Jani Nykänen

#include "mathext.h"

#include "stdlib.h"
#include "math.h"
#include "stdio.h"


/// Max float
float maxf(float a, float b)
{
    return a >= b ? a : b;
}

/// Max int
int max(int a, int b)
{
    return a >= b ? a : b;
}

/// Min float
float minf(float a, float b)
{
    return a <= b ? a : b;
}

/// Min integer
int min(int a, int b)
{
    return a <= b ? a : b;
}

/// Is inside triangle
bool inside_triangle(float px, float py, float x1, float y1, float x2, float y2, float x3, float y3)
{
    /*
     * Explanation: https://stackoverflow.com/a/9755252
    */

    float as_x = px-x1;
    float as_y = py-y1;
 
    bool s_ab = (x2-x1)*as_y-(y2-y1)*as_x > 0;
 
    if( ((x3-x1)*as_y-(y3-y1)*as_x > 0) == s_ab) return false;
    if( ((x3-x2)*(py-y2)-(y3-y2)*(px-x2) > 0) != s_ab) return false;
 
    return true;
}