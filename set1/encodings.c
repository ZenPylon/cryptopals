#include "encodings.h"
#include <string.h>

char base64_map[65];

char hex_digit_to_int_val(char digit)
{
    // 0-9 digits
    if (digit >= 48 && digit <= 57)
    {
        return (digit - 48);
    }
    // Lowercase hex values
    else if (digit >= 97 && digit <= 102)
    {
        // Render 'a' as 10, etc.
        return (digit - 87);
    }
    else
    {
        fprintf(stderr, "Encountered invalid hex digit\n");
        return -1;
    }
}

int parse_hex_string(const char *input, char *output_buf)
{
    size_t input_num_chars = strlen(input);
    size_t input_num_bytes = input_num_chars / 2;

    // Make sure our string-encoded hex input is valid
    if (input_num_bytes % 2 != 0 || input_num_chars % 2 != 0)
    {
        fprintf(stderr, "Invalid hex string in input");
        return -1;
    }

    for (size_t i = 0; i < input_num_bytes; i++)
    {
        output_buf[i] = hex_digit_to_int_val(input[2 * i]) << 4 | hex_digit_to_int_val(input[2 * i + 1]);
    }
}