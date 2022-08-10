#ifndef __NUM_STATE_LIB
#define __NUM_STATE_LIB

#include "stateMachine.h"

//Public states of the number state machine
int int_decimal(TStateMachine *sm, char input);
int int_non_decimal(TStateMachine *sm, char input);
int float_point_s1(TStateMachine *sm, char input);

#endif	/* __INT_STATE_LIB */
