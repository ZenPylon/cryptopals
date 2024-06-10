#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BITS_PER_BASE64_LETTER 6

static inline uint8_t hex_digit_to_int_val(char digit)
{
    // 0-9 digits
    if (digit >= 48 && digit <= 57)
    {
        return ((uint8_t)digit - 48);
    }
    // Lowercase hex values
    else if (digit >= 97 && digit <= 102)
    {
        // Render 'a' as 10, etc.
        return ((uint8_t)digit - 87);
    }
    else
    {
        fprintf(stderr, "Encountered invalid hex digit\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    // Populate base64_code
    char base64_code[65] = {0};

    // Upper case letters
    for (uint8_t i = 0; i < 26; i++)
    {
        // 65 for 'A'
        base64_code[i] = (char)(65 + i);
    }
    for (uint8_t i = 0; i < 26; i++)
    {
        // 97 for 'a', with offset 26 for the upper case letters added in loop above
        base64_code[26 + i] = (char)(97 + i);
    }
    for (uint8_t i = 0; i < 10; i++)
    {
        base64_code[i + 52] = (char)(i + 48);
    }
    base64_code[62] = '+';
    base64_code[63] = '/';

    const char *input = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    const char *ptr = input;
    size_t input_num_bytes = 0;

    while (*ptr != '\0')
    {
        ptr++;
    }
    // Excludes the null-byte
    input_num_bytes = ptr - input;
    size_t input_num_bits = input_num_bytes * 8;

    // Make sure that the number of bits in input is divisible by 6, the number of bits per base64 chars
    if (input_num_bits % BITS_PER_BASE64_LETTER != 0)
    {
        fprintf(stderr, "Invalid hex string in input");
        exit(EXIT_FAILURE);
    }

    const char *expected_output = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
    char *output = malloc(input_num_bytes + 1);
    for (size_t bit_offset = 0; bit_offset < input_num_bits; bit_offset += BITS_PER_BASE64_LETTER)
    {
        
    }
    output[input_num_bytes + 1] = '\0';

    printf("Hex string is: %s\n", input);
    printf("Expected base64 string is: %s\n", expected_output);
    printf("Actual base64 string is: %s\n", output);

    free(output);
}