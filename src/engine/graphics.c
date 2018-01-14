/// Graphics (source)
/// (c) 2018 Jani Nykänen

#include "graphics.h"

#include "mathext.h"
#include "transform.h"

#include "malloc.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"


// Triangle type
typedef struct
{
    VEC3 A,B,C; // Vertices
    VEC2 tA,tB,tC; // Texture coordinates
    BITMAP* tex; // Texture
    float depth; // Depth value
    bool light; // Light enabled
    VEC3 normal; // Normal
}
_TRIANGLE;

// Max darkness value
#define MAX_DARKNESS_VALUE 8

// Min depth
static const float DEPTH_MIN = 0.025f;

// Size of triangle buffer
#define TBUFFER_SIZE 1024
// Triangle buffer
static _TRIANGLE tbuffer[TBUFFER_SIZE];
// Which triangles are drawn
static bool tdrawn[TBUFFER_SIZE];
// Triangle index pointer
static int tindex;

// Global renderer
static SDL_Renderer* grend;
// Global frame
static FRAME* gframe;
// Window dim
static SDL_Point windowDim;

// Translate x
static int transX;
// Translate y
static int transY;

// Alpha pixel
static Uint8 alpha = 170;

// Put pixel function
static void (*ppfunc) (int,int,Uint8);

// Global texture used in drawing filled polygons
static BITMAP* gtex;

// Matrix
static MAT2 invM;
// UV coords
static VEC2 uv1, uv2, uv3;
// UV translation
static VEC2 UVtrans;
// Used normal
static VEC3* usedNormal;
// Light value
static int lightVal; 
// Light enabled
static bool lightEnabled;
// Light direction
static VEC3 lightDir;
// Light magnitude
static float lightMag;

// Light palettes
static Uint8 lpalettes[MAX_DARKNESS_VALUE] [256];


// Calculate darkened color index
static Uint8 calc_darkened_color(Uint8 col, int amount)
{
    Uint8 r,g,b;

    r = col >> 5;
    g = col << 3;
    g = g >> 5;
    b = col << 6;
    b = b >> 6; 

    int i = 0;
    for(; i < amount; ++i)
    {
        if(r>0) -- r;
        if(g>0) -- g;
        
        if(i % 2 == 1)
        {
            if(b > 0) --b;
        }
    }

    r = r << 5;
    g = g << 2;

    return r | g | b;
}


// Generate light palettes
static void gen_light_palettes()
{
    int i1 = 0, i2;

    for(; i1 < MAX_DARKNESS_VALUE; ++ i1)
    {
        for(i2 = 0; i2 < 256; ++ i2)
        {
            lpalettes[i1][i2] = calc_darkened_color(i2,i1);
        }
    }
}

// Put pixel to the screen
static void put_pixel(int x, int y, Uint8 index)
{
    if(index == alpha || x < 0 || y < 0 || x >= gframe->w || y >= gframe->h) return;
    gframe->colorData[y*gframe->w+x] = index;
}


/// Put pixel to the screen (with darkness enabled)
/// < x X coordinate
/// < y Y coordinate
/// < index Color index
static void put_pixel_dark(int x, int y, Uint8 index)
{
    if(index == 255 || x < 0 || y < 0 || x >= gframe->w || y >= gframe->h) return;

    if(lightVal > 0)
    {
        Uint8 col;
        if(lightVal % 2 == 0)
        {
            col = lpalettes[lightVal/2][index];
        }
        else
        {
            if( (x % 2 == 0 && y % 2 == 0) || (x % 2 == 1 && y % 2 == 1) )
                col = lpalettes[lightVal/2][index];
            else
                col = lpalettes[lightVal/2+1][index];
        }
        gframe->colorData[y*gframe->w+x] = col;
        return;
    }

    gframe->colorData[y*gframe->w+x] = index;
}


/// Calculate darkness value of lighting
static int calculate_ligthing_value(VEC3 normal)
{

    normal = tr_rotate_normal(normal);

    float multiplier = maxf( 0.0f, normal.x*lightDir.x+normal.y*lightDir.y+normal.z*lightDir.z);
    multiplier = (1.0f-lightMag) + lightMag * multiplier;

    return (int)floor( (1.0f-multiplier) / (1.0f/ ( (float)2*MAX_DARKNESS_VALUE)) );
}


