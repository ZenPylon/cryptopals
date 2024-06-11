#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "encodings.h"

#define BITS_PER_BASE64_LETTER 6


int main(int argc, char **argv)
{
    populate_base64_map();
    const char *input = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";

    size_t input_num_chars = strlen(input);
    size_t input_num_bytes = input_num_chars / 2;
    size_t input_num_bits = input_num_bytes * 8;

    // Make sure our string-encoded hex input is valid
    if (input_num_bytes % 2 != 0 || input_num_chars % 2 != 0)
    {
        fprintf(stderr, "Invalid hex string in input");
        exit(EXIT_FAILURE);
    }

    char input_as_bytes[input_num_bytes];
    parse_hex_string(input, input_as_bytes);

    // 8 bits of a base64 ASCII character correspond to 6 bits of input, which is a 4/3 ratio in bytes.
    // Then add any remaining bytes for padding (encoded as '=')
    size_t output_padding = input_num_bytes % 3;
    size_t output_size = 4 * (input_num_bytes / 3);

    // +1 for null byte
    char *output = calloc(output_size + output_padding + 1, 1);
    size_t input_bit_offset = 0;

    for (size_t i = 0; i < output_size; i++)
    {
        size_t left_byte = input_bit_offset / 8;
        size_t left_bit_offset = input_bit_offset % 8;
        size_t right_byte = (input_bit_offset + BITS_PER_BASE64_LETTER) / 8;
        size_t right_bit_offset = 8 - (input_bit_offset + BITS_PER_BASE64_LETTER) % 8;

        // For the left bits, extract from left_bit_offset to either 6 bits (using mask 0xFC to zero out the last two bits of the byte)
        // or until the end of the byte (whichever comes first)
        char left_bits = (0xFC >> left_bit_offset) & input_as_bytes[left_byte];

        // Select a quantity of "right_bit_offset" bits from the start of the right byte
        char right_bits = (0xFF << right_bit_offset) & input_as_bytes[right_byte];
        char bit_val;

        if (left_bit_offset == 0)
        {
            // Just grab bits 0 through 5, and shift over two to ensure it's a 6-bit value
            bit_val = left_bits >> 2;
        }
        else if (left_bit_offset == 6)
        {
            bit_val = (left_bits << 4) | (right_bits >> 4);
        }
        else if (left_bit_offset == 4)
        {
            bit_val = (left_bits << 2) | (right_bits >> 6);
        }
        else // left_bit_offset == 2
        {
            bit_val = left_bits;
        }
        output[i] = base64_map[bit_val];
        input_bit_offset += BITS_PER_BASE64_LETTER;
    }

    const char *expected_output = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
    printf("Hex string is: %s\n", input);
    printf("Expected base64 string is: %s\n", expected_output);
    printf("Actual base64 string is: %s\n", output);

    free(output);
}