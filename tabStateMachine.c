#include <stdio.h>

#include "definitions.h"
#include "tabStateMachine.h"

//Private state of the tab state machine
int tab_ignore_comment(TStateMachine *sm, char input);

int tab_whitespace(TStateMachine *sm, char input) {
  if(input == ' ') {
    sm->curState = tab_whitespace;
    return IN_PROGRESS;
  } else if(input == '\n') {
    sm->curState = end_state_return;
    return IGNORE;
  } else if(input == '#') {
    sm->curState = tab_ignore_comment;
    return IN_PROGRESS;
  }

  sm->curState = end_state_return;
  return TAB;
}

int tab_ignore_comment(TStateMachine *sm, char input) {
  if(input == '\n') {
    sm->curState = end_state_return;
    return IGNORE;
  }

  sm->curState = tab_ignore_comment;
  return IN_PROGRESS;
}
