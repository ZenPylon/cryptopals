#ifndef ENCODINGS_H
#define ENCODINGS_H
#include <stdlib.h>
#include <stdio.h>

// Populate base64_encode
extern char base64_map[65];

// Must be called before using other functions
static void populate_base64_map()
{
    // Upper case letters
    for (uint8_t i = 0; i < 26; i++)
    {
        // 65 for 'A'
        base64_map[i] = (char)(65 + i);
    }
    for (uint8_t i = 0; i < 26; i++)
    {
        // 97 for 'a', with offset 26 for the upper case letters added in loop above
        base64_map[26 + i] = (char)(97 + i);
    }
    for (uint8_t i = 0; i < 10; i++)
    {
        base64_map[i + 52] = (char)(i + 48);
    }
    base64_map[62] = '+';
    base64_map[63] = '/';
}

char hex_digit_to_int_val(char digit);
int parse_hex_string(const char *input, char *output_buf);
#endif