// Generate inverse matrix
static void gen_matrix(int x1, int y1, int x2, int y2, int x3, int y3)
{
    BITMAP* b = gtex;

    // UV matrix
    MAT2 uv = mat2(
        (uv3.x-uv1.x), (uv2.x-uv1.x),
        (uv3.y-uv1.y), (uv2.y-uv1.y)
    );
    MAT2 uvInv = mat2_inverse(uv);

    // U ja V vectors
    VEC2 u = vec2((x3-x1),(y3-y1));
    VEC2 v = vec2((x2-x1),(y2-y1));

    // Basis
    MAT2 basis = mat2(
        u.x, v.x,
        u.y, v.y
    );

    // Scale
    MAT2 scale = mat2(
        1.0f/b->w,0.0f,
        0.0f, 1.0/b->h
    );

    UVtrans = vec2(uv1.x * b->w, uv1.y * b->h);

    // Final matrix
    MAT2 m = mat2_mul(basis,uvInv);
    m = mat2_mul(scale,m);

    // Inverse matrix
    invM = mat2_inverse(m);
}


// Initialize graphics
void init_graphics()
{
    ppfunc = put_pixel;

    transX = 0;
    transY = 0;

    lightDir = vec3(0,0,-1);
    lightMag = 1.0f;
    lightEnabled = false;

    gen_light_palettes();
}


// Set global renderer
void set_global_renderer(SDL_Renderer* rend)
{
    grend = rend;
}


// Get grend
SDL_Renderer* get_global_renderer()
{
    return grend;
}


// Clear screen
void clear(unsigned char r, unsigned char g, unsigned char b)
{
    SDL_SetRenderDrawColor(grend, r,g,b, 255);
    SDL_RenderClear(grend);
}


// Set window dimensions
void set_dimensions(int w, int h)
{
    windowDim.x = w;
    windowDim.y = h;
}


// Get window dimensions
SDL_Point get_dimensions()
{
    return windowDim;
}


// Bind frame
void bind_frame(FRAME* fr)
{
    gframe = fr; 
}


// Return currently used frame
FRAME* get_current_frame()
{
    return gframe;
}


// Clear frame
void clear_frame(Uint8 index)
{
    memset(gframe->colorData,index,gframe->size);
}


// Draw a non-scaled bitmap
void draw_bitmap(BITMAP* b, int dx, int dy, int flip)
{
    dx += transX;
    dy += transY;

    int x; // Screen X
    int y = dy; // Screen Y
    int px = 0; // Pixel X
    int py = 0; // Pixel Y

    // Go though every pixel and put them to a frame
    for(; y < dy+b->h; y++)
    {
        for(x = dx; x < dx+b->w; x++)
        {
            ppfunc(x,y, (b->data[py*b->w +px]));
            px ++;
        }
        py ++;
        px = 0;
    } 
}


// Draw a non-scaled bitmap with inverted colors
void draw_inverted_bitmap(BITMAP* b, int dx, int dy, int flip)
{
    dx += transX;
    dy += transY;

    int x; // Screen X
    int y = dy; // Screen Y
    int px = 0; // Pixel X
    int py = 0; // Pixel Y

    Uint8 index;

    // Go though every pixel and put them to a frame
    for(; y < dy+b->h; y++)
    {
        for(x = dx; x < dx+b->w; x++)
        {
            index = b->data[py*b->w +px];
            index = ~index;
            index = index & 0b00111111;

            ppfunc(x,y, index);
            px ++;
        }
        py ++;
        px = 0;
    } 
}


