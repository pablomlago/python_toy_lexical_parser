#ifndef __SYN_LIB
#define __SYN_LIB

#include <stdio.h>
#include "lex_parser.h"

//We define the lexical parser as an abstract data type
//Its implementation is hidden for the user who only knows about the functionality it exports
typedef struct StructSyntaxParser *SyntaxParser;

/**
 * Initializes the syntax parser with the lexical parser and the symbol table
 */
void initializeSyntaxParser(SyntaxParser* syntaxParser, LexicalParser lexicalParser, abb* symbolTable);
/**
 * Removes the syntax parser, deallocating the memory associated with it
 */
void removeSyntaxParser(SyntaxParser* syntaxParser);

/**
 * Consumes the lexical elements returned by the lexical parser
 */
void performLexicalAnalysis(SyntaxParser SyntaxParser);

#endif	/* __SYN_LIB */
