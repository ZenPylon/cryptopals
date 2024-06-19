#include "cbc.h"

char *encrypt(char *input, size_t len, char *key, char *iv, size_t block_size);
char *decrypt(char *input, size_t len, char *key, char *iv, size_t block_size);