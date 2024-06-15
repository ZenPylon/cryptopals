#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

#include <openssl/evp.h>
#include <openssl/err.h>

#include "../common/encodings.h"

int main(int argc, char **argv)
{
    const char* filename = "challenge-7-no-newlines.txt";
    int ossl_ret = 1;

    FILE *challenge_file = fopen(filename, "r");
    if (challenge_file == NULL)
        goto error;

    struct stat file_stat;
    if (stat(filename, &file_stat) == -1)
        goto error;
    
    char *buf = malloc(file_stat.st_size);
    if (buf == NULL)
        goto error;

    if (!fread(buf, 1, file_stat.st_size, challenge_file))
        goto error;

    init_base64_module();
    char *encrypted_input = base64_decode(buf, file_stat.st_size);
    size_t encrypted_input_size = 3 * file_stat.st_size / 4;
    
    char key[16] = "YELLOW SUBMARINE";

    EVP_CIPHER* cipher = EVP_CIPHER_fetch(NULL, "AES-128-ECB", NULL);
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL || cipher == NULL)
        goto error;

    // The 0 value means decryption (vs. 1 for encryption)
    EVP_CipherInit_ex2(ctx, cipher, key, NULL, 0, NULL);

    // Make sure the key length and iv length match our expectation    
    OPENSSL_assert(EVP_CIPHER_CTX_get_key_length(ctx) == 16);
    unsigned char *outbuf = malloc(encrypted_input_size + 16 - (encrypted_input_size % 16));
    int outlen;

    if (!EVP_CipherUpdate(ctx, outbuf, &outlen, encrypted_input, encrypted_input_size))
        goto error;
    
    fwrite(outbuf, 1, outlen, stdout);
    int final_len;
    if (!EVP_CipherFinal_ex(ctx, outbuf + outlen, &final_len))
        goto error;


    fwrite(outbuf + outlen, 1, final_len, stdout);
    ossl_ret = 0;

error:
    // Print any errors if an error occured, and cleanup allocated resources
    if (errno != 0)
    {
        fprintf(stderr, "%s\n", strerror(errno));
    }
    if (ossl_ret != 0)
    {
        ERR_print_errors_fp(stderr);
    }

    EVP_CIPHER_free(cipher);
    EVP_CIPHER_CTX_free(ctx);
    free(buf);
    free(outbuf);
    return errno != 0 || ossl_ret != 0;
}