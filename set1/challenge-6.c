#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <math.h>
#include <string.h>

#include "../common/encodings.h"

#include "../common/bytes.h"

int main(int argc, char **argv)
{
    const char filename[] = "challenge-6-nonewlines.txt";
    
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
    size_t encrypted_input_size = 3 * file_stat.st_size / 4;
    for (size_t i = 0; i < encrypted_input_size; i++)
    {
        printf("%c", encrypted_input[i]);
    }
    
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
    // best_keysize = 5;

    // Each bit_block is an array of the nth bit from each keysize block, repeated until the end of the file
    char **bit_blocks = malloc(best_keysize * sizeof(char*));
    size_t num_full_blocks = encrypted_input_size / best_keysize;
    size_t extra_bits = encrypted_input_size % best_keysize;
    for (size_t i = 0; i < best_keysize; i++)
    {
        // If best_keysize doesn't exactly divide the encrypted input, add an extra bit for each bit that fits into the remainder
        size_t extra_bit = i < extra_bits ? 1 : 0;
        bit_blocks[i] = malloc(num_full_blocks + extra_bit);
    }

    for (size_t i = 0; i < num_full_blocks; i++)
    {
        for (size_t bit = 0; bit < best_keysize; bit++)
        {
            bit_blocks[bit][i] = encrypted_input[i * best_keysize + bit];
        }
    }
    for (size_t i = 0; i < extra_bits; i++)
    {
        bit_blocks[i][num_full_blocks + 1] = encrypted_input[num_full_blocks * best_keysize + i];
    }

    // One key for each bit in the keysized block
    char *key_guesses = malloc(best_keysize);
    float *best_scores = malloc(best_keysize);

    // Test every bit block against every possible one byte key, and save the key that 
    // produces the most plausible english output
    for (size_t i = 0; i < best_keysize; i++)
    {
        size_t extra_bit = i < extra_bits ? 1 : 0;
        for (size_t key = 0; key < 256; key++)
        {
            char xor_bits[num_full_blocks + extra_bit];
            for (size_t bit = 0; bit < num_full_blocks + extra_bit; bit++)
            {
                xor_bits[bit] = bit_blocks[i][bit] ^ key;
            }
            float score = get_english_score(xor_bits, num_full_blocks + extra_bit);
            if (score > best_scores[i])
            {
                best_scores[i] = score;
                key_guesses[i] = key;
            }
        }
    }

    for (size_t i = 0; i < encrypted_input_size; i++)
    {
        if ()
        {
            key_guesses
            printf("%c", key_guesses[i % best_keysize] ^ encrypted_input[i]);
        
    }

    // Cleanup
    for (size_t i = 0; i < best_keysize; i++)
    {
        free(bit_blocks[i]);
    }
    free(bit_blocks);
    free(encrypted_input);
    free(buf);

    return 0;
}