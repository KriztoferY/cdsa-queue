C = gcc
CFLAGS = -std=c99 -O3 -march=native -DNDEBUG -DQUEUE_INIT_CAP=2
SRC = src
TEST = test
BIN = bin
LIB = lib

default: prep all

.PHONY : all
all: queue_circ_array_demo queue_linked_list_demo queue_merge_demo \
queue_circ_array_test queue_linked_list_test
	rm -f $(BIN)/*.o

prep:
	mkdir -p $(BIN) $(LIB)

queue_circ_array_demo: queue_demo.o libqueuearr.a 
	$(C) $(CFLAGS) -o $(BIN)/queue_circ_array_demo $(BIN)/queue_demo.o \
	-L./$(LIB) -lqueuearr

queue_linked_list_demo: queue_demo.o libqueuenode.a 
	$(C) $(CFLAGS) -o $(BIN)/queue_linked_list_demo $(BIN)/queue_demo.o \
	-L./$(LIB) -lqueuenode

queue_merge_demo: queue_merge_demo.o libqueuearr.a libqueuealgos.a
	$(C) $(CFLAGS) -o $(BIN)/queue_merge_demo $(BIN)/queue_merge_demo.o \
	-L./$(LIB) -lqueuenode -lqueuealgos

queue_demo.o:
	$(C) $(CFLAGS) -o $(BIN)/queue_demo.o -c $(SRC)/queue_demo.c

queue_merge_demo.o:
	$(C) $(CFLAGS) -o $(BIN)/queue_merge_demo.o -c $(SRC)/queue_merge_demo.c

queue_circ_array_test: test_queue_impl.o libqueuearr.a
	$(C) $(CFLAGS) -o $(BIN)/test_circ_array_queue $(BIN)/test_queue_impl.o \
	-L./$(LIB) -lqueuearr

queue_linked_list_test: test_queue_impl.o libqueuenode.a
	$(C) $(CFLAGS) -o $(BIN)/test_linked_list_queue $(BIN)/test_queue_impl.o \
	-L./$(LIB) -lqueuenode

test_queue_impl.o: 
	$(C) $(CFLAGS) -I$(SRC) -o $(BIN)/test_queue_impl.o -c $(TEST)/test_queue_impl.c

queue_circ_array.o:
	$(C) $(CFLAGS) -o $(BIN)/queue_circ_array.o -c $(SRC)/queue_circ_array.c

queue_linked_list.o:
	$(C) $(CFLAGS) -o $(BIN)/queue_linked_list.o -c $(SRC)/queue_linked_list.c

queue_algos.o:
	$(C) $(CFLAGS) -o $(BIN)/queue_algos.o -c $(SRC)/algos.c

libqueuearr.a: queue_circ_array.o
	ar rcs $(LIB)/libqueuearr.a $(BIN)/queue_circ_array.o 

libqueuenode.a: queue_linked_list.o
	ar rcs $(LIB)/libqueuenode.a $(BIN)/queue_linked_list.o 

libqueuealgos.a: queue_algos.o
	ar rcs $(LIB)/libqueuealgos.a $(BIN)/queue_algos.o 

libs: libqueuearr.a libqueuenode.a libqueuealgos.a

.PHONY : clean
clean:
	rm -f $(BIN)/queue_circ_array_demo $(BIN)/queue_linked_list_demo \
	$(BIN)/queue_merge_demo \
	$(BIN)/qqueue_circ_array_test $(BIN)/qqueue_linked_list_test \
	$(BIN)/*.o $(LIB)/*.a $(BIN)/*.gch