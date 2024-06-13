#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "encodings.h"

int main(int argc, char **argv)
{
    init_base64_module();
    const char *input = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";

    size_t input_num_chars = strlen(input);
    size_t input_num_bytes = input_num_chars / 2;

    char *input_as_bytes = parse_hex_string(input);
    if (input_as_bytes == NULL)
    {
        fprintf(stderr, "Invalid hex string in input");
        exit(EXIT_FAILURE);
    }

    char *output = base64_encode(input_as_bytes, input_num_bytes);

    const char *expected_output = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
    printf("Hex string is: %s\n", input);
    printf("Expected base64 string is: %s\n", expected_output);
    printf("Actual base64 string is: %s\n", output);

    free(input_as_bytes);
    free(output);
}