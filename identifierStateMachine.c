#include <ctype.h>
#include <stdio.h>

#include "definitions.h"
#include "identifierStateMachine.h"

int id_continue(TStateMachine *sm, char input)
{
  if(input == '_' || isalnum(input)) {
    sm->curState = id_continue;
    return IN_PROGRESS;
  }

  sm->curState = end_state_return;
  return IDENTIFIER;
}
