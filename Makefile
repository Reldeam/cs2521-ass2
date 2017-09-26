CC=gcc
CFLAGS=-Wall -Werror -g
BINS=main

all : $(BINS)

#main : main.o util/BST.o util/Queue.o util/PriorityQueue.o util/LinkedList.o util/HashMap.o
#main.o : main.c util/BST.h util/Queue.h util/PriorityQueue.h

pagerank : pagerank.o util/HashMap.o
pagerank.o : pagerank.c HashMap.h

BST.o : util/BST.c util/BST.h util/Queue.h
Queue.o : util/Queue.c util/Queue.h
LinkedList.o : util/LinkedList.c util/LinkedList.h
PriorityQueue.o : util/PriorityQueue.c util/PriorityQueue.h util/BST.h
HashMap.o: util/HashMap.c util/HashMap.h


clean :
	rm -f $(BINS) *.o core *.dSYM