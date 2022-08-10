#include <ctype.h>
#include <stdio.h>

#include "definitions.h"

#include "identifierStateMachine.h"
#include "numberStateMachine.h"
#include "commentStateMachine.h"
#include "stringStateMachine.h"
#include "operatorDelimitierStateMachine.h"
#include "tabStateMachine.h"

/*
 *Initializes the state machine
 */
void initializeStateMachine(TStateMachine* sm) {
  //We set the state to the initial state
  sm->curState = initial_state;
}

/*
 *Removes the state machine
 */
void removeStateMachine(TStateMachine* sm) {
}

//Auxiliar function to determine if a simple character is a delimiter
int isSimpleDelimiter(char input) {
  return input == '(' || input == ')' ||
    input == '[' || input == ']' ||
    input == '{' || input  == '}' ||
    input == ',' || input == ':' ||
    input == ';' || input == '@';
}

int initial_state(TStateMachine *sm, char input) {
  if(input == '_' || isalpha(input)) {
    //We move to the identifier automata
    sm->curState = id_continue;
    return IN_PROGRESS;
  } else if(input == '0') {
    //We move to the number automata
    sm->curState = int_non_decimal;
    return IN_PROGRESS;
  } else if('1' <= input && input <= '9') {
    //We move to the number automata
    sm->curState = int_decimal;
    return IN_PROGRESS;
  } else if(input == '.') {
    //We move to the delimiter and operator automata
    sm->curState = op_del_dot;
    return IN_PROGRESS;
  } else if(input == '#') {
    //We move to the comment automata
    sm->curState = comm_single_line;
    return IN_PROGRESS;
  } else if(input == '\"') {
    //We move to the string literal automata
    sm->curState = str_one_double_quote;
    return IN_PROGRESS;
  } else if(input == '\'') {
    //We move to the string literal automata
    sm->curState = str_one_single_quote;
    return IN_PROGRESS;
  } else if(input == '+' || input == '-' || input == '&' || input == '|' || input =='%' || input == '^') {
    //We move to the delimiter and operator automata
    sm->curState = op_del_simple_eq;
    return IN_PROGRESS;
  } else if(input == '*') {
    //We move to the delimiter and operator automata
    sm->curState = op_del_mult_eq;
    return IN_PROGRESS;
  } else if(input == '/') {
    //We move to the delimiter and operator automata
    sm->curState = op_del_div_eq;
    return IN_PROGRESS;
  } else if(input == '~') {
    //We move to the end state in which we do not return the character
    sm->curState = end_state_not_return;
    return SINGLE_OPERATOR;
  } else if(isSimpleDelimiter(input)) {
    //We move to the end state in which we do not return the character
    sm->curState = end_state_not_return;
    return SINGLE_DELIMITER;
  } else if(input == '<') {
    //We move to the delimiter and operator automata
    sm->curState = op_del_less_eq;
    return IN_PROGRESS;
  } else if(input == '>') {
    //We move to the delimiter and operator automata
    sm->curState = op_del_plus_eq;
    return IN_PROGRESS;
  } else if(input == '=') {
    //We move to the delimiter and operator automata
    sm->curState = op_del_equals;
    return IN_PROGRESS;
  } else if(input == '\n') {
    //We move to the tab automata
    sm->curState = tab_whitespace;
    return TAB;
  } else if(input == EOF) {
    //We move to the end state in which we do not return the character
    sm->curState = end_state_not_return;
    return EOF;
  }
  //If none of the situations above verifies, we go back to the initial state
  sm->curState = initial_state;
  return INITIAL;
}

/*
 *Incorrect_state to which we get when we find an unexpected transition in an automata
 */
int incorrect_state(TStateMachine *sm, char input) {
  sm->curState = incorrect_state;
  return LEX_ERROR;
}
/*
 *End state in which we have to return the latest character read to the input buffer
 */
int end_state_return(TStateMachine *sm, char input) {
  sm->curState = end_state_return;
  return END_RETURN;
}
/*
 *End state in which we do not have to return the latest character read to the input buffer
 */
int end_state_not_return(TStateMachine *sm, char input) {
  sm->curState = end_state_not_return;
  return END_NOT_RETURN;
}
