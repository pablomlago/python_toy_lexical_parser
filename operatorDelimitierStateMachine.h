#ifndef __OP_STATE_LIB
#define __OP_STATE_LIB

#include "stateMachine.h"

//Public states of the operator and delimiter state machine
int op_del_simple_eq(TStateMachine *sm, char input);
int op_del_mult_eq(TStateMachine *sm, char input);
int op_del_div_eq(TStateMachine *sm, char input);
int op_del_less_eq(TStateMachine *sm, char input);
int op_del_plus_eq(TStateMachine *sm, char input);
int op_del_dot(TStateMachine *sm, char input);
int op_del_equals(TStateMachine *sm, char input);

#endif	/* __INT_STATE_LIB */
