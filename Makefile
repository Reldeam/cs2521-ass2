CC=gcc
CFLAGS=-Wall -Werror -g
BINS=pagerank inverted searchTfIdf searchPagerank scaledFootrule

all : $(BINS)

pagerank : pagerank.o collection.o Queue.o BST.o HashMap.o LinkedList.o PriorityQueue.o Number.o
pagerank.o : pagerank.c collection.h Queue.h BST.h HashMap.h LinkedList.h PriorityQueue.h Number.h

inverted : inverted.o collection.o BST.o PriorityQueue.o HashMap.o Queue.o Number.o
inverted.o : inverted.c collection.h BST.h PriorityQueue.h HashMap.h Queue.h Number.h

searchPagerank : searchPagerank.o collection.o PriorityQueue.o BST.o Queue.o HashMap.o BST.o Number.o
searchPagerank.o : searchPagerank.c collection.h PriorityQueue.h BST.h Queue.h HashMap.h BST.h Number.h

searchTfIdf : searchTfIdf.o collection.o BST.o PriorityQueue.o HashMap.o Queue.o Number.o -lm
searchTfIdf.o : searchTfIdf.c collection.h BST.h PriorityQueue.h HashMap.h Queue.h Number.h

scaledFootrule : scaledFootrule.o BST.o Queue.o HashMap.o PriorityQueue.o Number.o
scaledFootrule.o : scaledFootrule.c BST.h Queue.h HashMap.h PriorityQueue.h Number.h

collection.o : collection.c collection.h Queue.h Number.h HashMap.h

Queue.o : Queue.c Queue.h
BST.o : BST.c BST.h Queue.h
PriorityQueue.o : PriorityQueue.c PriorityQueue.h BST.h
HashMap.o : HashMap.c HashMap.h BST.h
Number.o : Number.c Number.h

clean :
	rm -f $(BINS) *.o core *.dSYM
