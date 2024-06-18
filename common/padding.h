#include <stdlib.h>

// Applies PKCS#7 padding
// Allocates a new . Does not free the input string
char * pkcs7_pad(char *input, size_t input_len, size_t block_size);