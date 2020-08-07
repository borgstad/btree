CC=gcc
CFLAGS=-I. -g

OBJ_PATH=bin/.obj
OBJ_PATHS=bin/.obj bin/.obj/tests
BIN_PATH=bin
TEST_PATH=tests

SRC := btree.c linked_list.c hash.c storage.c cache.c io.c
OBJ := $(patsubst %.c, $(OBJ_PATH)/%.o, $(SRC))

SRC_TEST := $(TEST_PATH)/btree_test.c $(TEST_PATH)/hash_test.c \
			$(TEST_PATH)/linked_list_test.c $(TEST_PATH)/storage_test.c
OBJ_TEST := $(patsubst %.c, $(OBJ_PATH)/%.o, $(SRC_TEST))
TESTS_BIN := $(patsubst tests/%.c, $(BIN_PATH)/%, $(SRC_TEST))


all: run_tests

$(BIN_PATH)/%: $(OBJ) $(OBJ_TEST)
	$(CC) $(OBJ_PATH)/tests/$*.o $(OBJ) -o $@

$(OBJ_PATH)/%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/tests/%.o: tests/%.c 
	$(CC) $(CFLAGS) -c $^ -o $@

$(SRC) $(SRC_TEST): | $(OBJ_PATHS)

$(OBJ_PATHS):
	mkdir -p $@

.PHONY: run_tests
run_tests: $(TESTS_BIN) 
	for BINARY in $$(ls bin); do $(BIN_PATH)/$$BINARY; done

clean: 
	rm -rf bin