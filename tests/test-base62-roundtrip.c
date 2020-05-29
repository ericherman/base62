/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-base62-roundtrip.c: testing a base62 encoder */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#include <base62.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int base62_round_trip(int verbose, char *str)
{
	if (verbose) {
		printf("data in: %s\n", str);
	}

	int failures = 0;

	size_t str_len = str ? strlen(str) : 0;
	size_t buf_size = (4 + (str_len * 2)) * (sizeof(char));
	char *ebuf = calloc(1, buf_size);
	if (!ebuf) {
		++failures;
		fprintf(stderr, "failed to malloc(%zu)?\n", buf_size);
		goto round_trip_end;
	}

	char *encoded = base62_encode(ebuf, buf_size, (uint8_t *)str, str_len);
	if (verbose) {
		printf("encoded: %s\n", ebuf);
	}
	if (!encoded) {
		++failures;
		fprintf(stderr, "base62_encode returned NULL?\n");
	}
	if (encoded != ebuf) {
		++failures;
		fprintf(stderr, "encoded (%p) != buf (%p)?\n", (void *)encoded,
			(void *)ebuf);
	}
	for (size_t i = 0; i < buf_size && ebuf[i]; ++i) {
		char c = ebuf[i];
		if (isspace(ebuf[i])) {
			continue;
		}
		if (!((c >= 'A' && c <= 'Z')
		      || (c >= 'a' && c <= 'z')
		      || (c >= '0' && c <= '9'))) {
			fprintf(stderr, "encoded a '%c'?\n", c);
			++failures;
		}
	}

	uint8_t *dbuf = calloc(1, buf_size);
	if (!dbuf) {
		++failures;
		fprintf(stderr, "failed to malloc(%zu)?\n", buf_size);
		goto round_trip_end;
	}

	uint8_t *decoded = base62_decode(dbuf, buf_size, ebuf, buf_size);
	if (verbose) {
		printf("decoded: %s\n", (char *)dbuf);
	}
	if (!encoded) {
		++failures;
		fprintf(stderr, "base62_decode returned NULL?\n");
	}
	if (decoded != dbuf) {
		++failures;
		fprintf(stderr, "decoded (%p) != buf (%p)?\n",
			(void *)decoded, (void *)dbuf);
	}

	char *dstr = (char *)dbuf;
	if (strcmp(str, dstr) != 0) {
		++failures;
		fprintf(stderr, "expected '%s' but was '%s'\n", str, dstr);
	}

round_trip_end:
	free(dbuf);
	free(ebuf);
	return failures;
}

int main(int argc, char **argv)
{
	int verbose = argc > 1 ? atoi(argv[1]) : 0;

	int failures = 0;
	failures += base62_round_trip(verbose, "foo");
	failures +=
	    base62_round_trip(verbose,
			      "!@#$%^&*(QWERTYUIOPASDL:ZXCM<qwertasdfghjkl;asdfghjkl;zxcvbnmcvb\t\nakjdf;");

	failures += base62_round_trip(verbose, "< = > ?");

	failures += base62_round_trip(verbose, "f");
	failures += base62_round_trip(verbose, "=");
	failures += base62_round_trip(verbose, "==");
	failures += base62_round_trip(verbose, "===");
	failures += base62_round_trip(verbose, "====");
	failures += base62_round_trip(verbose, "=====");
	failures += base62_round_trip(verbose, "======");
	failures += base62_round_trip(verbose, "=======");
	failures += base62_round_trip(verbose, "========");
	failures += base62_round_trip(verbose, "f=");
	failures += base62_round_trip(verbose, "=f=");
	failures += base62_round_trip(verbose, "==f=");
	failures += base62_round_trip(verbose, "=f==f=");
	failures += base62_round_trip(verbose, "f==f===f");
	failures += base62_round_trip(verbose, "=f==f==f=");
	failures += base62_round_trip(verbose, "==f=f==f==f");
	failures += base62_round_trip(verbose, "=f=f=f=f=f=f=ff=");

	return failures ? EXIT_FAILURE : EXIT_SUCCESS;
}
