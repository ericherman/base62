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

	char chunk[BASE62_CHUNK_SIZE];
	size_t pos = 0;
	uint8_t buf[BASE62_CHUNK_SIZE];
	memset(buf, 0x00, BASE62_CHUNK_SIZE);

	size_t written = 0;
	int c = EOF;
	while ((c = fgetc(input)) != EOF) {
		chunk[pos++] = c;
		if (pos < BASE62_CHUNK_SIZE) {
			continue;
		}
		uint8_t *decoded = base62_decode(buf, BASE62_CHUNK_SIZE, &written, chunk, pos);
		if (!decoded) {
			return 1;
		}
		for (size_t i = 0; i < written; ++i) {
			fprintf(output, "%c", buf[i]);
		}
		written = 0;
		pos = 0;
	}
	if (pos) {
		uint8_t *decoded = base62_decode(buf, BASE62_CHUNK_SIZE, &written, chunk, pos);
		if (!decoded) {
			return 1;
		}
		for (size_t i = 0; i < written; ++i) {
			fprintf(output, "%c", buf[i]);
		}
	}
	return 0;
}
