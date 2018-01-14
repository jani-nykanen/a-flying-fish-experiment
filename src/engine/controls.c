/// Controls (source)
/// (c) 2018 Jani Nykänen

#include "controls.h"

#include "stdlib.h"
#include "math.h"
#include "stdio.h"
#include "stdbool.h"

/// Amount of keys
#define KEYMAX 256
/// Button max
#define BUTTONMAX 4
/// Joystick button max
#define JOYMAX 12

/// An array of keys
static STATE keystate[KEYMAX];
/// An array of mouse buttons
static STATE mousestate[BUTTONMAX];
/// Joystick button state
static STATE joystate[JOYMAX];
/// Cursor pos
static SDL_Point mousePos;
/// Old cursor pos
static SDL_Point oldPos;
/// Delta pos
static SDL_Point delta;
/// Mouse wheel pos
static int wheel;
/// Joystick axis
static VEC2 joyAxis;
/// Ignore old mouse
static bool ignoreOldMouse;

/// Any pressed
static bool anyPressed;
/// Any released
static bool anyReleased;
/// Any down
static bool anyDown;

/// Init controls
void ctr_init()
{
    anyPressed = false;
    anyReleased = false;
    anyDown = false;

    // Let's initialize the keystate array (put UP state to all keys)
    int i = 0;
    for(; i < KEYMAX; i++)
    {
        keystate[i] = UP;

        if( i < JOYMAX)
            joystate[i] = UP;

        if( i < BUTTONMAX)
            mousestate[i] = UP;
    }

    wheel = 0;
    mousePos.x = 0;
    mousePos.y = 0;
    oldPos = mousePos;

    joyAxis.x = 0.0f;
    joyAxis.y = 0.0f;

    delta.x = 0.0f;
    delta.y = 0.0f;

    ignoreOldMouse = false;
}

/// On key down
void ctr_on_key_down(SDL_Scancode scancode)
{
    int key = (int)scancode;

    // If the key is in range, make it be pressed
    if(key < 0 || key >= KEYMAX || keystate[key] == DOWN) 
        return;

    anyPressed = true;
    anyDown = true;
    
    keystate[key] = PRESSED;
}

/// On key up
void ctr_on_key_up(SDL_Scancode scancode)
{
    int key = (int)scancode;

    // If the key is in range, make it be released
    if(key < 0 || key >= KEYMAX || keystate[key] == UP) 
        return;

    anyReleased = true;
    anyDown = false;

    keystate[key] = RELEASED;
}

/// Mouse move event
void ctr_on_mouse_move(int x, int y)
{
    mousePos.x = x;
    mousePos.y = y;
}

/// Mouse button pressed
void ctr_on_mouse_down(int button)
{
    if(button < 0 || button >= BUTTONMAX || mousestate[button] == DOWN) 
        return;
    
    mousestate[button] = PRESSED;
}

/// Mouse button released
void ctr_on_mouse_up(int button)
{
    if(button < 0 || button >= BUTTONMAX || mousestate[button] == UP) 
        return;

    mousestate[button] = RELEASED;
}

/// Mouse wheel event
void ctr_on_mouse_wheel(int pos)
{
    wheel = pos;
}


/// Joystick button down
void ctr_on_joy_down(int button)
{
    if(button < 0 || button >= JOYMAX || joystate[button] == DOWN) return;

    joystate[button] = PRESSED;

    anyPressed = true;
    anyDown = true;
}


/// Joystick button released
void ctr_on_joy_up(int button)
{
    if(button < 0 || button >= JOYMAX || joystate[button] == UP) return;
    
    joystate[button] = RELEASED;

    anyReleased = true;
    anyDown = false;
}

/// Joystick axis movement
void ctr_on_joy_axis(int axis, float value)
{
    if(axis == 0)
        joyAxis.x = value;
    else if(axis == 1)
        joyAxis.y = value;
}

/// Update
void ctr_update()
{
    wheel = 0;

    /*
     * Go through every key. If key is pressed, make it be
     * down, if released, up. This way pressed and released
     * states will only last a frame.
     */
    int i = 0;
    for(; i < KEYMAX; i++)
    {
        if(keystate[i] == RELEASED)
            keystate[i] = UP;

        else if(keystate[i] == PRESSED)
            keystate[i] = DOWN;

        if(i < BUTTONMAX)
        {
            if(mousestate[i] == RELEASED)
                mousestate[i] = UP;

            else if(mousestate[i] == PRESSED)
                mousestate[i] = DOWN;
        }

        if(i < JOYMAX)
        {
            if(joystate[i] == RELEASED)
                joystate[i] = UP;

            else if(joystate[i] == PRESSED)
                joystate[i] = DOWN;
        }
    }

    if(ignoreOldMouse)
    {
        oldPos.x = mousePos.x;
        oldPos.y = mousePos.y;
        ignoreOldMouse = false;
    }

    delta.x = (float)(mousePos.x-oldPos.x);
    delta.y = (float)(mousePos.y-oldPos.y);
    oldPos = mousePos;

    anyReleased = false;
    anyPressed = false;
}

/// Get key state
STATE get_key_state(int key)
{
    if(key < 0 || key >= KEYMAX) 
        return UP;

    return keystate[key];
}

/// Get button state
STATE get_mouse_button_state(int button)
{
    if(button < 0 || button >= BUTTONMAX) 
        return UP;
    
    return mousestate[button];
}

/// Get joystick button state
STATE get_joy_button_state(int button)
{
    if(button < 0 || button >= JOYMAX) 
        return UP;

    return joystate[button];
}

/// Get mouse pos
SDL_Point get_cursor_pos()
{
    return mousePos;
}

/// Is any button pressed
bool any_pressed()
{
    return anyPressed;
}

/// Is any button released
bool any_released()
{
    return anyReleased;
}

/// Is any button released
bool any_down()
{
    return anyDown;
}

/// Get wheel position
int get_mouse_wheel()
{
    return wheel;
}

/// Get joystick axes
/// > Axes
VEC2 get_joy_axes()
{
    return joyAxis;
}

/// Get mouse delta movement
/// > Mouse delta
VEC2 get_mouse_delta()
{
    return vec2(delta.x,delta.y);
}

/// Set new value for old mouse pos value x
/// < x X value
void set_old_mouse_x(int x)
{
    oldPos.x = x;
}

/// Set new value for old mouse pos value u
/// < y Y value
void set_old_mouse_y(int y)
{
    oldPos.y = y;
}

/// Required for mouse delta-ing
void flush_mouse_old_pos()
{
    ignoreOldMouse = true;
}