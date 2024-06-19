#ifndef ENCODINGS_H
#define ENCODINGS_H

#define BITS_PER_BASE64_LETTER 6

#include <stdlib.h>
#include <stdio.h>

// Populate base64_encode
extern char base64_encode_map[64];
extern char base64_decode_map[256];

void init_base64_module();
void populate_base64_encode_map();
void populate_base64_decode_map();
void populate_monogram_freq();

char hex_digit_to_int_val(char digit);
char *parse_hex_string(const char *input);

char *base64_encode(char *input, size_t input_num_bytes);
char *base64_decode(char *input, size_t input_num_bytes);

// Loads a base64 encoded file, allocates a buffer, and returns the buffer containing the decoded bytes
// Ignores newlines
// Caller responsible for freeing
char *base64_decode_file(char *filename, size_t *outlen);

float get_english_score(char *string, size_t length);

#endif