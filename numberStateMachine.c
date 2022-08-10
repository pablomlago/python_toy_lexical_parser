#include <ctype.h>

#include "definitions.h"
#include "numberStateMachine.h"

//Private states of the number state machine
int int_binary(TStateMachine *sm, char input);
int int_octal(TStateMachine *sm, char input);
int int_hexadecimal(TStateMachine *sm, char input);
int float_point_s2(TStateMachine *sm, char input);
int float_exp(TStateMachine *sm, char input);
int float_sign_exp(TStateMachine *sm, char input);
int float_decimal_digits(TStateMachine *sm, char input);

int int_decimal(TStateMachine *sm, char input) {
  if(isdigit(input)) {
    sm->curState = int_decimal;
    return IN_PROGRESS;
  } else if(input == '.') {
    sm->curState = float_point_s1;
    return IN_PROGRESS;
  } else if(input == 'e' || input == 'E') {
    sm->curState = float_exp;
    return IN_PROGRESS;
  }

  sm->curState = end_state_return;
  return INT_LITERAL;
}

int float_point_s1(TStateMachine *sm, char input) {
  if(isdigit(input)) {
    sm->curState = float_point_s2;
    return IN_PROGRESS;
  }

  sm->curState = end_state_return;
  return FLOAT_LITERAL;
}

int float_point_s2(TStateMachine *sm, char input) {
  if(isdigit(input)) {
    sm->curState = float_exp;
    return IN_PROGRESS;
  }

  sm->curState = end_state_return;
  return FLOAT_LITERAL;
}

int float_exp(TStateMachine *sm, char input) {
  if(isdigit(input)) {
    sm->curState = float_decimal_digits;
    return IN_PROGRESS;
  } else if(input == '+' || input == '-') {
    sm->curState = float_sign_exp;
    return IN_PROGRESS;
  }

  sm->curState = end_state_return;
  return FLOAT_LITERAL;
}

int float_sign_exp(TStateMachine *sm, char input) {
  if(isdigit(input)) {
    sm->curState = float_decimal_digits;
    return IN_PROGRESS;
  }

  sm->curState = end_state_return;
  return FLOAT_LITERAL;
}

int float_decimal_digits(TStateMachine *sm, char input) {
  if(isdigit(input)) {
    sm->curState = float_decimal_digits;
    return IN_PROGRESS;
  }

  sm->curState = end_state_return;
  return FLOAT_LITERAL;
}

int int_non_decimal(TStateMachine *sm, char input) {
  if(input == 'b' || input == 'B') {
    sm->curState = int_binary;
    return IN_PROGRESS;
  } else if(input == 'x' || input == 'X') {
    sm->curState = int_hexadecimal;
    return IN_PROGRESS;
  } else if(input == 'o' || input == 'O') {
    sm->curState = int_octal;
    return IN_PROGRESS;
  }

  sm->curState = end_state_return;
  return INT_LITERAL;
}

int int_binary(TStateMachine *sm, char input) {
  if(input == '0' || input == '1') {
    sm->curState = int_binary;
    return IN_PROGRESS;
  }

  sm->curState = end_state_return;
  return INT_LITERAL;
}

int int_octal(TStateMachine *sm, char input) {
  if('0' <= input && input <= '7') {
    sm->curState = int_octal;
    return IN_PROGRESS;
  }

  sm->curState = end_state_return;
  return INT_LITERAL;
}

int int_hexadecimal(TStateMachine *sm, char input) {
  if(isdigit(input)) {
    sm->curState = int_hexadecimal;
    return IN_PROGRESS;
  } else if('A' <= input && input <= 'F') {
    sm->curState = int_hexadecimal;
    return IN_PROGRESS;
  }
  sm->curState = end_state_return;
  return INT_LITERAL;
}
