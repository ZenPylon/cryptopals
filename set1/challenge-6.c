#include <stdlib.h>
#include <stdio.h>
#include "encodings.h"

int main(int argc, char **argv)
{
    init_base64_module();
    char *input = "V2hlcmV2ZXIgSSBnbywgdGhlcmUgSSBhbQ==";
    char *output = base64_decode(input, 36);
    printf("%s", output);



    // Load file

    // Decode base64 into unencoded (but encrypted) bytes


    return 0;
}