/// Scene (header)
/// (c) 2018 Jani Nykänen

#ifndef __SCENE__
#define __SCENE__

/// Scene structure
typedef struct
{
    int (*on_init) (); /// Initialize
    void (*on_update) (float tm); /// Update
    void (*on_draw) (); /// Draw
    void (*on_destroy) (void); /// Destroy
    void (*on_swap) (void); /// Scene swapped
    char name[8]; /// Scene name
}
SCENE;

/// Set scene name
/// < s Scene to edit
/// < name New name
void set_scene_name(SCENE* s, const char* name);

#endif // __SCENE__