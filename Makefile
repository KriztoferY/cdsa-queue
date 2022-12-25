C = gcc
CFLAGS = -std=c99 -O3 -march=native -DNDEBUG -DQUEUE_INIT_CAP=2
SRC = src
BIN = bin
LIB = lib

default: prep all

.PHONY : all
all: queue_circ_array_demo queue_linked_list_demo
	rm -f $(BIN)/*.o

prep:
	mkdir -p $(BIN) $(LIB)

queue_circ_array_demo: queue_demo.o libqueuearr.a 
	$(C) $(CFLAGS) -o $(BIN)/queue_circ_array_demo $(BIN)/queue_demo.o \
	-L./$(LIB) -lqueuearr

queue_linked_list_demo: queue_demo.o libqueuenode.a 
	$(C) $(CFLAGS) -o $(BIN)/queue_linked_list_demo $(BIN)/queue_demo.o \
	-L./$(LIB) -lqueuenode

queue_demo.o:
	$(C) $(CFLAGS) -o $(BIN)/queue_demo.o -c $(SRC)/queue_demo.c

queue_circ_array.o: $(SRC)/queue.h
	$(C) $(CFLAGS) -o $(BIN)/queue_circ_array.o -c $(SRC)/queue_circ_array.c

queue_linked_list.o: $(SRC)/queue.h
	$(C) $(CFLAGS) -o $(BIN)/queue_linked_list.o -c $(SRC)/queue_linked_list.c

libqueuearr.a: queue_circ_array.o
	ar rcs $(LIB)/libqueuearr.a $(BIN)/queue_circ_array.o 

libqueuenode.a: queue_linked_list.o
	ar rcs $(LIB)/libqueuenode.a $(BIN)/queue_linked_list.o 

libs: libqueuearr.a libqueuenode.a

.PHONY : clean
clean:
	rm -f $(BIN)/queue_circ_array_demo $(BIN)/queue_linked_list_demo \
	$(BIN)/*.o $(LIB)/*.a $(BIN)/*.gch