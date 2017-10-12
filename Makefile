CC=gcc
CFLAGS=-Wall -Werror -g
BINS=test pagerank invertedIndex

all : $(BINS)

test : test.o util/BST.o util/Queue.o util/HashMap.o collection.o
test.o : test.c util/BST.h util/Queue.h util/PriorityQueue.h util/HashMap.h

pagerank : pagerank.o collection.o util/Queue.o util/BST.o util/HashMap.o util/LinkedList.o
pagerank.o : pagerank.c collection.h util/Queue.h util/BST.h util/HashMap.h util/LinkedList.h

invertedIndex : invertedIndex.o collection.o util/BST.o util/PriorityQueue.o util/HashMap.o util/Queue.o
invertedIndex.o : invertedIndex.c collection.h util/BST.h util/PriorityQueue.h util/HashMap.h util/Queue.h

collection.o : collection.c collection.h util/Queue.h

Queue.o : util/Queue.c util/Queue.h
BST.o : util/BST.c util/BST.h util/Queue.h
PriorityQueue.o : util/PriorityQueue.c util/PriorityQueue.h util/BST.h
HashMap.o : util/HashMap.c util/HashMap.h util/BST.h

clean :
	rm -f $(BINS) *.o core *.dSYM