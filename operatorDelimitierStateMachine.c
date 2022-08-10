#include <ctype.h>
#include <stdio.h>

#include "definitions.h"
#include "operatorDelimitierStateMachine.h"
#include "numberStateMachine.h"

//Private states of the operator and delimiter state machine
int op_del_exp_eq(TStateMachine *sm, char input);
int op_del_floor_div_eq(TStateMachine *sm, char input);
int op_del_two_less_eq(TStateMachine *sm, char input);
int op_del_two_plus_eq(TStateMachine *sm, char input);


int op_del_simple_eq(TStateMachine *sm, char input)
{
  if(input == '=') {
    sm->curState = end_state_not_return;
    return DOUBLE_DELIMITER;
  }

  sm->curState = end_state_return;
  return SINGLE_OPERATOR;
}

int op_del_mult_eq(TStateMachine *sm, char input)
{
  if(input == '=') {
    sm->curState = end_state_not_return;
    return DOUBLE_DELIMITER;
  } else if(input == '*') {
    sm->curState = op_del_exp_eq;
    return IN_PROGRESS;
  }

  sm->curState = end_state_return;
  return SINGLE_OPERATOR;
}

int op_del_exp_eq(TStateMachine *sm, char input)
{
  if(input == '=') {
    sm->curState = end_state_not_return;
    return TRIPLE_DELIMITER;
  }

  sm->curState = end_state_return;
  return DOUBLE_OPERATOR;
}

int op_del_div_eq(TStateMachine *sm, char input)
{
  if(input == '=') {
    sm->curState = end_state_not_return;
    return DOUBLE_DELIMITER;
  } else if(input == '/') {
    sm->curState = op_del_floor_div_eq;
    return IN_PROGRESS;
  }

  sm->curState = end_state_return;
  return SINGLE_OPERATOR;
}

int op_del_floor_div_eq(TStateMachine *sm, char input)
{
  if(input == '=') {
    sm->curState = end_state_not_return;
    return TRIPLE_DELIMITER;
  }

  sm->curState = end_state_return;
  return DOUBLE_OPERATOR;
}

int op_del_less_eq(TStateMachine *sm, char input) {
  if(input == '=') {
    sm->curState = end_state_not_return;
    return DOUBLE_OPERATOR;
  } else if(input == '>') {
    sm->curState =end_state_not_return;
    return DOUBLE_OPERATOR;
  } else if(input == '<') {
    sm->curState = op_del_two_less_eq;
    return IN_PROGRESS;
  }

  sm->curState = end_state_return;
  return SINGLE_OPERATOR;
}

int op_del_two_less_eq(TStateMachine *sm, char input) {
  if(input == '=') {
    sm->curState = end_state_not_return;
    return TRIPLE_DELIMITER;
  }

  sm->curState = end_state_return;
  return DOUBLE_OPERATOR;
}

int op_del_plus_eq(TStateMachine *sm, char input) {
  if(input == '=') {
    sm->curState = end_state_not_return;
    return DOUBLE_OPERATOR;
  } else if(input == '>') {
    sm->curState = op_del_two_plus_eq;
    return IN_PROGRESS;
  }

  sm->curState = end_state_return;
  return SINGLE_OPERATOR;
}

int op_del_two_plus_eq(TStateMachine *sm, char input) {
  if(input == '=') {
    sm->curState = end_state_not_return;
    return TRIPLE_DELIMITER;
  }

  sm->curState = end_state_return;
  return SINGLE_OPERATOR;
}

int op_del_dot(TStateMachine *sm, char input) {
  if(isdigit(input)) {
    sm->curState = float_point_s1;
    return IN_PROGRESS;
  }

  sm->curState = end_state_return;
  return SINGLE_OPERATOR;
}

int op_del_equals(TStateMachine *sm, char input) {
  if(input == '=') {
    sm->curState = end_state_not_return;
    return DOUBLE_OPERATOR;
  }

  sm->curState = end_state_return;
  return SINGLE_DELIMITER;
}
