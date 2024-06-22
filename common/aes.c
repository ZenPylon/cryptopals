#include "aes.h"
#include "padding.h"

unsigned char *cbc_encrypt(unsigned char *input, size_t len, unsigned char *key, unsigned char *iv, size_t block_size)
{
    size_t padded_len;
    unsigned char *padded_input = pkcs7_pad(input, len, block_size, &padded_len);
    unsigned char *cipher = malloc(padded_len);
    
    size_t num_blocks = padded_len / block_size;
    unsigned char *prev_cipher = iv;
    for (size_t block = 0; block < num_blocks; block++)
    {        
        for (size_t byte = 0; byte < block_size; byte++)
        {
            cipher[block * block_size + byte] = (input[block * block_size + byte] + prev_cipher[byte]) + key[byte];
        }
        prev_cipher = cipher + block * block_size;
    }
    free(padded_input);
    return cipher;
}

unsigned char *cbc_decrypt(unsigned char *cipher, size_t len, unsigned char *key, unsigned char *iv, size_t block_size)
{
    unsigned char *plain_text = malloc(len);
    size_t num_blocks = len / block_size;
    unsigned char *prev_cipher = iv;

    for (size_t block = 0; block < num_blocks; block++)
    {        
        for (size_t byte = 0; byte < block_size; byte++)
        {
            plain_text[block * block_size + byte] = cipher[block * block_size + byte] ^ prev_cipher[byte] ^ key[byte];
        }
        prev_cipher = cipher + block * block_size;
    }
    return plain_text;
}

// As specified in FIPS-197
// https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197-upd1.pdf
unsigned char *aes_128(unsigned char *input, unsigned char *key, size_t keysize)
{
    unsigned char *state = input;
    // 10 - 1 for 128-bit key
    size_t num_loop_rounds = 9;
    for (size_t i = 0; i < num_loop_rounds; i++)
    {
        sub_bytes();
        shift_rows();
        mix_columns();
        add_round_key();
    }
    sub_bytes();
    shift_rows();
    add_round_key();
    return state;
}

unsigned char *sub_bytes()
{

}
unsigned char *shift_rows()
{

}
unsigned char *mix_columns()
{

}
unsigned char *add_round_key()
{

}
unsigned char *key_expansion()
{

}