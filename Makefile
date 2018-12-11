CC = gcc
CFLAGS = -Wall -Werror -g
LDFLAGS = -lncurses
OBJECTS = Time_list.o cubecurse.o 

cubecurse : Time_list.o cubecurse.o 
	    $(CC) -o cubecurse $(CFLAGS) $(LDFLAGS) $(OBJECTS)

Time_list.o : src/time_list/Time_list.c
	      cc -c -g src/time_list/Time_list.c

cubecurse.o : src/cubecurse.c
	      cc -c -g src/cubecurse.c

clean : 
	rm -f *.o cubecurse
