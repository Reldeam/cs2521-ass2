CC=gcc
CFLAGS=-Wall -Werror -g
BINS=pagerank invertedIndex test

all : $(BINS)

test : test.o util/BST.o util/HashMap.o
test.o : test.c util/BST.c util/HashMap.h

pagerank : pagerank.o  util/HashMap.o util/BST.o util/LinkedList.o -lm
pagerank.o : pagerank.c util/HashMap.h util/BST.h util/LinkedList.h 

invertedIndex : invertedIndex.o  util/HashMap.o util/BST.o util/Queue.o util/PriorityQueue.o
invertedIndex.o : invertedIndex.c util/HashMap.h util/BST.h util/Queue.h util/PriorityQueue.h

main : main.o util/BST.o util/Queue.o util/PriorityQueue.o util/LinkedList.o util/HashMap.o
main.o : main.c util/BST.h util/Queue.h util/PriorityQueue.h

BST.o : util/BST.c util/BST.h util/Queue.h
Queue.o : util/Queue.c util/Queue.h
LinkedList.o : util/LinkedList.c util/LinkedList.h
PriorityQueue.o : util/PriorityQueue.c util/PriorityQueue.h util/BST.h
HashMap.o: util/HashMap.c util/HashMap.h util/BST.h



clean :
	rm -f $(BINS) *.o core *.dSYM