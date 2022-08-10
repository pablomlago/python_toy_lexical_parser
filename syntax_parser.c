#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "definitions.h"
#include "syntax_parser.h"
#include "pila.h"

//Definition of the syntax parser
struct StructSyntaxParser {
  LexicalParser lexicalParser;
  abb* symbolTable;
  //We need one stack for checking the identation of the code
  pila tabStack;
  //We need another stack to check whether the brackets are balanced
  //And a logical line splits over several various lines
  pila bracketStack;
};
/**
 * Initializes the syntax parser with the lexical parser and the symbol table
 */
void initializeSyntaxParser(SyntaxParser* syntaxParser, LexicalParser lexicalParser, abb* symbolTable) {
  //We allocate memory for the syntax parser
  *syntaxParser = (SyntaxParser)malloc(sizeof(struct StructSyntaxParser));
  //We assing the elements of the struct to the parameters provided
  (*syntaxParser)->lexicalParser = lexicalParser;
  (*syntaxParser)->symbolTable = symbolTable;
  //We initialize the tab stack
  crearPila(&((*syntaxParser)->tabStack));
  //We add the initial element to the stack
  push(&((*syntaxParser)->tabStack), 0);
  //We initialize the bracket stack
  crearPila(&((*syntaxParser)->bracketStack));
}

/**
 * Removes the syntax parser, deallocating the memory associated with it
 */
void removeSyntaxParser(SyntaxParser* syntaxParser) {
  //The syntax parser is not responsible for freeing the memory associated to its components
  (*syntaxParser)->lexicalParser = NULL;
  (*syntaxParser)->symbolTable = NULL;
  //We destroy the tab stack
  destruirPila(&((*syntaxParser)->tabStack));
  //We destroy the bracket stack
  destruirPila(&((*syntaxParser)->bracketStack));
  //We deallocate the memory associated to the syntax parser itself
  free(*syntaxParser);
  *syntaxParser = NULL;
}
/*
 * Checks whether two characters are open and its correspondent closing bracket
 */
int isMatchingBracket(char c1, char c2) {
  return (c1 == '(' && c2 == ')') ||
         (c1 == '{' && c2 == '}') ||
         (c1 == '[' && c2 == ']');
}

/**
 * Consumes the lexical elements returned by the lexical parser
 */
