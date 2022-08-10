#ifndef __INP_SYSTEM
#define __INP_SYSTEM

//We define the input buffer as an abstract data type
//Its implementation is hidden for the user who only knows about the functionality it exports
typedef struct StructInputBuffer *InputBuffer;
/**
 * Initializes the input system
 */
void initializeBuffer(InputBuffer* inputBuffer, char* fileName);
/**
 * Removes the input buffer, deallocating the memory associated with it
 */
void removeBuffer(InputBuffer* inputBuffer);
/**
 * Reads the next character in the buffer
 */
char returnNextCaracter(InputBuffer inputBuffer);
/**
 * Goes back one position in the buffer
 */
void reverseOneCharacter(InputBuffer inputBuffer);
/*
 * We need to invoke this function when the current character does not belong to any lexeme
 */
void ignoreCharacter(InputBuffer inputBuffer);
/**
 * Reads the current lexeme being processed
 */
char* returnLexeme(InputBuffer inputBuffer, size_t lengthLexeme);


#endif
