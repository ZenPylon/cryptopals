#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../common/padding.h"

int main(int argc, char **argv)
{
    int ret = 1;
    char input1[16] = "YELLOW SUBMARINE";
    char *input1_padded = pkcs7_pad(input1, 16, 20);
    if (input1_padded == NULL)
        goto error;

    char input2[16] = "YELLOW SUBMARINE";
    char *input2_padded = pkcs7_pad(input2, 16, 10);
    if (input2_padded == NULL)
        goto error;

    char input3[6] = "YELLOW";
    char *input3_padded = pkcs7_pad(input3, 6, 18);
    if (input3_padded == NULL)
        goto error;

    // Input 1
    printf("Expected result: ");
    for (size_t i = 0; i < 16; i++)
    {
        printf("%c", input1[i]);
    }
    printf("%s\n", "\x04\x04\x04\x04\0");

    printf("Actual result: ");
    for (size_t i = 0; i < 20; i++)
    {
        printf("%c", input1_padded[i]);
    }
    printf("\n");

    // Input 2
    printf("Expected result: ");
    for (size_t i = 0; i < 16; i++)
    {
        printf("%c", input2[i]);
    }
    printf("%s\n", "\x04\x04\x04\x04\0");

    printf("Actual result: ");
    for (size_t i = 0; i < 20; i++)
    {
        printf("%c", input2_padded[i]);
    }
    printf("\n");

    // Input 3
    printf("Expected result: ");
    for (size_t i = 0; i < 6; i++)
    {
        printf("%c", input3[i]);
    }
    printf("%s\n", "\xc\xc\xc\xc\xc\xc\xc\xc\xc\xc\xc\xc\0");

    printf("Actual result: ");
    for (size_t i = 0; i < 18; i++)
    {
        printf("%c", input3_padded[i]);
    }
    printf("\n");

    ret = 0;

error:
    if (errno)
        printf("Failed to pad string: %s", strerror(errno));

    free(input1_padded);
    free(input2_padded);
    free(input3_padded);
    return ret;
}
