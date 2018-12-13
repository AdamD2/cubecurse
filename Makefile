CC = gcc
CFLAGS = -Wall -Werror -g
LDFLAGS = -lncurses
OBJECTS = Time_list.o Scramble.o cubecurse.o 

cubecurse : Time_list.o Scramble.o cubecurse.o 
	    $(CC) -o cubecurse $(CFLAGS) $(LDFLAGS) $(OBJECTS)

Time_list.o : src/time_list/Time_list.c
	      cc -c -g src/time_list/Time_list.c

Scramble.o : src/scramble/Scramble.c
		  cc -c -g src/scramble/Scramble.c

cubecurse.o : src/cubecurse.c
	      cc -c -g src/cubecurse.c

clean : 
	rm -f *.o cubecurse
