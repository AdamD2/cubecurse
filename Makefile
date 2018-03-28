CC = gcc
CFLAGS = -Wall -Werror -g
LDFLAGS = -lncurses
OBJECTS = Time_list.o cubecurse.o 

cubecurse : Time_list.o cubecurse.o 
	    $(CC) -o cubecurse $(CFLAGS) $(LDFLAGS) $(OBJECTS)

Time_list.o : Time_list.c
	      cc -c -g Time_list.c

cubecurse.o : cubecurse.c
	      cc -c -g cubecurse.c

clean : 
	rm -f *.o cubecurse
