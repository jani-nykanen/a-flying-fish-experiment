/// List loading (header)
/// (c) 2018 Jani Nykänen

#ifndef __LIST__
#define __LIST__

#include "SDL2/SDL.h"

/// Word type
typedef struct
{
    char data [256]; /// Word characters
    Uint8 len; /// Word length
}
WORD;

/// Load a list to memory
/// < path List path
/// > 0 if success, 1 on error
int load_list(const char* path);

/// Return word count of the list
/// > The word count
unsigned int get_list_word_count();

/// Returns a word in an index
/// < index Word index
/// > Empty word (len=0) if index out of range
WORD get_list_word(unsigned int index);

#endif // __LIST__