void performLexicalAnalysis(SyntaxParser syntaxParser) {
  //Lexical element returned by the lexical parser
  LexicalElement* lexicalElement = NULL;
  //Variable for storing the lexical component associated to a lexical element
  int currentLexicalComponent = EOF;
  //Number of lexical elements in the current line
  int numLexicalElements = 0;
  //While we dont get to the end of the file, we ask the lexical parser for lexical components
  do {
    //We obtain the next lexical component and we process it
    lexicalElement = returnNextLexicalComponent(syntaxParser->lexicalParser);
    //We assign the lexical component to the one of the current lexical element
    currentLexicalComponent = lexicalElement->lexicalComponent;
    //The processing of the lexical element will consist only in printing it
    //In case the lexeme exceeded the maximum length we only print the lexical component
    if(lexicalElement->lexeme == NULL) {
      printf("< %d >\n", lexicalElement->lexicalComponent);
    //Blank lines should be ignored
    //TAB lexical components will be processed later
    //Comments should be ignored
    } else if(lexicalElement->lexicalComponent != COMMENT && lexicalElement->lexicalComponent != IGNORE && lexicalElement->lexicalComponent != TAB) {
      //We print the lexical component
      printf("< %d, %s >\n", lexicalElement->lexicalComponent,lexicalElement->lexeme);
      //We increment the number of lexical elements in the line
      numLexicalElements++;
    }

    //Firstly we check if the lexical element returned by the lexical parser corresponds to an opening bracket
    if(lexicalElement->lexicalComponent == '(' || lexicalElement->lexicalComponent == '[' || lexicalElement->lexicalComponent == '{') {
      //In that case, we add it to the bracket stack
      push(&(syntaxParser->bracketStack), lexicalElement->lexicalComponent);
    //If the first condition does not verify, we check if the current lexical element is a closing bracket
    } else if(lexicalElement->lexicalComponent == ')' || lexicalElement->lexicalComponent == ']' || lexicalElement->lexicalComponent == '}') {
      //We get the opening bracket from the top of the stack
      char currentBracket = tope(syntaxParser->bracketStack);
      //Nextly, we verify if the bracket from the top of the stack and the bracket returned by the lexical parser are from the same type
      if(isMatchingBracket(currentBracket, lexicalElement->lexicalComponent)) {
        //If the brackets match, we remove the top of the stack since we paired those brackets
        pop(&(syntaxParser->bracketStack));
      } else {
        //In case the brackets dont match, we show an error
        errorBracketsNotBalanced();
      }
    }

    /*The lexical component LEX_TAB comprises a lexeme with the newline character along with the spaces before the start of the line
     *Therefore, this is not a real lexical component in Python as it can result in the lexical components INDENT, DETENT and NEWLINE
     *Thus, the lexical component NEWLINE will be generated when we reach the end of a logical line
     *Expressions in parentheses, square brackets or curly braces can be split over more than one physical line, so we have to check
     *that we have closed all of them before issuing NEWLINE when detecting the end of a line
     *As a consequence, we will check if the bracketStack is empty*/

     /*Apart from NEWLINE, we have to issue INDENT and DETENT when changes in the indentation are detected
      *To do this we need a stack for the tabs, following we provide the explanation on how to do this provided in the Python documentation:
      *Before the first line of the file is read, a single zero is pushed on the stack; this will never be popped off again.
      *The numbers pushed on the stack will always be strictly increasing from bottom to top. At the beginning of each logical
      *line, the line’s indentation level is compared to the top of the stack. If it is equal, nothing happens. If it is larger,
      *it is pushed on the stack, and one INDENT token is generated. If it is smaller, it must be one of the numbers occurring
      *on the stack; all numbers on the stack that are larger are popped off, and for each number popped off a DEDENT token is
      *generated. At the end of the file, a DEDENT token is generated for each number remaining on the stack that is larger than zero.*/
    if(lexicalElement->lexicalComponent == TAB && esVaciaPila(syntaxParser->bracketStack)) {
      //If those conditions are verified, we will have reached the end of a logical line
      //Therefore, we will issue the corresponding lexical element
      //We check that in the line there was some lexical element before issuing a NEWLINE
      if(numLexicalElements > 0) {
        printf("< %d, NEWLINE >\n", NEWLINE);
        //We reset the count of lexical elements in the line
        numLexicalElements = 0;
      }
      //LEX_TAB lexemes are composed of a end of line character and an arbitrary number of whitespaces
      //Counting this whitespaces will allow us to determine the indentation level
      int numSpaces = strlen(lexicalElement->lexeme)-1;
      //Top of the tab stack holds the current indentation level
      int currentIndentation = tope(syntaxParser->tabStack);
      //If the level indentation is greater than in the top of the stack, we generate an INDENT lexical element
      if(numSpaces > currentIndentation) {
        //We issue a INDENT lexical element
        printf("< %d, INDENT >\n", INDENT);
        //The number of spaces for this identation level will be stored on top of the stack
        push(&(syntaxParser->tabStack), numSpaces);
      //If the level indentation is less than in the top of the stack
      } else if(numSpaces < currentIndentation) {
        ///We generate an DEDENT lexical element for every element in the stack until we get to the same indentation level
        while(numSpaces < tope(syntaxParser->tabStack)) {
          printf("< %d, DEDENT >\n", DEDENT);
          //We remove the element on top of the stack
          pop(&(syntaxParser->tabStack));
        }
        //If after finishing the loop above the top of the stack is not equal to the number of spaces and indentation error was found
        if(numSpaces != tope(syntaxParser->tabStack)) {
          //We give this error appropiate treatment
          errorUnexpectedIndent();
        }
      }
    }
    //We free the memory associated to the returned lexical element
    removeLexicalElement(lexicalElement);
    //And we deallocate the memory of the pointer itself
    free(lexicalElement);
    lexicalElement = NULL;
    //We check for the end of file
  } while(currentLexicalComponent != EOF);
}
