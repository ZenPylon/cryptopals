#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "encodings.h"

#define NUM_LINES 327
#define HEX_CHARS_PER_LINE 60
#define BYTES_PER_LINE (HEX_CHARS_PER_LINE / 2)
int main(int argc, char **argv)
{
    FILE *input_file = fopen("chal4-file.txt", "r");
    // 327 lines * 60 chars/line + newline
    size_t num_bytes = (HEX_CHARS_PER_LINE + 1) * NUM_LINES + 1;
    char input_buf[num_bytes + 1];

    fread(input_buf, 1, num_bytes, input_file);
    
    if (ferror(input_file))
    {
        perror("Encountered error while reading file");
        exit(EXIT_FAILURE);
    }
    if (!feof(input_file))
    {
        fprintf(stderr, "Unexpected number of bytes in file");
        exit(EXIT_FAILURE);
    }

    // Translate each string of ascii hex into actual bytes
    // Replace newlines with null-strings so that parse_hex_string can recognize the end of the line
    char **encrypted_lines = malloc(NUM_LINES * sizeof(char *));
    for (size_t line_idx = 0; line_idx < NUM_LINES; line_idx++)
    {
        input_buf[line_idx * (HEX_CHARS_PER_LINE + 1) + HEX_CHARS_PER_LINE] = '\0';
        encrypted_lines[line_idx] = parse_hex_string(&input_buf[line_idx * (HEX_CHARS_PER_LINE + 1)]);
    }

    // Test every char against the input to see if it's the key
    size_t num_char_tests = 256;
    

    // A larger number of common letters indicates more plausible plaintext
    const char* common_letters = " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    size_t best_common_letter_score = 0;
    size_t best_line_guess = 0;
    char best_key_guess = 0;
    char best_plaintext[BYTES_PER_LINE + 1] = {0};

    for (size_t line_idx = 0; line_idx < NUM_LINES; line_idx++)
    {
        for (size_t key_idx = 0; key_idx < num_char_tests; key_idx++)
        {
            // printf("Testing key: %c\n", key_idx);

            char char_counts[256] = {0};
            size_t common_letter_score = 0;
            char curr_plaintext[BYTES_PER_LINE + 1] = {0};

            for (size_t char_idx = 0; char_idx < BYTES_PER_LINE; char_idx++)
            {
                // + key_idx to account for the null-byte of each string we've processed
                curr_plaintext[char_idx] = encrypted_lines[line_idx][char_idx] ^ ((char)key_idx);
                char_counts[curr_plaintext[char_idx]]++;
            }

            for (int i = 0; i < strlen(common_letters); i++)
            {
                common_letter_score += char_counts[common_letters[i]];
            }
            if (common_letter_score > best_common_letter_score)
            {
                best_common_letter_score = common_letter_score;
                best_line_guess = line_idx;
                best_key_guess = key_idx;
                memcpy(best_plaintext, curr_plaintext, BYTES_PER_LINE + 1);
            }
        }
    }

    printf("\n Best guess for the line number that was encrypted: %d\n", best_line_guess);
    printf("\nBest guess for the key is %c\n", best_key_guess);
    printf("\nBest guess plaintext: %s", best_plaintext);

    for (size_t line_idx = 0; line_idx < NUM_LINES; line_idx++)
    {
        free(encrypted_lines[line_idx]);
    }
    free(encrypted_lines);

    return 0;
}