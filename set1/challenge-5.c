#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void *xor_encrypt(char *dest, const char *bytes, size_t num_bytes, const char *key)
{
    for (size_t i = 0; i < num_bytes; i++)
    {
        dest[i] = bytes[i] ^ key[i % 3];
    }
}
int main(int argc, char **argv)
{
    const char key[3] = "ICE";

    const char plaintext_1[] = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
    size_t num_bytes_1 = strlen(plaintext_1);
    char *output_1 = calloc(num_bytes_1 + 1, 1);
    xor_encrypt(output_1, plaintext_1, num_bytes_1, key);
    
    const char expected[] = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272\na282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";

    printf("Expected result:\n%s\n", expected);
    printf("Actual result:\n");
    for (size_t i = 0; i < num_bytes_1; i++)
    {
        printf("%02x", output_1[i]);
    }
    printf("\n");

    return 0;
}