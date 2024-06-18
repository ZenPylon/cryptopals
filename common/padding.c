#include "padding.h"
#include <string.h>

char * pkcs7_pad(char *input, size_t input_len, size_t block_size)
{
    size_t padding_len = block_size - input_len % block_size;
    char *output = malloc(input_len + padding_len);
    memcpy(output, input, input_len);
    if (output == NULL)
    {
        return NULL;
    }
    for (size_t byte = input_len; byte < input_len + padding_len; byte++)
    {
        output[byte] = padding_len;
    }
    
    return output;
}