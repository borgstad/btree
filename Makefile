CC=gcc
CFLAGS=-I.
DEPS = btree.h
OBJ = btree.o main.o

TEST = tests/btree_test.o
TEST_DEP = btree.o


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

btree: $(OBJ)
	$(CC) $(OBJ) -o $@

tests/%.o: tests/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

unit_test: $(TEST) $(TEST_DEP)	
	$(CC) $(TEST) $(TEST_DEP) -o $@ && ./unit_test