// Draw a rotated bitmap area
void draw_rotated_bitmap_area(BITMAP* b,  float trx, float try, int skip, float angle)
{
    skip ++;

    // Rotation matrix B
    float b11 = cos(angle), b21 = -sin(angle);
    float b12 = sin(angle), b22 = cos(angle);

    // Inverse of determinant of B
    float detInv = 1.0f / (b11 * b22 - b12 * b21);

    // The inverse of matrix B
    float ib11 = detInv * (b22), ib21 = detInv * -b21;
    float ib12 = detInv * -(b12), ib22 = detInv * b22;

    // Texture coordinates in bitmap
    int tx = 0;
    int ty = 0;

    // "gframe" stands for "global frame",
    // so cx and cy point to the center of
    // the frame where this thing is drawn to
    int cx = gframe->w / 2;
    int cy = gframe->h / 2;

    // Translated coordinates
    int xx, yy;

    // Coordinates
    int x,y;

    // Color, obviously
    Uint8 color;

    // Draw pixels
    for(x = 0; x < gframe->w; x++)
    {
        for(y = 0; y < gframe->h; y++)
        {
            if(!(skip == 0 || (x % skip == 0 && y % skip == 0) ) )
                continue;

            // Translate point
            xx = x - cx;
            yy = y - cy;

            // Get texture coordinates
            tx = (int)(ib11 * xx + ib21 * yy +trx);
            ty = (int)(ib12 * xx + ib22 * yy +try);

            // If texture coords are outside the
            // texture area, force them back!
            while(tx >= b->w) tx -= b->w;
            while(ty >= b->h) ty -= b->h;
            while(tx < 0) tx += b->w;
            while(ty < 0) ty += b->h;

            
            color = b->data[ty*b->w +tx];
            ppfunc(x,y, color);
        }
    } 
}


// Draw a bitmap region
void draw_bitmap_region(BITMAP* b, int sx, int sy, int sw, int sh, int dx, int dy, int flip)
{
    dx += transX;
    dy += transY;

    int x; // Screen X
    int y = dy; // Screen Y
    int px = sx; // Pixel X
    int py = sy; // Pixel Y

    int beginx = flip == FLIP_NONE ? dx : dx+sw-1;
    int endx = flip == FLIP_NONE ? dx+sw : dx;
    int stepx = flip == FLIP_NONE ? 1 : -1;

    // Go though every pixel and put them to a frame
    for(; y < dy+sh; y++)
    {
        for(x = beginx; x != endx; x += stepx)
        {
            ppfunc(x,y, b->data[py*b->w +px]);

            px ++;
        }
        py ++;
        px = sx;
    } 
}


// Draw a skipped bitmap region
void draw_skipped_bitmap_region(BITMAP* b, int sx, int sy, int sw, int sh, int dx, int dy, int skipx, int skipy, int flip)
{

    dx += transX;
    dy += transY;

    int x; // Screen X
    int y = dy; // Screen Y
    int px = sx; // Pixel X
    int py = sy; // Pixel Y

    int beginx = flip == FLIP_NONE ? dx : dx+sw-1;
    int endx = flip == FLIP_NONE ? dx+sw : dx;
    int stepx = flip == FLIP_NONE ? (1) : -(1);

    int skipxCount = 0;
    int skipyCount = 0;

    // Go though every pixel and put them to a frame
    for(; y < dy+sh; y++)
    {
        skipxCount = 0;
        for(x = beginx; x != endx; x += stepx)
        {
            
            if(skipx == 0 || (skipxCount % skipx != 0 && (skipy == 0 || skipyCount % skipy != 0) ))
            {
                ppfunc(x,y, b->data[py*b->w +px]);
            }

            px ++;
            skipxCount ++;
        }
        skipyCount ++;
        py ++;
        px = sx;
    } 
}


// Draw a scaled bitmap line
void draw_scaled_bitmap_region(BITMAP* b, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh)
{
    dx += transX;
    dy += transY;

    int x; // Screen X
    int y = dy; // Screen Y
    int px = sx; // Pixel X
    int py = sy; // Pixel Y
    float pxf = (float)sx; // Pixel X (float)
    float pyf = (float)sy; // Pixel Y (float)

    float ssx = (float)dw/(float)sw;
    float ssy = (float)dh/(float)sh;

    // Go though every pixel and put them to a frame
    // if pixel is not out of frame range
    for(; y < dy+ (int)floor(dh ); y++)
    {
        for(x = dx; x < dx+ (int)floor(dw ); x++)
        {
            if(x < -1)
            {
                pxf += 1.0f/ssx * abs(x);
                x = -1;
                continue;
            }
            else if(x >= gframe->w)
                break;

            px = (int)(pxf);
            py = (int)(pyf);

            ppfunc(x,y, b->data[py*b->w +px]);
            pxf += 1.0f/ssx;
        }
        pyf += 1.0f/ssy;
        pxf = (float)sx;
    } 
}


