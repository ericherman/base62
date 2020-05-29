default: check

CFLAGS=-g -Wall -Wextra -Werror -pipe

# extracted from https://github.com/torvalds/linux/blob/master/scripts/Lindent
LINDENT=indent -npro -kr -i8 -ts8 -sob -l80 -ss -ncs -cp1 -il0

base62.o: src/base62.c src/base62.h
	$(CC) -c $(CFLAGS) -o base62.o src/base62.c

test-base62-roundtrip: tests/test-base62-roundtrip.c base62.o
	$(CC) $(CFLAGS) -o test-base62-roundtrip \
		-Isrc \
		base62.o \
		tests/test-base62-roundtrip.c

check: test-base62-roundtrip
	./test-base62-roundtrip

tidy:
	$(LINDENT) \
		-T size_t -T ssize_t \
		-T uint8_t -T int8_t \
		`find src tests -name '*.h' -o -name '*.c'`

clean:
	rm -fv *.o test-base62-roundtrip
