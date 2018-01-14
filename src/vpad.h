/// Virtual game pad (header)
/// (c) 2018 Jani Nykänen

#ifndef __VPAD__
#define __VPAD__

#include "engine/controls.h"
#include "engine/vector.h"

/// Initialize virtual gamepad
void vpad_init();

/// Update vpad
void vpad_update();

/// Get stick axis
/// > A vector holding stick axis information
VEC2 vpad_get_stick();

/// Get stick delta
/// > Delta vector
VEC2 vpad_get_delta();

/// Add a new button
/// < index Button index
/// < scancode Button scancode
/// < joybutton Joystick button index
void vpad_add_button(Uint8 index, int scancode, int joybutton);

/// Get virtual pad button state
/// < index Button index
int vpad_get_button(Uint8 index);

#endif // __VPAD__