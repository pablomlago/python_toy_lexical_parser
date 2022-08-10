#ifndef __STATE_LIB
#define __STATE_LIB

/*enum StateTransitions {
  INITIAL,
  IN_PROGRESS,
  ERROR,
  END_COMMENT,
  END_IDENTIFIER,
  END_INT_LITERAL,
  END_FLOAT_LITERAL,
  END_STR_LITERAL,
  END_SINGLE_OPERATOR,
  END_DOUBLE_OPERATOR,
  END_SINGLE_DELIMITER,
  END_DOUBLE_DELIMITER,
  END_TRIPLE_DELIMITER,
  END_RETURN,
  END_NOT_RETURN,
  END_IGNORE,
  END_TAB,
  END_FILE
};*/

enum StateTransitions {
  INITIAL,
  IN_PROGRESS,
  END_RETURN,
  END_NOT_RETURN
};

//Forward declaration of TStateMachine
typedef struct StateMachine TStateMachine;

/*
 *Each state is represented as a function returning int which
 *receives a pointer to the state machine and a character who triggers
 *a transition
 */
typedef int (*StateProc)(TStateMachine *sm, char input);

//The StateMachien only holds a reference to the current state
struct StateMachine
{
  StateProc curState;
};

/*
 *Initializes the state machine
 */
void initializeStateMachine(TStateMachine* sm);
/*
 *Removes the state machine
 */
void removeStateMachine(TStateMachine* sm);

/*
 *Initial state in which no character has been processed
 */
int initial_state(TStateMachine *sm, char input);
/*
 *Incorrect_state to which we get when we find an unexpected transition in an automata
 */
int incorrect_state(TStateMachine *sm, char input);
/*
 *End state in which we have to return the latest character read to the input buffer
 */
int end_state_return(TStateMachine *sm, char input);
/*
 *End state in which we do not have to return the latest character read to the input buffer
 */
int end_state_not_return(TStateMachine *sm, char input);

#endif	/* __STATE_LIB */
