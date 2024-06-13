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

char hex_digit_to_int_val(char digit);
char* parse_hex_string(const char *input);

// Allocates a base64 encoded string from a series of bytes. Caller is responsible for freeing
char* base64_encode(char *input, size_t input_num_bytes);
char* base64_decode(char *input, size_t input_num_bytes);

#endif