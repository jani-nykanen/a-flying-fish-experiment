/// List loading (header)
/// (c) 2018 Jani Nykänen

#include "list.h"

#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"

/// Maximum amount of words in a file
#define MAX_WORDS 65535

/// An array of words
static WORD words [MAX_WORDS]; // No more words supported

/// Amount of words
static unsigned int wordCount;

/// Parse file
/// path File path
/// Returns 0 if success, 1 if error
static int parse_file(const char* path)
{
    // Open file
    FILE* f = fopen(path,"r");
    if(f == NULL)
    {
        printf("Failed to open a file in %s!\n",path);
        return 1;
    }

    // Initialize the first word
    words[0].len = 0;

    // Go through characters
    char ch;
    bool ignore = false;
    while ( (ch = fgetc(f)) != EOF)
    {
        // If comment, ignore until newline
        if(ch == '#')
        {
            ignore = true;
        }

        if(!ignore)
        {
            // If newline or space, move to the next word (if the word length is not 0)
            if(words[wordCount].len != 0 && (ch == '\n' || ch == ' '))
            {
                words[wordCount].data[words[wordCount].len] = '\0';

                wordCount ++;
                if(wordCount >= MAX_WORDS)
                    wordCount = MAX_WORDS;

                // Clear word
                int i=0;
                for(; i < 256; i++)
                {
                    words[wordCount].data[i] = 0;
                }

                words[wordCount].len = 0;
            }
            else
            {
                words[wordCount].data[words[wordCount].len] = ch;
                words[wordCount].len ++;
            }
        }
        // If newline and comment, comment ends
        else if(ignore && ch == '\n')
        {
            ignore = false;
        }

        
    }

    fclose(f);
    
    return 0;
}

/// Load a list to memory
int load_list(const char* path)
{
    wordCount = 0;
    if(parse_file(path) != 0)
    {
        return 1;
    }

    return 0;
}

/// Return word count of the list
unsigned int get_list_word_count()
{
    return wordCount;
}

/// Returns a word in an index
WORD get_list_word(unsigned int index)
{
    if(index > wordCount)
    {
        WORD w;
        w.len = 0;
        return w;
    }

    return words[index];
}
