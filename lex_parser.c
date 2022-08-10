#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stateMachine.h"
#include "identifierStateMachine.h"
#include "numberStateMachine.h"
#include "commentStateMachine.h"

#include "abb.h"
#include "definitions.h"

#include "input_system.h"

#include "lex_parser.h"

//Definition of the lexical parser
struct StructLexicalParser{
  //Input system used to read the characters from the code file
  InputBuffer inputBuffer;
  //Finite state automata used to recognize the different lexical components
  TStateMachine* stateMachine;
  //Symbol table in which the identifiers will be stored
  abb* symbolTable;
};

//Declaration of the private functions in the lexical parser
int getLexicalComponent(abb* symbolTable, int stateTransition, char* lexeme);

/**
 * Initializes the a lexical element, allocating a given size for the lexeme
 */
void initializeLexicalElement(LexicalElement* lexicalElement, size_t lexemeSize) {
  lexicalElement->lexeme = (char*)malloc(lexemeSize*sizeof(char));
}

/*
 * Removes a lexical element, deallocating the memory associated with it
 */
void removeLexicalElement(LexicalElement* lexicalElement) {
  free(lexicalElement->lexeme);
  lexicalElement->lexeme = NULL;
}

/*
 * Initializes the lexical parser, saving the reference to the symbol table and providing the file to be compiled
 */
void initializeLexicalParser(LexicalParser* lexicalParser, abb* symbolTable, char* fileName) {
  //We allocate memory for the lexical parser itself
  *lexicalParser = (LexicalParser)malloc(sizeof(struct StructLexicalParser));

  //We initialize the input buffer passing the name of the file to be compiled
  initializeBuffer(&((*lexicalParser)->inputBuffer), fileName);

  //We allocate memory for the state machine detecting the different types of lexical components
  (*lexicalParser)->stateMachine = (TStateMachine*)malloc(sizeof(TStateMachine));
  //We initialize the state machine using the function provided in its interface
  initializeStateMachine((*lexicalParser)->stateMachine);

  //We save the reference to the symbol table
  (*lexicalParser)->symbolTable = symbolTable;
}

/*
 * Removes the lexical parser, deallocating the memory associated to the input buffer and the state state machine
 * The symbol table is not removed, as this element is shared between different components of our compiler
 */
void removeLexicalParser(LexicalParser* lexicalParser) {
  //We remove the buffer using the specific function provided in its interface
  removeBuffer(&((*lexicalParser)->inputBuffer));
  //We remove the state machine and then free the memory associated with it
  removeStateMachine((*lexicalParser)->stateMachine);
  free((*lexicalParser)->stateMachine);
  (*lexicalParser)->stateMachine = NULL;
  //We stop saving the reference to the symbol table
  (*lexicalParser)->symbolTable = NULL;
  //We deallocate the memory associated to the lexical parser itself
  free(*lexicalParser);
  *lexicalParser = NULL;
}

/*
 * Return the next lexical element in the code file being compiled
 */
LexicalElement* returnNextLexicalComponent(LexicalParser lexicalParser) {
  //Pointer to the next lexical element in the code
  LexicalElement* lexicalElement = NULL;
  //Length of the current lexeme being processed
  size_t lengthLexeme = 0;
  //Character read in every iteration
  char c;
  //We initialize the state transition
  int stateTransition = DEFAULT;
  //We set our state to the initial state
  lexicalParser->stateMachine->curState = initial_state;
  //We iterate until we get to an end state
  while(lexicalParser->stateMachine->curState != end_state_return
    && lexicalParser->stateMachine->curState != end_state_not_return
    && lexicalParser->stateMachine->curState != incorrect_state) {
      //We extract the next character from the buffer
      c = returnNextCaracter(lexicalParser->inputBuffer);
      //And we execute the corresponding transition in our automata
      stateTransition = (lexicalParser->stateMachine->curState)(lexicalParser->stateMachine, c);
      //In case we are in an intermediate state we start saving characters in our buffer
      if(lexicalParser->stateMachine->curState != initial_state) {
        lengthLexeme++;
      } else {
      //Otherwise, we ignore the caracter
        ignoreCharacter(lexicalParser->inputBuffer);
      }
  }

  //We reached an end state in which we have to return the last character read to the buffer
  if(lexicalParser->stateMachine->curState == end_state_return) {
    //In case the last character read does not correspond to the lexeme, the length of the lexeme will be one unit less
    lengthLexeme--;
    //We return the last character read to the buffer
    reverseOneCharacter(lexicalParser->inputBuffer);
  } else if(lexicalParser->stateMachine->curState == end_state_not_return) {
  }
  //We allocate memory for the next lexical component in the code
  lexicalElement = (LexicalElement*)malloc(sizeof(LexicalElement));
  //We get the current lexeme from the input system
  lexicalElement->lexeme = returnLexeme(lexicalParser->inputBuffer, lengthLexeme);
  //Now that we have the lexeme, we get the lexical component
  lexicalElement->lexicalComponent = getLexicalComponent(lexicalParser->symbolTable, stateTransition, lexicalElement->lexeme);
  //Finally, we return the lexical element found
  return lexicalElement;
}

/*
 *Extracts the corresponding lexical component using the symbol table, the return value of the transition in the automata and the detected lexeme
 */
int getLexicalComponent(abb* symbolTable, int stateTransition, char* lexeme) {
  //The lexical component will be determined using the value returned during the state transition
  switch (stateTransition) {
    //In case we are dealing with an identifier, we have to check whether it is in the symbol table
    case IDENTIFIER: ;
      //The lexeme could be NULL in case the input system was not able to return it (due to memory limitations)
      //In that case, we cannot check whether it is a keyword or an identifier
      if(lexeme == NULL) return IDENTIFIER;
      //Element to introduce to read from the symbol table
      tipoelem t;
      //In case the identifier is not in the symbol table, these values will not be overwritten
      t.lexeme = NULL;
      t.lexicalComponent = IDENTIFIER;
      //We check if the identifier is in the symbol table
      buscar_nodo(*symbolTable, lexeme, &t);
      //If the value of lexeme is not overwritten, the identifier is not in the symbol table
      if(t.lexeme == NULL) {
        //In that case, we allocate memory for storing it
        size_t lexemeSize = strlen(lexeme)+1;
        t.lexeme = (char*)malloc(lexemeSize*sizeof(char));
        //We copy the contents of lexeme to the element in the tree
        strncpy(t.lexeme, lexeme, lexemeSize);
        //Since the element was not already in the symbol table, it should be an identifier
        t.lexicalComponent = IDENTIFIER;
        //We insert in the symbol table
        insertar(symbolTable, t);
      }
      return t.lexicalComponent;
    //The lexical components of operators and delimiters are get using their ASCII code
    //To avoid collisions we have to distinguish them by their number of characters
    //And introduce a shift with respect to the number of ASCII characters (128)
    //Note that the first character and their length uniquely identify each of them
    case SINGLE_OPERATOR:
      return lexeme[0];
    case SINGLE_DELIMITER:
      return lexeme[0];
    case DOUBLE_OPERATOR:
      return 128+lexeme[0];
    case DOUBLE_DELIMITER:
      return 128+lexeme[0];
    case TRIPLE_DELIMITER:
      return 256+lexeme[0];
    //The stateTransition already corresponds with the lexical componet
    default:
      return stateTransition;
  }
}
