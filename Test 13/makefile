CC = g++
SOURCES = Point.cpp Fish.cpp Boid.cpp
INCLUDES = Point.h Fish.h Boid.h
PROG = main.cpp
BINARY = ./main
FLAGS = -lglut -lGLU -lGL -Wall -g -lm -Wall -Werror

all:
	@$(CC) -o $(BINARY) $(PROG) $(SOURCES) $(FLAGS)

run:
	@$(BINARY)

clean:
	rm $(BINARY)