// Draw text using a bitmap font
void draw_text(BITMAP* b, Uint8* text, int len, int dx, int dy, int xoff, int yoff, bool center)
{
    dx += transX;
    dy += transY;

    int x = dx;
    int y = dy;
    int cw = b->w / 16;
    int ch = b->h / 16;
    int i=0;
    Uint8 c;
    int sx;
    int sy;

    if(center)
    {
        x -= (int) ( ((float)len+1)/2.0f * (float)(cw+xoff) );
    }

    for(; i < len && text[i] != '\0'; i++)
    {
        c = text[i];
        if(c == '\n')
        {
            x = dx;
            y += yoff;
            continue;
        }

        sx = c % 16;
        sy = c / 16;

        draw_bitmap_region(b,sx*cw,sy*ch,cw,ch,x,y,0);

        x += cw + xoff;
    }
}


// Fill rectangle
void fill_rect(int x, int y, int w, int h, Uint8 index)
{
    x += transX;
    y += transY;

    int dx = x;
    int dy = y;

    for(; dy < y+h; dy++)
    {
        for(dx = x; dx < x+w; dx++)
        {
            ppfunc(dx,dy,index);
        }
    }
}


// Fill a skipped rectangle
void fill_skipped_rect(int x, int y, int w, int h, int skipx, int skipy, Uint8 index)
{
        x += transX;
    y += transY;

    int dx = x;
    int dy = y;

    int skipxCount = 0;
    int skipyCount = 0;

    // Go though every pixel and put them to a frame
    for(; dy < y+h; dy++)
    {
        for(dx = x; dx < x+w; dx++)
        {
            
            if(skipx == 0 || (skipxCount % skipx != 0 && (skipy == 0 || skipyCount % skipy != 0) ))
            {
                ppfunc(dx,dy,index);
            }
            skipxCount ++;
        }
        skipyCount ++;
    } 
}


// Draw a line
void draw_line(int x1, int y1, int x2, int y2, Uint8 color)
{
    x1 += transX;
    y1 += transY;
    x2 += transX;
    y2 += transY;

    // Bresenham's line algorithm
    int dx = abs(x2-x1), sx = x1<x2 ? 1 : -1;
    int dy = abs(y2-y1), sy = y1<y2 ? 1 : -1; 
    int err = (dx>dy ? dx : -dy)/2, e2;
     
    for(;;)
    {
        ppfunc(x1,y1, color);
        
        if (x1==x2 && y1==y2) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; x1 += sx; }
        if (e2 < dy) { err += dx; y1 += sy; }
    }
}


