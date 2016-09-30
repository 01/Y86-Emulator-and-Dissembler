#Variable definition
CC  = gcc -g 
CCFLAGS = -Wall 

#rules definition
all : y86emul

y86emul : y86emul.c y86emul.h
	$(CC) $(CCFLAGS) -o y86emul y86emul.c y86emul.h 

#This rule cleans up executable file
clean: 
	rm -f y86emul extra2
