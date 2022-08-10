#include <stdio.h>
#include <stdlib.h>

#define COMPILER_NAME "etsepy"

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */

#define FATAL_ERROR "fatal error"
#define FATAL_ERROR_CODE 404

#define NORMAL_ERROR "error"
#define NORMAL_ERROR_CODE 403

#include "errors.h"

/* Definition of the functions for fatal error treatment */

/*
 * Code file was not provided as an argument
 */
void errorWrongNumberArguments() {
  printf(COMPILER_NAME": "RED FATAL_ERROR RESET ": No input file provided as command line argument\n");
  exit(FATAL_ERROR_CODE);
}
/*
 * Impossible to open code file
 */
void errorImpossibleToOpenFile() {
  perror(COMPILER_NAME": "RED FATAL_ERROR RESET);
  exit(FATAL_ERROR_CODE);
}
/*
 * The length of the lexeme is too big
 */
void errorLexemeMaximumLengthExceeded() {
   printf(COMPILER_NAME": "RED NORMAL_ERROR RESET ": Maximum length of lexeme exceeded\n");
}
/*
 * Brackets are not balanced
 */
void errorBracketsNotBalanced() {
  printf(COMPILER_NAME": "RED NORMAL_ERROR RESET ": Brackets are not balanced\n");
}
/*
 * Wrong level of indentation
 */
void errorUnexpectedIndent() {
  printf(COMPILER_NAME": "RED NORMAL_ERROR RESET ": Unexpected indent\n");
}
