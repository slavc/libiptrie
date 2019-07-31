CFLAGS += -Wall -Werror -Wextra -pedantic -std=c11 -O2 -Isrc

.PHONY: all clean run-tests

all: libiptrie.so

libiptrie.so: src/iptrie.o
	$(CC) -shared -fPIC -o $@ $^

src/iptrie.o: src/iptrie.c
	$(CC) $(CFLAGS) -c -shared -fPIC -o $@ $^

clean:
	rm -f test_iptrie *.so src/*.o tests/*.o

run-tests: test_iptrie
	env LD_LIBRARY_PATH=. ./test_iptrie

test_iptrie: tests/test_iptrie.o libiptrie.so
	$(CC) -L. -liptrie -o $@ $<
