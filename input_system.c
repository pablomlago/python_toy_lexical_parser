#include <stdio.h>
#include <stdlib.h>

#include "input_system.h"
#include "errors.h"

//Size of each of the two blocks that will be read from the code file
#define BUFFER_SIZE 1024
//End of the first block
#define END_FIRST_BUFFER BUFFER_SIZE
//End of the second block
#define END_SEC_BUFFER 2*BUFFER_SIZE+1
//Combined size of the two blocks that form the input system
#define FULL_BUFFER_SIZE END_SEC_BUFFER+1

//Definition of the input system
struct StructInputBuffer{
  //Descriptor to the file to be compiled
  FILE* codeFile;
  //The two blocks will be implemented as a single array
  char buffer[FULL_BUFFER_SIZE];
  //Pointer to the start of the lexeme
  int start;
  //Pointer to the position being processed at a given point in time
  int lead;
  //Flag to check whether a block has already been loaded in memory
  int blockLoaded;
};

//Declaration of the private functions in the input system module
FILE* openCodeFile(char* fileName);
void readBlock(FILE* codeFile, char* buffer, size_t bufferSize);

/**
 * Opens a given file in read-only mode
 */
FILE* openCodeFile(char* fileName) {
  FILE* codeFile = fopen(fileName, "r");
  //We check if we managed to open the file succesfully
  if(codeFile == NULL) errorImpossibleToOpenFile();
  return codeFile;
}
/**
 * Reads a block of a given size from the file containing to be compiled
 */
void readBlock(FILE* codeFile, char* buffer, size_t bufferSize) {
  //We read a number of characters equals to bufferSize from codeFile
  int num = fread(buffer, sizeof(char), bufferSize, codeFile);
  //In case the read was succesful, the number of bytes read will be less than bufferSize
  //if the end of file was reached. Since this is not returned, we have to added manually
  if(num < bufferSize) buffer[num] = EOF;
}
/**
 * Initializes the input system
 */
void initializeBuffer(InputBuffer* inputBuffer, char* fileName) {
  //We allocate memory for the input buffer itself
  *inputBuffer = (InputBuffer)malloc(sizeof(struct StructInputBuffer));
  //We open the file containing the code
  (*inputBuffer)->codeFile = openCodeFile(fileName);
  //We initialize the first block of the buffer with the initial content of the file
  readBlock((*inputBuffer)->codeFile, (*inputBuffer)->buffer, BUFFER_SIZE);
  //We add EOF at the centinel positions
  (*inputBuffer)->buffer[END_FIRST_BUFFER] = EOF;
  (*inputBuffer)->buffer[END_SEC_BUFFER] = EOF;
  //In the beginning, both pointers start at the initial position of the block
  (*inputBuffer)->start = 0;
  (*inputBuffer)->lead = 0;
  //The next block has not been loaded
  (*inputBuffer)->blockLoaded = 0;
}
/**
 * Removes the input buffer, deallocating the memory associated with it
 */
void removeBuffer(InputBuffer* inputBuffer) {
  //We close the file containing the code
  fclose((*inputBuffer)->codeFile);
  (*inputBuffer)->codeFile = NULL;
  //We free the memory associated to the input buffer itself
  free(*inputBuffer);
  *inputBuffer = NULL;
}
/**
 * Reads the next character in the buffer
 */
