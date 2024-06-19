#include <stdlib.h>

// Caller is responsible for freeing the returned output
// iv should have length of block_size
char *encrypt(char *input, size_t len, char *key, char *iv, size_t block_size);
char *decrypt(char *input, size_t len, char *key, char *iv, size_t block_size);