#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <math.h>

#include "../common/encodings.h"
#include "../common/bytes.h"

int main(int argc, char **argv)
{
    const char filename[] = "challenge-6.txt";
    
    // Load file
    struct stat file_stat;
    int ret = stat(filename, &file_stat);
    if (ret == -1)
    {
        perror("Could not stat file");
        exit(EXIT_FAILURE);
    }

    FILE *challenge_file = fopen(filename, "r");
    if (challenge_file == NULL)
    {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }
    
    char *buf = malloc(file_stat.st_size);
    if (buf == NULL)
    {
        perror("Could not allocate memory");
        exit(EXIT_FAILURE);
    }

    if (!fread(buf, 1, file_stat.st_size, challenge_file))
    {
        perror("Could not read file");
        exit(EXIT_FAILURE);
    }
    
    // Decode base64 into unencoded (but encrypted) bytes
    init_base64_module();
    char *encrypted_input = base64_decode(buf, file_stat.st_size);
    
    // Find the best keysize by testing which key produces the most similar stream of bytes.
    // This works because English characters are closer together in the ascii table than a randomly sampled byte.

    size_t best_keysize = 0;
    size_t num_test_blocks = 40;
    float best_hamming_distance = INFINITY;
    for (size_t keysize = 2; keysize < 41; keysize++)
    {
        float dist = 0.f;
        for (size_t test_block = 0; test_block < num_test_blocks; test_block++)
        {
            // Calculate the hamming distance between this test block and the next one
            dist += (float)hamming_distance(
                encrypted_input + (keysize * test_block), 
                encrypted_input + (keysize * (test_block + 1)), 
                keysize
            ) / (float)keysize;
        }
        // The challenge says to average the distance across the number of blocks, but every calculation 
        // has the same number of blocks included, so it shouldn't matter
        
        if (dist < best_hamming_distance)
        {
            printf("Smallest hamming distance now has keysize %d with distance of %f\n", keysize, dist);
            best_keysize = keysize;
            best_hamming_distance = dist;
        }
        else {
            printf("Passing on keysize %d with distance %f\n", keysize, dist);
        }
    }

    free(encrypted_input);
    free(buf);

    return 0;
}