/// Controls (header)
/// (c) 2018 Jani Nykänen

#ifndef __CONTROLS__
#define __CONTROLS__

#include <SDL2/SDL.h>

#include "vector.h"

#include "stdbool.h"

/// Key states
typedef enum
{
    UP = 0,
    DOWN = 1,
    PRESSED = 2,
    RELEASED = 3 
}
STATE;

/// Initialize controls
void ctr_init();

/// Update controls
void ctr_update();

/// Triggered when a key is pressed down
/// < scancode Key scancode
void ctr_on_key_down(SDL_Scancode scancode);

///Triggered when a key is released
/// < scancode Key scancode
void ctr_on_key_up(SDL_Scancode scancode);

/// Mouse move event
/// < x Mouse x
/// < y Mouse y
void ctr_on_mouse_move(int x, int y);

/// Mouse button pressed event
/// < button Button
void ctr_on_mouse_down(int button);

/// Mouse button released event
/// < button Button
void ctr_on_mouse_up(int button);

/// Mouse wheel event
/// < pos Wheel position
void ctr_on_mouse_wheel(int pos);

/// Joystick button pressed
/// < button Button
void ctr_on_joy_down(int button);

/// Joystick button released
/// < button Button
void ctr_on_joy_up(int button);

/// Joystick axis movement
/// < axis Axis id
/// < value Value
void ctr_on_joy_axis(int axis, float value);

/// Get a key state
/// < key Key
/// > Key state
STATE get_key_state(int key);

/// Get a mouse button state
/// < button Button
/// > A button state
STATE get_mouse_button_state(int button);

/// Get joystick button state
/// < button Button
/// > Button state
STATE get_joy_button_state(int button);

/// Returns cursor position
/// > Cursor position
SDL_Point get_cursor_pos();

/// Is any button pressed
/// > True, if pressed
bool any_pressed();

/// Is any button released
/// > True, if released
bool any_released();

/// Is any button released
/// > True, if down
bool any_down();

/// Returns the mouse wheel position
/// > Wheel position
int get_mouse_wheel();

/// Get joystick axes
/// > Axes
VEC2 get_joy_axes();

/// Get mouse delta movement
/// > Mouse delta
VEC2 get_mouse_delta();

/// Set new value for old mouse pos value x
/// < x X value
void set_old_mouse_x(int x);

/// Set new value for old mouse pos value u
/// < y Y value
void set_old_mouse_y(int y);

/// Required for mouse delta-ing
void flush_mouse_old_pos();

#endif // __CONTROLS__