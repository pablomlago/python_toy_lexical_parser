#include <ctype.h>
#include <stdio.h>

#include "definitions.h"
#include "commentStateMachine.h"

int comm_single_line(TStateMachine *sm, char input) {
  if(input == '\n') {
    sm->curState = end_state_return;
    return COMMENT;
  }

  sm->curState = comm_single_line;
  return IN_PROGRESS;
}
