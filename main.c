#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "definitions.h"
#include "errors.h"

#include "abb.h"
#include "lex_parser.h"
#include "syntax_parser.h"

int main(int argc, char** argv)
{
  /*  START VARIABLES DEFINITION  */
  //The symbol table is implemented as a binary tree
  abb symbolTable;
  //Name of the file to be compiled
  char* fileName = NULL;
  //Element to insert in the symbol table
  tipoelem temp;
  //Size of the buffer for the lexeme
  size_t lexemeSize;
  //Lexical parser, responsible for extracting the lexical components from the code file
  LexicalParser lexicalParser;
  //Syntax parser, responsible for doing the syntax analysis from the lexical components extracted by the lexical parser
  //The relationship between these two parser follows the producer-consumer model
  SyntaxParser syntaxParser;

  //TO DO: Move this out of main
  char reservedKeywords[NUM_KEYWORDS][MAX_KEYWORD_LENGTH] = {
    "False",
    "await",
    "else",
    "import",
    "pass",
    "None",
    "break",
    "except",
    "in",
    "raise",
    "True",
    "class",
    "finally",
    "is",
    "return",
    "and",
    "continue",
    "for",
    "lambda",
    "try",
    "as",
    "def",
    "from",
    "nonlocal",
    "while",
    "assert",
    "del",
    "global",
    "not",
    "with",
    "async",
    "elif",
    "if",
    "or",
    "yield"
  };

  //We check that the name of the file to compile was provided as a command line argument
  if(argc != 2) {
    //We execute the function to deal with the error of a wrong number of arguments
    errorWrongNumberArguments();
    //We stop the execution of the program
    return (EXIT_FAILURE);
  }
  /*  END VARIABLES DEFINITION  */


  /*  START VARIABLES INITIALIZATION */
  //We initialize the binary tree
  crear_arbol(&symbolTable);
  //The name of the file to be compiled is provided in the second argument
  fileName = argv[1];

  //We introduce the reserved keywords in the symbol table
  for(int i = 0; i < NUM_KEYWORDS; i++) {
    //Note that we only strlen does not take into account the termination character \0
    //Therefore, we have to allocate the size of strlen(...) + 1 characters
    lexemeSize = strlen(reservedKeywords[i])+1;
    //Now, we allocate memory for the lexeme
    temp.lexeme = (char*)malloc(lexemeSize*sizeof(char));
    //We copy the contents of the static array to the newly allocated lexeme
    strncpy(temp.lexeme, reservedKeywords[i], lexemeSize);
    //We assume that the order in definitions.h of reservedKeywords and the definition of the keywords is the same
    temp.lexicalComponent = START_KEYWORDS + i;
    //We add the newly created lexical element to the symbol table
    insertar(&symbolTable, temp);
  }

  //We initialize the lexical parser, passing the reference to the symbol table and the file to be compiled
  initializeLexicalParser(&lexicalParser, &symbolTable, fileName);
  //We initialize the syntax parser, passing the reference to the symbol table and the lexical parser
  initializeSyntaxParser(&syntaxParser, lexicalParser, &symbolTable);
  /*  END VARIABLES INITIALIZATION */

  //We print the symbol table before starting the lexical analysis
  printf("Symbol table before the start of the lexical analysis:\n");
  inorden(symbolTable);
  printf("---------------------\n");

  //We perform the lexical analysis from the syntax parser
  printf("Lexical analysis:\n");
  performLexicalAnalysis(syntaxParser);
  printf("---------------------\n");

  //We print the symbol table after the lexical analysis
  printf("Symbol table after the lexical analysis:\n");
  inorden(symbolTable);
  printf("---------------------\n");

  /*  START VARIABLES REMOVAL */
  //We destroy the symbol table
  destruir_arbol(&symbolTable);
  //We destroy the lexical parser
  removeLexicalParser(&lexicalParser);
  //We destory the syntax parser
  removeSyntaxParser(&syntaxParser);
  /*  END VARIABLES REMOVAL */

  //We end the program succesfully
  return (EXIT_SUCCESS);
}
