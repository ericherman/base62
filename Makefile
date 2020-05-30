default: all

CFLAGS=-g -Wall -Wextra -Werror -pipe -Isrc/

# extracted from https://github.com/torvalds/linux/blob/master/scripts/Lindent
LINDENT=indent -npro -kr -i8 -ts8 -sob -l80 -ss -ncs -cp1 -il0

base62.o: src/base62.c src/base62.h
	$(CC) -c $(CFLAGS) -o base62.o src/base62.c

base62encode: src/base62encode.c base62.o
	$(CC) $(CFLAGS) -o base62encode base62.o src/base62encode.c


base62decode: src/base62decode.c base62.o
	$(CC) $(CFLAGS) -o base62decode base62.o src/base62decode.c


test-base62-roundtrip: tests/test-base62-roundtrip.c base62.o
	$(CC) $(CFLAGS) base62.o \
		-o test-base62-roundtrip \
		tests/test-base62-roundtrip.c

check: test-base62-roundtrip
	./test-base62-roundtrip

all: base62encode base62decode

tidy:
	$(LINDENT) \
		-T size_t -T ssize_t \
		-T uint8_t -T int8_t \
		`find src tests -name '*.h' -o -name '*.c'`

clean:
	rm -fv *.o base62encode base62decode test-base62-roundtrip
