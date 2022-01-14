CC = g++
SOURCES = MyUtility.cpp Point.cpp Fish.cpp Boid.cpp 
INCLUDES = MyUtility.h Point.h Fish.h Boid.h
PROG = main.cpp
BINARY = ./main
FLAGS = -lglut -lGLU -lGL -Wall -g -lm -Wall -Werror

all:
	@$(CC) -o $(BINARY) $(PROG) $(SOURCES) $(FLAGS)

run:
	@$(BINARY)

clean:
	rm $(BINARY)