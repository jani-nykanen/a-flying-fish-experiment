/// Main file (source)
/// (c) 2018 Jani Nykänen

#include "app.h"

#include <stdio.h>
#include <stdbool.h>

#include "controls.h"
#include "graphics.h"
#include "assets.h"

#include "music.h"
#include "sample.h"

#include "stdlib.h"
#include "math.h"
#include "stdio.h"

// Is application app_running
static bool isRunning;
// Is full screen
static bool isFullscreen;

// Window
static SDL_Window* window;
// Renderer
static SDL_Renderer* rend;
// Canvas
static FRAME* canvas;

// (Timer) old ticks
static int oldTicks;
// (Timer) new ticks
static int newTicks;
// (Timer) delta time
static int deltaTime;

// Canvas pos
static SDL_Point canvasPos;
// Canvas size
static SDL_Point canvasSize;

// Current scene
static SCENE currentScene;
// Global scene
static SCENE globalScene;
// Scenes
static SCENE scenes[16];
// Scene count
static Uint8 sceneCount;

// Wait time
static int frame_wait;

// Configuration
static CONFIG config;

// Joystick
static SDL_Joystick* joy;


// Calculate canvas size and position on screen
static void app_calc_canvas_prop(int winWidth, int winHeight)
{
    // If aspect ratio is bigger or equal to the ratio of the canvas
    if((float)winWidth/(float)winHeight >= (float)config.canvasWidth/ (float)config.canvasHeight )
    {
        canvasSize.y = winHeight;
        canvasSize.x = (int) ( (float)winHeight / (float) config.canvasHeight  * config.canvasWidth);

        canvasPos.x = winWidth/2 - canvasSize.x/2;
        canvasPos.y = 0;
    }
    else
    {
        canvasSize.x = winWidth;
        canvasSize.y =  (int) ( (float)canvasSize.x / (float) config.canvasWidth  * config.canvasHeight );

        canvasPos.y = winHeight/2 - canvasSize.y/2;
        canvasPos.x = 0;
    }
}


// Initialize SDL
static int app_init_SDL()
{   
    // Init
    if(SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) != 0)
    {
        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR,"Error!","Failed to init SDL!\n",NULL);
        return 1;
    }

    // Create window

    int windowWidth = config.winWidth;
    int windowHeight = config.winHeight;

    set_dimensions(windowWidth,windowHeight);

    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				              windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
	if(window == NULL)
	{
		SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR,"Error!","Failed to create an SDL window!\n",NULL);
        return 1;
	}

    isFullscreen = config.fullscreen == 1;
    // app_toggle_fullscreen();

    // Create renderer
    rend = SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
    if(rend == NULL)
    {
        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR,"Error!","Failed to create an SDL renderer!\n",NULL);
        return 1;
    }
    SDL_RenderClear(rend);
    SDL_RenderPresent(rend);

    // Hide mouse cursor
    SDL_ShowCursor(0);

    // Open joystick
    joy = SDL_JoystickOpen(0);
    if(joy == NULL)
    {
        printf("No joystick detected\n");
    }

    // Init audio
    if(init_music() == 1)
    {
        return 1;
    }
    init_samples();

    return 0;
}


