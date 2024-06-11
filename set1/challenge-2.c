#include <stdlib.h>
#include <string.h>
#include "encodings.h"

int main(int argc, char **argv)
{
    const char *left_input = "1c0111001f010100061a024b53535009181c";
    char *left_hex = parse_hex_string(left_input);
    
    const char *right_input = "686974207468652062756c6c277320657965";
    char *right_hex = parse_hex_string(right_input);

    // Both inputs are of equal size here
    size_t input_len = strlen(left_input) / 2;
    char *output = malloc(input_len + 1);

    if (output == NULL)
    {
        perror("Could not allocate buffer");
        exit(EXIT_FAILURE);
    }
    
    for (size_t i = 0; i < input_len; i++)
    {
        output[i] = *(left_hex + i) ^ *(right_hex + i);
    }
    output[input_len] = '\0';

    printf("Expected result 746865206b696420646f6e277420706c6179.\n Actual result:");
    
    for (size_t i = 0; i < input_len; i++)
    {
        printf("%02x", output[i]);
    }
    
    return 0;
}