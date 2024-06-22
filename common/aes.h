#include <stdlib.h>

// Caller is responsible for freeing the returned output
// iv should have length of block_size
unsigned char *cbc_encrypt(unsigned char *input, size_t len, unsigned char *key, unsigned char *iv, size_t block_size);
unsigned char *cbc_decrypt(unsigned char *cipher, size_t len, unsigned char *key, unsigned char *iv, size_t block_size);

unsigned char *aes_128(unsigned char *input, unsigned char *key, size_t keysize);

unsigned char *sub_bytes();
unsigned char *shift_rows();
unsigned char *mix_columns();
unsigned char *add_round_key();
unsigned char *key_expansion();