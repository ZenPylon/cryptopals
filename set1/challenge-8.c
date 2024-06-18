#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#include "../common/encodings.h"

int main(int argc, char **argv)
{
    // Let's be lazy and just use what we know of the file from manual inspection
    size_t num_lines = 204;
    size_t line_width = 320 ;
    size_t line_bytes = line_width / 2;
    size_t block_size = 16;
    size_t blocks_per_line = line_bytes / block_size;
    char buf[line_width];
    int max_repeats = 0;
    int suspected_line = 0;


    int ret = EXIT_FAILURE;
    FILE *challenge_file = fopen("challenge-8-file.txt", "r");

    if (challenge_file == NULL)
        goto error;


    for (size_t lineno = 0; lineno <= num_lines; lineno++)
    {
        // +1 for the newline at the end of each line
        fread(buf, 1, line_width + 1, challenge_file);
        if (ferror(challenge_file))
            goto error;

        // Replace the newline with a null-byte to terminate the string for the call to parse_hex_string()
        buf[line_width] = '\0';
        char *hex_string = parse_hex_string(buf);

        // We'll do a naive n^2 implementation here to compare each block to every other block
        // We take repeated blocks to be a sign of ECB encryption, since repeated plaintext leads to repeated ciphers
        // (And presumably because a repeated random bitstream is very unlikely).
        // TOOD - improve with a hash table
        size_t num_repeats = 0;
        for (size_t block1 = 0; block1 < blocks_per_line - 1; block1++)
        {
            for (size_t block2 = block1 + 1; block2 < blocks_per_line; block2++)
            {
                size_t matching = 1;
                for (size_t byte = 0; byte < block_size; byte++)
                {
                    if (hex_string[block1 * block_size + byte] != hex_string[block2 * block_size + byte])
                    {
                        matching = 0;
                        break;
                    }
                }
                num_repeats += matching;
            }
        }  

        printf("Repeat count for line %d: %d\n", lineno, num_repeats);
        if (num_repeats > max_repeats )
        {
            printf("That's a new maximum!\n");
            suspected_line = lineno;
            max_repeats = num_repeats;
        }
    }

    printf("Suspected line has index %d\n", suspected_line);

error:
    if (errno != 0)
        fprintf(stderr, "Error: %s", strerror(errno));
    
    return ret;
}