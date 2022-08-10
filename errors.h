#ifndef __ERRORS_LIB
#define __ERRORS_LIB

/*
 * Code file was not provided as an argument
 */
void errorWrongNumberArguments();
/*
 * Impossible to open code file
 */
void errorImpossibleToOpenFile();
/*
 * The length of the lexeme is too big
 */
void errorLexemeMaximumLengthExceeded();
/*
 * Brackets are not balanced
 */
void errorBracketsNotBalanced();
/*
 * Wrong level of indentation
 */
void errorUnexpectedIndent();

#endif	/* __ERRORS_LIB */
