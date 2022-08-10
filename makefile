CC = gcc -Wall
LIBS = -lm
MAIN= compiler

SRCS = main.c stateMachine.c identifierStateMachine.c numberStateMachine.c commentStateMachine.c stringStateMachine.c abb.c input_system.c lex_parser.c syntax_parser.c operatorDelimitierStateMachine.c errors.c pila.c tabStateMachine.c

DEPS = stateMachine.h identifierStateMachine.h numberStateMachine.h commentStateMachine.h stringStateMachine.h abb.h defnitions.h input_system.h lex_parser.h syntax_parser.h operatorDelimitierStateMachine.h errors.h pila.h tabStateMachine.h

OBJS = $(SRCS:.c=.o)

$(MAIN): $(OBJS)
	$(CC) -o $(MAIN) $(OBJS) $(LIBS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<

cleanall: clean
	rm -f $(MAIN)

clean:
	rm -f *.o *.~w
