#include <string.h>
#include <errno.h>
#include "../common/encodings.h"
#include "../common/aes.h"


int main(int argc, char **argv)
{
    init_base64_module();
    size_t decoded_len = 0;
    int ret = 1;
    unsigned char *decoded_input = base64_decode_file("challenge-2.txt", &decoded_len);
    
    if (decoded_input == NULL)
        goto error;
    
    unsigned char key[16] = "YELLOW SUBMARINE";
    unsigned char plain[16] = "Whatever, man!!!";
    unsigned char iv[16] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
    unsigned char *encrypted = cbc_encrypt(plain, 16, key, iv, 16);
    unsigned char* encoded = base64_encode(encrypted, 16);
    for (size_t i = 0; i < 22; i++) 
    {
        printf("%c", encoded[i]);
    }

    unsigned char *decrypted = cbc_decrypt(decoded_input, decoded_len, key, iv, 16);

    for (size_t i = 0; i < decoded_len; i++)
    {
        printf("%c", decrypted[i]);
    }

    ret = 0;
    
error:
    if (errno)
        fprintf(stderr, strerror(errno));

    // free(decoded);
    return ret;
}