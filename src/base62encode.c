/* SPDX-License-Identifier: GPL-3.0-or-later */
/* main.c for base62 */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#ifndef BASE62_CHUNK_SIZE
#define BASE62_CHUNK_SIZE 40
#endif

#include <base62.h>

#include <string.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	FILE *input, *output;

	if (argc > 1) {
		const char *path = argv[1];
		input = fopen(path, "r");
		if (!input) {
			fprintf(stderr, "could not open %s\n", path);
			return 1;
		}
	} else {
		input = stdin;
	}

	if (argc > 2) {
		const char *path = argv[2];
		output = fopen(path, "w+");
		if (!input) {
			fprintf(stderr, "could not open %s\n", path);
			return 1;
		}
	} else {
		output = stdout;
	}

	unsigned char chunk[BASE62_CHUNK_SIZE];
	size_t pos = 0;
	size_t buf_size = (2 * BASE62_CHUNK_SIZE) + 8;
	char buf[buf_size];
	memset(buf, 0x00, buf_size);

	int c = EOF;
	while ((c = fgetc(input)) != EOF) {
		chunk[pos++] = c;
		if (pos < BASE62_CHUNK_SIZE) {
			continue;
		}
		char *encode =
		    base62_encode(buf, buf_size, chunk, pos);
		fprintf(output, "%s\n", encode);
		pos = 0;
		memset(buf, 0x00, buf_size);
	}
	if (pos) {
		char *encode =
		    base62_encode(buf, buf_size, chunk, pos);
		fprintf(output, "%s\n", encode);
	}
	return 0;
}