char returnNextCaracter(InputBuffer inputBuffer)  {
  //We read the character pointed by lead
  char c = inputBuffer->buffer[inputBuffer->lead];
  //In case EOF is encountered, we have to distinguish between three situations
  if(c == EOF) {
    //We check if we are at the last position of the first block
    if(inputBuffer->lead == END_FIRST_BUFFER) {
      //We check if the second block is already in memory (we have returned a character previously)
      //Otherwise, we load the second block, reading it from the code file
      if(inputBuffer->blockLoaded) inputBuffer->blockLoaded = 0;
      else readBlock(inputBuffer->codeFile, (inputBuffer->buffer+(BUFFER_SIZE+1)), BUFFER_SIZE);
      //We move lead to the first position of the second block
      inputBuffer->lead = END_FIRST_BUFFER+1;
    //We check if we are at the last position of the second block
    } else if(inputBuffer->lead == END_SEC_BUFFER) {
      //We check if the first block is already in memory (we have returned a character previously)
      //Otherwise, we load the first block, reading it from the code file
      if(inputBuffer->blockLoaded) inputBuffer->blockLoaded = 0;
      else readBlock(inputBuffer->codeFile, (inputBuffer->buffer), BUFFER_SIZE);
      //We move lead to the first position of the first block
      inputBuffer->lead = 0;
    }
    //We read the new character pointed by lead
    //In case we were at the end of the file, EOF will be read again
    c = inputBuffer->buffer[inputBuffer->lead];
  }
  //We move lead one position forward
  inputBuffer->lead++;
  //The read character is returned
  return c;
}
/**
 * Goes back one position in the buffer
 */
void reverseOneCharacter(InputBuffer inputBuffer) {
  //We move lead one position backwards
  inputBuffer->lead--;
  //Our array is circular,i.e., in case we were at the first position of the first block,
  //lead should be moved to the last position of the second block
  inputBuffer->lead %= FULL_BUFFER_SIZE;
  //We read the current character pointed by lead
  char c = inputBuffer->buffer[inputBuffer->lead];
  //In case EOF was encountered, the previous position was the last position either from the first or the second block
  //or the EOF corresponds to the actual end of file
  if(c == EOF) {
    //In case we are at the end of some of the blocks, we have to move the pointer another position backwards
    if(inputBuffer->lead == END_FIRST_BUFFER || inputBuffer->lead == END_SEC_BUFFER) {
      inputBuffer->lead--;
      inputBuffer->lead %= FULL_BUFFER_SIZE;
      //We indicate that the next block is already in memory to avoid overwriting it
      inputBuffer->blockLoaded = 1;
    }
  }
}
/*
 * We need to invoke this function when the current character does not belong to any lexeme
 */
void ignoreCharacter(InputBuffer inputBuffer) {
  //Move start pointer to the character pointed by lead
  inputBuffer->start = inputBuffer->lead;
}
/**
 * Reads the current lexeme being processed
 */
char* returnLexeme(InputBuffer inputBuffer, size_t lengthLexeme) {
  //Lexeme to be returned by the input system
  char* lexeme = NULL;
  //Current character being processed
  char c;
  //Position in the lexeme
  int pos = 0;
  //If the size of the lexeme exceeds the buffer size we will not be able
  //to reconstruct it, so we return the null pointer and show and error
  if(lengthLexeme > BUFFER_SIZE) {
    inputBuffer->start = inputBuffer->lead;
    errorLexemeMaximumLengthExceeded();
  }
  else {
    //We allocate exactly the memory we need to store the lexeme
    lexeme = (char*)malloc((lengthLexeme+1)*sizeof(char));
    //We iterate until we copy the whole lexeme
    while(pos < lengthLexeme) {
      //We get the current character being processed
      c = inputBuffer->buffer[inputBuffer->start];
      //In case EOF is encountered, we have to distinguish between three situations
      if(c == EOF) {
        //We check if we are at the last position of the first block
        if(inputBuffer->start == END_FIRST_BUFFER) {
          //We move start to the first position of the second block
          inputBuffer->start = END_FIRST_BUFFER+1;
        //We check if we are at the last position of the second block
        }  else if(inputBuffer->start == END_SEC_BUFFER) {
          //We move start to the first position of the first block
          inputBuffer->start = 0;
        }
        //We read the new character pointed by start
        //In case we were at the end of the file, EOF will be read again
        c = inputBuffer->buffer[inputBuffer->start];
      }
      //We move start one position forward
      inputBuffer->start++;
      //We add the character to the lexeme
      lexeme[pos++] = c;
    }
    //We add the null character to the string
    lexeme[lengthLexeme] = '\0';
  }
  //Finally, we move start to the same position as lead
  //In the end of block, start is in a position before
  inputBuffer->start = inputBuffer->lead;
  //We return the constrycted lexeme
  return lexeme;
}