// Draw a textured triangle
static void _draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int spc)
{
    BITMAP* b = gtex;

    // Calculate minimums & maximums
    int maxy = max(y1,max(y2,y3));
    int miny = min(y1,min(y2,y3));
    int maxx = max(x1,max(x2,x3));
    int minx = min(x1,min(x2,x3));

    // Do not draw if not visible
    if(maxx < 0 || minx >= gframe->w || maxy < 0 || miny >= gframe->h 
        || (maxy == miny)) 
        return;

    // Sort points
    int py1 = miny;
    int px1 = y1 == py1 ? x1 : (y2 == py1 ? x2 : x3);
    int px2,py2;
    int px3, py3;
    if(py1 == y1 && px1 == x1)
    {
        px2 = x2; py2 = y2;
        px3 = x3; py3 = y3;
    }
    else if(py1 == y2 && px1 == x2)
    {
        px2 = x1; py2 = y1;
        px3 = x3; py3 = y3;
    }
    else
    {
        px2 = x1; py2 = y1;
        px3 = x2; py3 = y2;
    }

    // Swap p2 & p3 if certain conditions are met
    if( (spc == 0 && px3 < px2) || spc == 2)
    {
        int bx = px3;
        int by = py3;
        px3 = px2;
        py3 = py2;
        px2 = bx;
        py2 = by;
    }

    // Calculate steps
    float step1 = (py2 != py1) ? (float) (px2 - px1) / (float) (py2 - py1) : (px2-px1);
    float step2 = (py3 != py1) ? (float) (px3 - px1) / (float) (py3 - py1) : (px3-px1);
    float step3 = (py3 != py2) ? (float) (px3 - px2) / (float) (py3 - py2) : (px3-px2);

    int x,y;
    float startx = px1;
    float endx = px1;

    // Is the top or bottom flat
    bool flat = py1 == py2 || py2 == py3 || py1 == py3;

    Uint8 col = 63;

    // Texture coordinates in bitmap
    int tx = 0;
    int ty = 0;

    // Translated coordinates
    int xx, yy;

    // Draw visible pixels
    for(y = miny; y <= min(maxy,gframe->h); y++)
    {
        if(y >= 0)
        {
            for(x = max(0,(int)startx); x <= min(gframe->w,(int)endx); ++ x)
            {
                // Translate point
                xx = x - x1;
                yy = y - y1;

                // Get texture coordinates
                tx = (int)(invM.m11 * xx + invM.m21 * yy);
                ty = (int)(invM.m12 * xx + invM.m22 * yy);

                tx += UVtrans.x;
                ty += UVtrans.y;

                // If texture coords are outside the
                // texture area, force them back!
                while(tx >= b->w) tx -= b->w;
                while(ty >= b->h) ty -= b->h;
                while(tx < 0) tx += b->w;
                while(ty < 0) ty += b->h;

                col = b->data[ty*b->w +tx];
                ppfunc(x,y,col);
            }
        }

        if(!flat || y > miny)
        {
            if(y >= py3)
                step2 = step3;
            if(y >= py2)
                step1 = step3;
        }

        startx += step1;
        endx += step2;

        if(spc != 2 && startx-endx > 1)
        {
            _draw_triangle(x1,y1,x2,y2,x3,y3,spc+1);
            return;
        }
    }
}
// Draw a textured triangle (actual definition)
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    // Check if vectors are not linearly dependaple
    int ux = (x2-x1);
    int uy = (y2-y1);

    int vx = (x3-x1);
    int vy = (y3-y1);

    if(ux*vy - uy * vx == 0) return;

    gen_matrix(x1,y1,x2,y2,x3,y3);

    _draw_triangle(x1,y1,x2,y2,x3,y3,0);
}


// Draw a filled triangle in float coordinates
void draw_triangle_float(float x1, float y1, float x2, float y2, float x3, float y3)
{
    int h = gframe->h;
    int w = gframe->w;

    float mh = 2.0f;
    float mw = 2.0f * (float)w / (float)h;

    float ty = 1.0f;
    float tx = mw / 2.0f;

    int px1 = (int) ( (x1+tx)/mw * w);
    int py1 = (int) ( (y1+ty)/mh * h);
    int px2 = (int) ( (x2+tx)/mw * w);
    int py2 = (int) ( (y2+ty)/mh * h);
    int px3 = (int) ( (x3+tx)/mw * w);
    int py3 = (int) ( (y3+ty)/mh * h);

    draw_triangle(px1,py1,px2,py2,px3,py3);
}


// Draw a filled triangle in 3D space
void draw_triangle_3d(VEC3 a, VEC3 b, VEC3 c, VEC2 tA, VEC2 tB, VEC2 tC, VEC3 n)
{
    VEC3 ta = tr_use_transform(a);
    VEC3 tb = tr_use_transform(b);
    VEC3 tc = tr_use_transform(c);

    if(ta.z < DEPTH_MIN && tb.z < DEPTH_MIN && tc.z < DEPTH_MIN)
        return;

    if(ta.z < DEPTH_MIN)
        ta.z = DEPTH_MIN;

    if(tb.z < DEPTH_MIN)
        tb.z = DEPTH_MIN;
        
    if(tc.z < DEPTH_MIN)
        tc.z = DEPTH_MIN;

    ta.x /= ta.z; ta.y /= ta.z;
    tb.x /= tb.z; tb.y /= tb.z;
    tc.x /= tc.z; tc.y /= tc.z;

    // Calculate minimums & maximums
    float maxy = maxf(ta.y,maxf(tb.y,tc.y));
    float miny = minf(ta.y,minf(tb.y,tc.y));
    float maxx = maxf(ta.x,maxf(tb.x,tc.x));
    float minx = minf(ta.x,minf(tb.x,tc.x));

    float ratio = (float)gframe->w / (float) gframe->h;

    // Ignore if outside the view
    if(maxx < -ratio || minx > ratio || maxy < -1.0f || miny > 1.0f) 
    {
        return;
    }

    float depth = (ta.z+tb.z+tc.z)/3.0f;
    tbuffer[tindex] = (_TRIANGLE){ta,tb,tc,tA,tB,tC,gtex, depth,lightEnabled, n};
    tindex ++;
}


