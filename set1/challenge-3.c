#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    const char* input = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    char *hex_input = parse_hex_string(input);


    // Test every char against the input to see if it's the key
    size_t num_char_tests = 256;
    size_t input_len = strlen(input) / 2;

    // Store the XOR'd input 
    char *output = malloc((input_len * num_char_tests) + 1);

    if (output == NULL)
    {
        perror("Could not allocate buffer");
        exit(EXIT_FAILURE);
    }

    for (size_t key_idx = 0; key_idx < num_char_tests; key_idx++)
    {
        for (size_t char_idx = 0; char_idx <= input_len; char_idx++)
        {
            output[key_idx * 256 + char_idx] = hex_input[char_idx] ^ ((char)key_idx);
        }
    }
    
    output[(input_len * num_char_tests) + 1] = '\0';
    return 0;
}