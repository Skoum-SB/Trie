CC= g++
SRC= indexation.cpp
OBJ= indexation.o
CFLAGS= -O3 -c -Wall -std=c++17

all: trie

trie: indexation.o
	$(CC) indexation.o -o trie


indexation.o:indexation.cpp
	$(CC) indexation.cpp $(CFLAGS)

clean:
	rm -rf *o all 