// "Clear" triangle buffer
void clear_triangle_buffer()
{
    tindex = 0;
}


// Draw triangle buffer
void draw_triangle_buffer()
{
    // Sort triangles by depth and draw
    int i = 0;
    _TRIANGLE t;

    for(; i < tindex; ++ i)
    {
        tdrawn[i] = false;
        t = tbuffer[i];
    }

    float maxDepth = 0.0f;
    int drawIndex = 0;
    int count = 0;

    int minIndex = 0;
    int maxIndex = tindex;

    while(true)
    {
        count = 0;
        maxDepth = 0.0f;
        for(int i = minIndex; i < maxIndex; ++ i)
        {
            t = tbuffer[i];
            if(!tdrawn[i] && t.depth > maxDepth)
            {
                maxDepth = t.depth;
                drawIndex = i;
                count ++;
            }
        }

        if(count == 0)
        {
            break;
        }
        else
        {
            if(drawIndex == minIndex)
                ++ minIndex;

            if(drawIndex == maxIndex -1)
                -- maxIndex;

            t = tbuffer[drawIndex];

            ppfunc = lightEnabled ? put_pixel_dark : put_pixel;
            lightVal = calculate_ligthing_value(t.normal);
            bind_texture(t.tex);
            set_uv(t.tA.x,t.tA.y,t.tB.x,t.tB.y,t.tC.x,t.tC.y);
            draw_triangle_float(t.A.x,t.A.y, t.B.x,t.B.y, t.C.x,t.C.y);

            tdrawn[drawIndex] = true;
        }
    }
    usedNormal = NULL;
    lightVal = 0;
    ppfunc = put_pixel;
    
}


// Set translation
void set_translation(int x, int y)
{
    transX = x;
    transY = y;
}


// Set alpha color index
void set_alpha(Uint8 index)
{
    alpha = index;
}


// Get alpha color index
Uint8 get_alpha()
{
    return alpha;
}


// Bind a texture
void bind_texture(BITMAP* tex)
{
    gtex = tex;
}

// Bind uv coordinates
void set_uv(float x1, float y1, float x2, float y2, float x3, float y3)
{
    const float DELTA = 0.001f;

    uv1 = vec2(x1,y1);
    uv2 = vec2(x2,y2);
    uv3 = vec2(x3,y3);

    // Check if vectors are not linearly dependaple
    float ux = (x2-x1);
    float uy = (y2-y1);

    float vx = (x3-x1);
    float vy = (y3-y1);

    if(fabs(ux*vy - uy * vx) < DELTA)
    {
        uv1 = vec2(0.0f,0.0f);
        uv2 = vec2(0.0001f,0.0f);  
        uv3 = vec2(0.0f,0.0001f);
    }
}


// Draw mesh
void draw_mesh(MESH* m)
{
    if(m == NULL) return;

    int i = 0;

    for(; i < m->elementCount; i += 3)
    {
        draw_triangle_3d(
            vec3(m->vertices[m->indices[i]*3],m->vertices[m->indices[i]*3 +1],m->vertices[m->indices[i]*3+2]),
            vec3(m->vertices[m->indices[i]*3 +3],m->vertices[m->indices[i]*3 +4],m->vertices[m->indices[i]*3+5]),
            vec3(m->vertices[m->indices[i]*3 +6],m->vertices[m->indices[i]*3 +7],m->vertices[m->indices[i]*3+8]),

            vec2(m->uvs[m->indices[i]*2],m->uvs[m->indices[i]*2 +1]),
            vec2(m->uvs[m->indices[i]*2 + 2],m->uvs[m->indices[i]*2 +3]),
            vec2(m->uvs[m->indices[i]*2 + 4],m->uvs[m->indices[i]*2 + 5]),

            vec3(m->normals[m->indices[i]*3],m->normals[m->indices[i]*3 +1],m->normals[m->indices[i]*3+2])
        );
    }
}


/// Toggle lighting
void toggle_lighting(bool state)
{
    lightEnabled = state;
}


/// Set lighting
void set_ligthing(VEC3 dir, float mag)
{
    lightDir = dir;
    lightMag = mag;
}