INCLUDES = -I./src/socket -I./src/events -I./src/player -I./src/communication -I./src/match

all: compile exec clean

compile:
	gcc -Wall -g -c ./src/**/*.c $(INCLUDES)
	gcc -Wall -g -c ./src/main.c $(INCLUDES)

exec: 
	gcc ./*.o -o server -lpthread

clean:
	rm -f ./*.o