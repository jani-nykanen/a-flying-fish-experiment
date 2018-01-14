/**
 * libparseword library
 * Header file
 * 
 * @author Jani Nykänen
 * @version 1.0.0
 */

#ifndef __LIB__PARSEWORD__
#define __LIB__PARSEWORD__

/** Word data type */
typedef struct
{
    char* data;
    int* wordPos;
    int* wordLength;
    int wordCount; 
    int size;
}
WORDDATA;

/**
 * Parse a file and return word data
 * @param path File path
 * @return Word data object pointer
 */
WORDDATA* parse_file(const char* path);

/**
 * Free word data object
 * @param w Word data object pointer
 */
void destroy_word_data(WORDDATA* w);

/**
 * Get word in index
 * @param w Word data
 * @param index Word index
 * @return Word pointer
 */
char* get_word(WORDDATA* w, int index);

#endif // __LIB__PARSEWORD__