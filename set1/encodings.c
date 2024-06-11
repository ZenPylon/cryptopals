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
        return -1;
    }

    for (size_t i = 0; i < input_num_bytes; i++)
    {
        output_buf[i] = hex_digit_to_int_val(input[2 * i]) << 4 | hex_digit_to_int_val(input[2 * i + 1]);
    }
}

char* base64_encode(char *input, size_t input_num_bytes)
{
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
        char left_bits = (0xFC >> left_bit_offset) & input[left_byte];

        // Select a quantity of "right_bit_offset" bits from the start of the right byte
        char right_bits = (0xFF << right_bit_offset) & input[right_byte];
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
    return output;
}