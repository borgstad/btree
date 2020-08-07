CC=gcc
CFLAGS=-I. -g

OBJ_PATH=bin/.obj
OBJ_PATHS=bin/.obj bin/.obj/tests
BIN_PATH=bin
TEST_PATH=tests
SRC_PATH=src
HDR_PATH=include

SRC := $(SRC_PATH)/btree.c $(SRC_PATH)/linked_list.c $(SRC_PATH)/hash.c \
		$(SRC_PATH)/storage.c $(SRC_PATH)/cache.c $(SRC_PATH)/io.c
OBJ := $(patsubst $(SRC_PATH)/%.c, $(OBJ_PATH)/%.o, $(SRC))

SRC_TEST := $(TEST_PATH)/test_btree.c $(TEST_PATH)/test_hash.c \
			$(TEST_PATH)/test_linked_list.c $(TEST_PATH)/test_storage.c \
			$(TEST_PATH)/test_cache.c
OBJ_TEST := $(patsubst %.c, $(OBJ_PATH)/%.o, $(SRC_TEST))
TESTS_BIN := $(patsubst tests/%.c, $(BIN_PATH)/%, $(SRC_TEST))


all: run_tests

$(BIN_PATH)/%: $(OBJ) $(OBJ_TEST)
	$(CC) $(OBJ_PATH)/tests/$*.o $(OBJ) -o $@

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c $(HDR_PATH)/%.h
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
