#include "cbc.h"
#include "padding.h"

char *encrypt(char *input, size_t len, char *key, char *iv, size_t block_size)
{
    size_t padded_len;
    char *padded_input = pkcs7_pad(input, len, block_size, &padded_len);
    free(input);
    char *cipher = malloc(padded_len);
    
    size_t num_blocks = padded_len / block_size;
    char *prev_cipher = iv;
    for (size_t block = 0; block < num_blocks; block++)
    {        
        for (size_t byte = 0; byte < block_size; byte++)
        {
            cipher[block * block_size + byte] = input[block * block_size + byte] ^ prev_cipher[byte] ^ key[byte];
        }
        prev_cipher = cipher[block * block_size];
    }
    free(padded_input);
}

char *decrypt(char *input, size_t len, char *key, char *iv, size_t block_size)
{

}