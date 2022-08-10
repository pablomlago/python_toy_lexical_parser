#ifndef __STR_STATE_LIB
#define __STR_STATE_LIB

#include "stateMachine.h"

//Public states of the string literal state machine
int str_one_double_quote(TStateMachine *sm, char input);
int str_one_single_quote(TStateMachine *sm, char input);

#endif	/* __STR_STATE_LIB */
