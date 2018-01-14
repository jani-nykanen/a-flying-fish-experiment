/**
 * libparseword library
 * Source file
 * 
 * @author Jani Nykänen
 * @version 1.0.0
 */

#include "parseword.h"

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"

// File size in bytes
static int fileSize;
// File data (char array)
static char* fdata;

// Calculate file size
static int calculate_file_size(FILE* f)
{
    int length = 0;

    char c;
    while ((c = fgetc(f)) != EOF)
        ++ length;

    return length;
}

// Store byte data to a char array
static int store_byte_data(FILE* f)
{
    rewind(f);

    // Allocate memory
    fdata = (char*)malloc((size_t)fileSize);
    if(fdata == NULL)
    {
        printf("Memory allocation error!\n");
        return 1;
    }

    // Store characters
    char c;
    int index = 0;
    while ((c = fgetc(f)) != EOF)
    {
        fdata[index] = c;
        ++ index;
    }

    return 0;
}

//  Calculate actual size & word count and/or store word positions, length and actual data
static void read_data(int* charCount, int* wordCount, int* wordPos, int* wordLengths, char* data)
{
    int size = 0;
    int words = 0;

    char c;
    int index = 0;

    bool commentStarted = false;
    bool quote = false;
    char quoteType = 0;
    int wordLength = 0;
    bool wordStarted = false;

    int cindex = 0;
    int wstart = 0;

    for(; index < fileSize; ++ index)
    {
        c = fdata[index];
        
        if(!commentStarted && c == '#')
        {
            commentStarted = true;
        }
        else if(!commentStarted && !quote && (c == 39 || c == '"'))
        {
            quote = true;
            quoteType = c;
            wordStarted = true;
            wordLength = 0;
            wstart = cindex;
            continue;
        }

        if(commentStarted)
        {
            if(c == '\n')
                commentStarted = false;
            
            continue;
        }

        if(quote)
        {
            if(c == quoteType)
            {
                quote = false;
                wordStarted = false;

                if(wordPos != NULL)
                {
                    wordPos[words] = wstart;
                }

                if(wordLength > 0)
                    ++ words;

                if(data != NULL)
                {
                    data[cindex] = 0;
                    ++ cindex;
                }

                continue;
            }

            if(c != '\n')
            {
                if(data != NULL)
                {
                    data[cindex] = c;
                }
                ++ size;
                ++ cindex;
            }

            ++ wordLength;
        }
        else 
        {
            if(c != ' ' && c != '\t' && c != ',' && c != '\n')
            {
                if(data != NULL)
                {
                    data[cindex] = c;
                }

                ++ size;
                ++ wordLength;
                if(!wordStarted)
                {
                    if(wordPos != NULL)
                    {
                        wordPos[words] = cindex;
                    }

                    wordStarted = true;
                    ++ words;
                }
                ++ cindex;
            }
            else
            {
                if(wordStarted && wordLength > 0)
                {
                    if(wordLengths != NULL)
                        wordLengths[words-1] = wordLength;

                    if(data != NULL)
                    {
                        data[cindex] = 0;
                        ++ cindex;
                    }
                }
                wordStarted = false;
                wordLength = 0;
            }
        }
        
    }

    if(quote)
    {
        quote = false;
        wordStarted = false;

        if(wordPos != NULL)
        {
            wordPos[words] = wstart;
        }

        if(wordLength > 0)
            ++ words;

        if(data != NULL)
        {
            data[cindex] = 0;
            ++ cindex;
        }
    }

    if(charCount != NULL) *charCount = size;
    if(wordCount != NULL) *wordCount = words;
}

// Parse file
WORDDATA* parse_file(const char* path)
{
    // Open file
    FILE* f = fopen(path,"r");
    if(f == NULL)
    {
        printf("Failed to open a file in %s!\n",path);
        return NULL;
    }
    // Calculate char count & store data
    fileSize = calculate_file_size(f);
    if(store_byte_data(f) == 1)
    {
        fclose(f);
        return NULL;
    }
    fclose(f);

    // Allocate memory
    WORDDATA* w = malloc(sizeof(WORDDATA));
    if(w == NULL)
    {
        printf("Memory allocation error!\n");
        return NULL;
    }

    // Calculate actual size
    read_data(&w->size,&w->wordCount,NULL,NULL,NULL);
    // Allocate memory for the data
    w->data = (char*)malloc(sizeof(char) * (w->size + w->wordCount) );
    if(w->data == NULL)
    {
        free(w);
        printf("Memory allocation error!\n");
        return NULL;
    }
    w->wordPos = (int*)malloc(w->wordCount * sizeof(int));
    if(w->data == NULL)
    {
        free(w->data);
        free(w);
        printf("Memory allocation error!\n");
        return NULL;
    }
    w->wordLength = (int*)malloc(w->wordCount * sizeof(int));
    if(w->wordLength == NULL)
    {
        free(w->data);
        free(w->wordPos);
        free(w);
        printf("Memory allocation error!\n");
        return NULL;
    }
    // Store word pos & length
    read_data(NULL,NULL,w->wordPos,w->wordLength,w->data);
    w->data[w->size + w->wordCount] = 0;

    free(fdata);

    return w;
}

// Free word data
void destroy_word_data(WORDDATA* w)
{
    if(w == NULL) return;

    free(w);
}

// Get word
char* get_word(WORDDATA* w, int index)
{
    if(index >= w->wordCount) return NULL;

    return w->data + w->wordPos[index];
}