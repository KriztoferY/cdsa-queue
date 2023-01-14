C = gcc
CFLAGS = -std=c99 -O3 -march=native -DNDEBUG -DQUEUE_INIT_CAP=2
SRC = src
TEST = test
BIN = bin
LIB = lib

default: prep all

.PHONY : all
all: circ_array_queue_demo linked_list_queue_demo merge_queues_demo \
test_circ_array_queue test_linked_list_queue \
test_merge_queues_circ_array test_merge_queues_linked_list
	rm -f $(BIN)/*.o

prep:
	mkdir -p $(BIN) $(LIB)

circ_array_queue_demo: queue_demo.o libqueuearr.a 
	$(C) $(CFLAGS) -o $(BIN)/circ_array_queue_demo $(BIN)/queue_demo.o \
	-L./$(LIB) -lqueuearr

linked_list_queue_demo: queue_demo.o libqueuenode.a 
	$(C) $(CFLAGS) -o $(BIN)/linked_list_queue_demo $(BIN)/queue_demo.o \
	-L./$(LIB) -lqueuenode

merge_queues_demo: merge_queues_demo.o libqueuearr.a libqueuealgos.a
	$(C) $(CFLAGS) -o $(BIN)/merge_queues_demo $(BIN)/merge_queues_demo.o \
	-L./$(LIB) -lqueuenode -lqueuealgos

queue_demo.o:
	$(C) $(CFLAGS) -o $(BIN)/queue_demo.o -c $(SRC)/queue_demo.c

merge_queues_demo.o:
	$(C) $(CFLAGS) -o $(BIN)/merge_queues_demo.o -c $(SRC)/merge_queues_demo.c

test_circ_array_queue: test_queue_impl.o libqueuearr.a
	$(C) $(CFLAGS) -o $(BIN)/test_circ_array_queue $(BIN)/test_queue_impl.o \
	-L./$(LIB) -lqueuearr

test_linked_list_queue: test_queue_impl.o libqueuenode.a
	$(C) $(CFLAGS) -o $(BIN)/test_linked_list_queue $(BIN)/test_queue_impl.o \
	-L./$(LIB) -lqueuenode

test_queue_impl.o: 
	$(C) $(CFLAGS) -I$(SRC) -o $(BIN)/test_queue_impl.o -c $(TEST)/test_queue_impl.c

test_merge_queues_circ_array: test_merge_queues.o libqueuearr.a libqueuealgos.a
	$(C) $(CFLAGS) -o $(BIN)/test_merge_queues_circ_array $(BIN)/test_merge_queues.o \
	-L./$(LIB) -lqueuearr -lqueuealgos

test_merge_queues_linked_list: test_merge_queues.o libqueuenode.a libqueuealgos.a
	$(C) $(CFLAGS) -o $(BIN)/test_merge_queues_linked_list $(BIN)/test_merge_queues.o \
	-L./$(LIB) -lqueuenode -lqueuealgos

test_merge_queues.o: 
	$(C) $(CFLAGS) -I$(SRC) -o $(BIN)/test_merge_queues.o -c $(TEST)/test_merge_queues.c

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
	rm -f $(BIN)/circ_array_queue_demo $(BIN)/linked_list_queue_demo \
	$(BIN)/merge_queues_demo \
	$(BIN)/test_circ_array_queue $(BIN)/test_linked_list_queue \
	$(BIN)/test_merge_queues_circ_array $(BIN)/test_merge_queues_linked_list \
	$(BIN)/*.o $(LIB)/*.a $(BIN)/*.gch