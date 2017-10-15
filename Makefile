CC=gcc
CFLAGS=-Wall -Werror -g
BINS=test pagerank invertedIndex searchTfIdf searchPagerank

all : $(BINS)

test : test.o util/BST.o util/Queue.o util/HashMap.o collection.o util/PriorityQueue.o
test.o : test.c util/BST.h util/Queue.h util/PriorityQueue.h util/HashMap.h util/PriorityQueue.h

pagerank : pagerank.o collection.o util/Queue.o util/BST.o util/HashMap.o util/LinkedList.o
pagerank.o : pagerank.c collection.h util/Queue.h util/BST.h util/HashMap.h util/LinkedList.h

invertedIndex : invertedIndex.o collection.o util/BST.o util/PriorityQueue.o util/HashMap.o util/Queue.o
invertedIndex.o : invertedIndex.c collection.h util/BST.h util/PriorityQueue.h util/HashMap.h util/Queue.h

searchPagerank : searchPagerank.o util/PriorityQueue.o util/BST.o util/Queue.o util/HashMap.o util/BST.o
searchPagerank.o : searchPagerank.c util/PriorityQueue.h util/BST.h util/Queue.h util/HashMap.h util/BST.h

searchTfIdf : searchTfIdf.o collection.o util/BST.o util/PriorityQueue.o util/HashMap.o util/Queue.o -lm
searchTfIdf.o : searchTfIdf.c collection.h util/BST.h util/PriorityQueue.h util/HashMap.h util/Queue.h 

collection.o : collection.c collection.h util/Queue.h

Queue.o : util/Queue.c util/Queue.h
BST.o : util/BST.c util/BST.h util/Queue.h
PriorityQueue.o : util/PriorityQueue.c util/PriorityQueue.h util/BST.h
HashMap.o : util/HashMap.c util/HashMap.h util/BST.h

clean :
	rm -f $(BINS) *.o core *.dSYM
