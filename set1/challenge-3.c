#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "encodings.h"

int main(int argc, char **argv)
{
    const char *input = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    char *hex_input = parse_hex_string(input);

    // Test every char against the input to see if it's the key
    size_t num_char_tests = 256;
    size_t input_len = strlen(input) / 2;

    // Store the XOR'd input against every possible char key
    char *output = malloc((input_len * num_char_tests) + 1);

    if (output == NULL)
    {
        perror("Could not allocate buffer");
        exit(EXIT_FAILURE);
    }

    // Use a lower number of of unique chars as a proxy for regular (e.g. English) text vs random bytes
    const char* common_letters = " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    size_t best_common_letter_score = 0;
    char best_key_guess = 0;

    for (size_t key_idx = 0; key_idx < num_char_tests; key_idx++)
    {
        printf("Testing key: %c\n", key_idx);

        char char_counts[256] = {0};
        size_t common_letter_score = 0;

        for (size_t char_idx = 0; char_idx < input_len; char_idx++)
        {
            // + key_idx to account for the null-byte of each string we've processed
            output[key_idx * input_len + key_idx + char_idx] = hex_input[char_idx] ^ ((char)key_idx);
            char_counts[hex_input[char_idx] ^ ((char)key_idx)]++;
            printf("%c", output[key_idx * input_len + key_idx + char_idx]);
        }

        for (int i = 0; i < strlen(common_letters); i++)
        {
            common_letter_score += char_counts[common_letters[i]];
        }
        if (common_letter_score > best_common_letter_score)
        {
            best_common_letter_score = common_letter_score;
            best_key_guess = key_idx;
        }
        printf("\n");
    }

    output[(input_len * num_char_tests) + 1] = '\0';
    printf("\n\nBest guess for the key is %c\n", best_key_guess);
    return 0;
}