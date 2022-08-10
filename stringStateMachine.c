#include <ctype.h>
#include <stdio.h>

#include "definitions.h"
#include "stringStateMachine.h"

//Private states of the string literal state machine
int str_single_double_quote(TStateMachine *sm, char input);
int str_two_double_quote(TStateMachine *sm, char input);
int str_three_double_quote(TStateMachine *sm, char input);
int str_closing_one_double_quote(TStateMachine *sm, char input);
int str_closing_two_double_quote(TStateMachine *sm, char input);

int str_single_single_quote(TStateMachine *sm, char input);
int str_two_single_quote(TStateMachine *sm, char input);
int str_three_single_quote(TStateMachine *sm, char input);
int str_closing_one_single_quote(TStateMachine *sm, char input);
int str_closing_two_single_quote(TStateMachine *sm, char input);

//Auxiliar function to check if a character correspondes to the definition of shortstringchar in Python
int isShortStringChar(char input) {
  return input != '\\' && input && input != '\n' && input != '\'' && input != '\"';
}

//Auxiliar function to check if a character correspondes to the definition of longstringchar in Python
int isLongStringChar(char input) {
  return input != '\\';
}

int str_one_double_quote(TStateMachine *sm, char input) {
  if(isShortStringChar(input) || input == '\'') { //any source character except ...
    sm->curState = str_single_double_quote;
    return IN_PROGRESS;
  } else if(input == '\"') {
    sm->curState = str_two_double_quote;
    return IN_PROGRESS;
  }
  sm->curState = incorrect_state;
  return LEX_ERROR;
}

int str_single_double_quote(TStateMachine *sm, char input) {
  if(isShortStringChar(input) || input == '\'') { //any source character except ...
    sm->curState = str_single_double_quote;
    return IN_PROGRESS;
  } else if(input == '\"') {
    sm->curState = end_state_not_return;
    return STR_LITERAL;
  }
  sm->curState = incorrect_state;
  return LEX_ERROR;
}

int str_two_double_quote(TStateMachine *sm, char input) {
  if(input == '\"') {
    sm->curState = str_three_double_quote;
    return IN_PROGRESS;
  }

  sm->curState = end_state_not_return;
  return STR_LITERAL;
}

int str_three_double_quote(TStateMachine *sm, char input) {
  if(input == '\"') {
    sm->curState = str_closing_one_double_quote;
    return IN_PROGRESS;
  } else if(isLongStringChar(input)) {
    sm->curState = str_three_double_quote;
    return IN_PROGRESS;
  }

  sm->curState = incorrect_state;
  return LEX_ERROR;
}

int str_closing_one_double_quote(TStateMachine *sm, char input) {
  if(input == '\"') {
    sm->curState = str_closing_two_double_quote;
    return IN_PROGRESS;
  } else if(isLongStringChar(input)){
    sm->curState = str_three_double_quote;
    return IN_PROGRESS;
  }

  sm->curState = incorrect_state;
  return LEX_ERROR;
}

int str_closing_two_double_quote(TStateMachine *sm, char input) {
  if(input == '\"') {
    sm->curState = end_state_not_return;
    return STR_LITERAL;
  } else if(isLongStringChar(input)){
    sm->curState = str_three_double_quote;
    return IN_PROGRESS;
  }

  sm->curState = incorrect_state;
  return LEX_ERROR;
}

int str_one_single_quote(TStateMachine *sm, char input) {
  if(isShortStringChar(input) || input == '\"') { //any source character except ...
    sm->curState = str_single_single_quote;
    return IN_PROGRESS;
  } else if(input == '\'') {
    sm->curState = str_two_single_quote;
    return IN_PROGRESS;
  }

  sm->curState = incorrect_state;
  return LEX_ERROR;
}

int str_single_single_quote(TStateMachine *sm, char input) {
  if(isShortStringChar(input) || input == '\"') { //any source character except ...
    sm->curState = str_single_single_quote;
    return IN_PROGRESS;
  } else if(input == '\'') {
    sm->curState = end_state_not_return;
    return STR_LITERAL;
  }

  sm->curState = incorrect_state;
  return LEX_ERROR;
}

int str_two_single_quote(TStateMachine *sm, char input) {
  if(input == '\'') {
    sm->curState = str_three_single_quote;
    return IN_PROGRESS;
  }
  sm->curState = end_state_not_return;
  return STR_LITERAL;
}

int str_three_single_quote(TStateMachine *sm, char input) {
  if(input == '\'') {
    sm->curState = str_closing_one_single_quote;
    return IN_PROGRESS;
  } else if(isLongStringChar(input)) {
    sm->curState = str_three_single_quote;
    return IN_PROGRESS;
  }

  sm->curState = incorrect_state;
  return LEX_ERROR;
}

int str_closing_one_single_quote(TStateMachine *sm, char input) {
  if(input == '\'') {
    sm->curState = str_closing_two_single_quote;
    return IN_PROGRESS;
  } else if(isLongStringChar(input)){
    sm->curState = str_three_single_quote;
    return IN_PROGRESS;
  }

  sm->curState = incorrect_state;
  return LEX_ERROR;
}

int str_closing_two_single_quote(TStateMachine *sm, char input) {
  if(input == '\'') {
    sm->curState = end_state_not_return;
    return STR_LITERAL;
  } else if(isLongStringChar(input)){
    sm->curState = str_three_single_quote;
    return IN_PROGRESS;
  }

  sm->curState = incorrect_state;
  return LEX_ERROR;
}
