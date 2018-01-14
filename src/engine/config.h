/// Configuration file (header)
/// (c) 2018 Jani Nykänen

#ifndef __CONFIG__
#define __CONFIG__

#include "stdbool.h"

/// Title string size
#define TITLE_STRING_SIZE 64
/// Asset path size
#define ASSET_PATH_SIZE 256

/// Configuration structure 
typedef struct
{
    int winWidth;
    int winHeight;
    int canvasWidth;
    int canvasHeight;
    int fps;
    bool fullscreen;
    char title[TITLE_STRING_SIZE];
}
CONFIG;

/// Read a configuration file
/// < c Config data
/// < path File path
/// > 1 on success, 0 on error
int read_config(CONFIG* c, const char* path);

#endif // __CONFIG__