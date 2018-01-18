/// Graphics (header)
/// (c) 2018 Jani Nykänen

#ifndef __GRAPHICS__
#define __GRAPHICS__

#include "stdbool.h"

#include "bitmap.h"
#include "frame.h"
#include "vector.h"
#include "mesh.h"

/// Flipping enumerations
enum
{
    FLIP_NONE = 0,
    FLIP_HORIZONTAL = 1,
    FLIP_VERTICAL = 2,
    FLIP_BOTH = 3,
};

/// Initialize graphics
void init_graphics();

/// Set the global renderer
/// < rend Renderer
void set_global_renderer(SDL_Renderer* rend);

/// Returns the global renderer
SDL_Renderer* get_global_renderer();

/// Clear screen
/// < r Red
/// < g Green
/// < b Blue
void clear(unsigned char r, unsigned char g, unsigned char b);

/// Set window dimensions
/// < w Width
/// < h Height
void set_dimensions(int w, int h);

/// Returns window dimensions
/// > Window dimensions
SDL_Point get_dimensions();

/// Bind frame so it can be used globally
/// < fr Frame to bind
void bind_frame(FRAME* fr);

/// Return currently used frame
/// > Frame
FRAME* get_current_frame();

/// Clear frame
/// < index Palette index
void clear_frame(Uint8 index);

/// Draw a non-scaled bitmap
/// < b Bitmap to be drawn
/// < dx X coordinate
/// < dy Y coordinate
/// < flip Flip
void draw_bitmap(BITMAP* b, int dx, int dy, int flip);

/// Draw a non-scaled bitmap with inverted colors
/// < b Bitmap to be drawn
/// < dx X coordinate
/// < dy Y coordinate
/// < flip Flip
void draw_inverted_bitmap(BITMAP* b, int dx, int dy, int flip);

/// Draw a rotated bitmap area
/// < b Bitmap to be drawn
/// < trx X translation
/// < try Y translation
/// < skip Skipping value
/// < angle Rotation angle
void draw_rotated_bitmap_area(BITMAP* b, float trx, float _try, int skip, float angle);

/// Draw a bitmap region
/// < b Bitmap to be drawn
/// < sx Source X
/// < sy Source Y
/// < sw Source W
/// < sh Source H
/// < dx X coordinate
/// < dy Y coordinate
/// < flip Flip
void draw_bitmap_region(BITMAP* b, int sx, int sy, int sw, int sh, int dx, int dy, int flip);

/// Draw a skipped bitmap region
/// < b Bitmap to be drawn
/// < sx Source X
/// < sy Source Y
/// < sw Source W
/// < sh Source H
/// < dx X coordinate
/// < dy Y coordinate
/// < skipx Skip x pixels
/// < skipy Skip y pixels
/// < flip Flip
void draw_skipped_bitmap_region(BITMAP* b, int sx, int sy, int sw, int sh, int dx, int dy, int skipx, int skipy, int flip);

/// Draw a bitmap region
/// < b Bitmap to be drawn
/// < sx Source X
/// < sy Source Y
/// < sw Source W
/// < sh Source H
/// < dx X coordinate
/// < dy Y coordinate
/// < dw Destination w
/// < dh Destination h
void draw_scaled_bitmap_region(BITMAP* b, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh);

/// Draw text using a bitmap font
/// < b Bitmap font
/// < text Text
/// < len Text length
/// < dx Destination x
/// < dy Destination y
/// < xoff X offset
/// < yoff Y offset
/// < center Center text
void draw_text(BITMAP* b, Uint8* text, int len, int dx, int dy, int xoff, int yoff, bool center);

/// Fill rectangle
/// < x X coordinate
/// < y Y coordinate
/// < w Width
/// < h Height
/// < index Color index
void fill_rect(int x, int y, int w, int h, Uint8 index);

/// Fill a skipped rectangle
/// < x X coordinate
/// < y Y coordinate
/// < w Width
/// < h Height
/// < skipx Skip horizontal pixels
/// < skipy Skip vertical pixels
/// < index Color index
void fill_skipped_rect(int x, int y, int w, int h, int skipx, int skipy, Uint8 index);

/// Draw a line
/// < x1 X coordinate 1
/// < y1 Y coordinate 1
/// < x2 X coordinate 2
/// < y2 Y coordinate 2
/// < color Color index
void draw_line(int x1, int y1, int x2, int y2, Uint8 color);

/// Draw a textured triangle
/// < x1 X coordinate 1
/// < y1 Y coordinate 1
/// < x2 X coordinate 2
/// < y2 Y coordinate 2
/// < x3 X coordinate 3
/// < y3 Y coordinate 3
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3);

/// Draw a filled "float" triangle (i.e. coordinates in [-1.0,1.0]×[-1.0,1.0])
/// < x1 X coordinate 1
/// < y1 Y coordinate 1
/// < x2 X coordinate 2
/// < y2 Y coordinate 2
/// < x3 X coordinate 3
/// < y3 Y coordinate 3
void draw_triangle_float(float x1, float y1, float x2, float y2, float x3, float y3);

/// Draw a filled triangle in 3D space
/// < a Vertice A
/// < b Vertice B
/// < c Vertice C
/// < tA Tex coord A
/// < tB Tex coord B
/// < tC Tex coord C
/// < n Normal
void draw_triangle_3d(VEC3 a, VEC3 b, VEC3 c, VEC2 tA, VEC2 tB, VEC2 tC, VEC3 n);

/// "Clear" triangle buffer
void clear_triangle_buffer();

/// Draw triangle buffer
void draw_triangle_buffer();

/// Set translation
/// < x Horizontal
/// < y Vertical
void set_translation(int x, int y);

/// Set alpha color index
/// < Index
void set_alpha(Uint8 index);

/// Get alpha color index
/// > Index
Uint8 get_alpha();

/// Bind a texture
/// < tex Texture to bind
void bind_texture(BITMAP* tex);

/// Bind uv coordinates
/// < x1 X coordinate 1
/// < y1 Y coordinate 1
/// < x2 X coordinate 2
/// < y2 Y coordinate 2
/// < x3 X coordinate 3
/// < y3 Y coordinate 3
void set_uv(float x1, float y1, float x2, float y2, float x3, float y3);

/// Draw mesh
/// < m Mesh to drawW
void draw_mesh(MESH* m);

/// Toggle lighting
/// < state On/off state
void toggle_lighting(bool state);

/// Set lighting
/// < dir Direction
/// < mag Magnitude
void set_ligthing(VEC3 dir, float mag);

/// Toggle darkness
/// < state On/off state
void toggle_darkness(bool state);

/// Set darkness
/// < min Minimum
/// < max Maximum
void set_darkness(float min, float max);

/// Set near/far plane
/// < near Near
/// < far Far
void set_near_far_planes(float near, float far);

#endif // __GRAPHICS__