// Toggle fullscreen mode
void app_toggle_fullscreen()
{
    SDL_SetWindowFullscreen(window,!isFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	isFullscreen = !isFullscreen;
}


// Initialize application
static int app_init(SCENE* arrScenes, int count, const char* assPath)
{
    // Init SDL
    if(app_init_SDL() != 0)
    {
        return 1;
    }

    // Set global renderer & init graphics
    init_graphics();
    set_global_renderer(rend);

    // Gen palette
    fr_gen_palette();

    // Create frame
    canvas = frame_create(config.canvasWidth,config.canvasHeight);
    if(canvas == NULL)
    {
        return 1;
    }
    bind_frame(canvas);

    // Calculate canvas pos & size
    int w,h;
    SDL_GetWindowSize(window,&w,&h);
    app_calc_canvas_prop(w,h);

    // Copy scenes to a scene array
    // and initialize them
    sceneCount = count;
    int i =0;
    for(; i < count; i++)
    {
        scenes[i] = arrScenes[i];
        if(scenes[i].on_init != NULL)
        {
            if(scenes[i].on_init() == 1)
            {
                return 1;
            }
        }

        if(strcmp(scenes[i].name,"global") == 0)
        {
            globalScene = scenes[i];
        }
    }

    // Make the last scene the current scene
    if(sceneCount > 0)
        currentScene = scenes[count -1];

    // Calculate frame wait value
    frame_wait = (int) floor (1000.0f / (float) config.fps);

    isRunning = true;

    return 0;
}


// Go through events
static void app_events()
{
    SDL_Event event;

    // Go through every event
    while (SDL_PollEvent(&event) != 0)
    {
        switch(event.type)
        {
        // Application quit
        case SDL_QUIT:
            ask_to_quit();
            // isRunning = false;
            return;

        // Window event (resize etc)
        case SDL_WINDOWEVENT:
            // Resize
            if(event.window.windowID == SDL_GetWindowID(window) && event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                app_calc_canvas_prop(event.window.data1,event.window.data2);
            }
            break;
        
        // Key down event
        case SDL_KEYDOWN:
            ctr_on_key_down(event.key.keysym.scancode);
            break;
        // Key up event
        case SDL_KEYUP:
            ctr_on_key_up(event.key.keysym.scancode);
            break;

        // Joy button down
        case SDL_JOYBUTTONDOWN:
            ctr_on_joy_down(event.jbutton.button);
            break;

        // Joy button up
        case SDL_JOYBUTTONUP:
            ctr_on_joy_up(event.jbutton.button);
            break;

        // Joy axis
        case SDL_JOYAXISMOTION:
        {
            int axis = 0;
            if(event.jaxis.axis == 0)
                axis = 0;
            else if(event.jaxis.axis == 1)
                axis = 1;
            else 
                break;

            float value = (float)event.jaxis.value / 32767.0f;

            ctr_on_joy_axis(axis,value);
            
            break;
        }

        // Joy hat
        case SDL_JOYHATMOTION:
        {
            int v = event.jhat.value;
            VEC2 stick = vec2(0.0f,0.0f);
            if(v == SDL_HAT_LEFTUP || v == SDL_HAT_LEFT || v == SDL_HAT_LEFTDOWN)
            {
                stick.x = -1.0f;
            }

            if(v == SDL_HAT_RIGHTUP || v == SDL_HAT_RIGHT || v == SDL_HAT_RIGHTDOWN)
            {
                stick.x = 1.0f;
            }

            if(v == SDL_HAT_LEFTUP || v == SDL_HAT_UP || v == SDL_HAT_RIGHTUP)
            {
                stick.y = -1.0f;
            }

            if(v == SDL_HAT_LEFTDOWN || v == SDL_HAT_DOWN || v == SDL_HAT_RIGHTDOWN)
            {
                stick.y = 1.0f;
            }

            ctr_on_joy_axis(0,stick.x);
            ctr_on_joy_axis(1,stick.y);

            break;
        }

        default:
            break;
        }
    }

}   


// Update application
// < delta Delta time in milliseconds
static void app_update(Uint32 delta)
{
    float tm = (float)((float)delta/1000.0f) / (1.0f/60.0f);
    /// Limit tm (in other words, limit minimum fps)
    if(tm > 5.0) tm = 5.0;

    // Quit
    if(get_key_state(SDL_SCANCODE_LCTRL) == DOWN &&
       get_key_state(SDL_SCANCODE_Q) == PRESSED)
    {
        isRunning = false;
        return;
    }

    // Full screen
    if( (get_key_state(SDL_SCANCODE_LALT) == DOWN &&
       get_key_state(SDL_SCANCODE_RETURN) == PRESSED) ||
       get_key_state(SDL_SCANCODE_F4) == PRESSED )
    {
        app_toggle_fullscreen();
    }

    // Update current & global scenes
    if(currentScene.on_update != NULL)
    {
        currentScene.on_update(tm);
    }
    if(globalScene.on_update != NULL)
    {
        globalScene.on_update(tm);
    }

    ctr_update();
    update_music(tm);

}


// Draw application
static void app_draw()
{
    // Clear to black
    clear(0,0,0);

    // Draw global & current scenes
    if(currentScene.on_draw != NULL)
    {
        currentScene.on_draw();
    }
    if(globalScene.on_draw != NULL)
    {
        globalScene.on_draw();
    }

    // Update frame texture
    frame_update_tex(canvas);

    // Draw frame
    SDL_Rect dest = (SDL_Rect){canvasPos.x,canvasPos.y,canvasSize.x,canvasSize.y};
    SDL_RenderCopy(rend,canvas->tex,NULL,&dest);

    SDL_RenderPresent(rend);
}


// Destroy application
static void app_destroy()
{
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);

    SDL_JoystickClose(joy);
}


// Swap scene
void app_swap_scene(const char* name)
{
    int i = 0;
    for(; i < sceneCount; i++)
    {
        if(strcmp(scenes[i].name,name) == 0)
        {
            currentScene = scenes[i];
            if(currentScene.on_swap != NULL)
                currentScene.on_swap();
            return;
        }
    }

}


// Terminate application
void app_terminate()
{
    isRunning = false;
}


// Get main frame
FRAME* app_get_canvas()
{
    return canvas;
}


// Run application
int app_run(SCENE* arrScenes, int count, CONFIG c)
{
    config = c;

    if(app_init(arrScenes,count,NULL) != 0) return 1;

    while(isRunning)
    {
        oldTicks = SDL_GetTicks();
        app_events();
        app_update(deltaTime);
        app_draw();
        newTicks = SDL_GetTicks();

        // Wait
        int deltaMilliseconds = (newTicks - oldTicks);
        int restTime = (int) (frame_wait-1) - (int)deltaMilliseconds;
        if (restTime > 0) 
            SDL_Delay((unsigned int) restTime);

        // Set delta time
        deltaTime = SDL_GetTicks() - oldTicks;;

    }
    app_destroy();

    return 0;
}


/// Ask if the user wants to quit
int ask_to_quit()
{
    const SDL_MessageBoxButtonData buttons[] = {
        { 0, 0, "Yes" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "No" },
        
    };
    const SDL_MessageBoxColorScheme colorScheme = {
        { 
            { 85,   85,   85 },
            {   255, 255,   255 },
            { 255, 255,   255 },
            {   170,   170, 170 },
            { 255,   255, 0 }
        }
    };

    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION,
        NULL, 
        "Quit application?", 
        "Are you sure you want to\nterminate the application?", 
        SDL_arraysize(buttons),
        buttons,
        &colorScheme
    };

    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) 
    {
        printf("Error displaying a message box!\n");
        return 1;
    }

    if (buttonid == 0)
    {
        isRunning = false;
        return 1;
    } 

    return 0;
}