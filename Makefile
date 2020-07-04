CC=gcc
CFLAGS=-I. -g
DEPS = btree.h io.h hash.h linked_list.h
OBJ = btree.o main.o io.o hash.o linked_list.o

TEST_HASH = tests/hash_test.o
TEST_BTREE = tests/btree_test.o
TEST_IO = tests/io_test.o
TEST_LINKED_LIST = tests/linked_list_test.o

TEST_DEP = btree.o io.o hash.o linked_list.o


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) $(OBJ) -o $@

test_all: test_btree test_hash

tests/%.o: tests/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: test_btree
test_btree: $(TEST_BTREE) $(TEST_DEP)	
	$(CC) $(TEST_BTREE) $(TEST_DEP) -o $@ && ./test_btree

test_hash: $(TEST_HASH) $(TEST_DEP)	
	$(CC) $(TEST_HASH) $(TEST_DEP) -o $@ && ./test_hash

test_linked_list: $(TEST_LINKED_LIST) $(TEST_DEP)	
	$(CC) $(TEST_LINKED_LIST) $(TEST_DEP) -o $@ && ./test_linked_list

.PHONY: test_io
test_io: $(TEST_IO) $(TEST_DEP)	
	$(CC) $(TEST_IO) $(TEST_DEP) -o $@ && ./test_io