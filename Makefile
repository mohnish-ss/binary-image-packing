CC ?= cc
AR ?= ar
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror -g

LIBRARY = libimagepacking.a
OBJECT = image_packing.o
TEST_BINARY = test_image_packing
SANITIZER_BINARY = test_image_packing_sanitized

$(LIBRARY): $(OBJECT)
	$(AR) rcs $@ $^

$(OBJECT): image_packing.c image_packing.h
	$(CC) $(CFLAGS) -c image_packing.c -o $@

.PHONY: test asan clean
test: $(LIBRARY) tests/test_image_packing.c
	$(CC) $(CFLAGS) -I. tests/test_image_packing.c $(LIBRARY) -o $(TEST_BINARY)
	./$(TEST_BINARY)

asan: image_packing.c image_packing.h tests/test_image_packing.c
	$(CC) $(CFLAGS) -fno-omit-frame-pointer -fsanitize=address,undefined -I. image_packing.c tests/test_image_packing.c -o $(SANITIZER_BINARY)
	./$(SANITIZER_BINARY)

clean:
	rm -f $(OBJECT) $(LIBRARY) $(TEST_BINARY) $(SANITIZER_BINARY)
	rm -rf $(TEST_BINARY).dSYM $(SANITIZER_BINARY).dSYM
