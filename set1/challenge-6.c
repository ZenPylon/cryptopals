#include <stdlib.h>
#include <stdio.h>
#include "encodings.h"
#include "bytes.h"

int main(int argc, char **argv)
{
    char ltest[] = "this is a test";
    char rtest[] = "wokka wokka!!!";

    printf("Hamming distance: %d\n", hamming_distance(ltest, rtest, 14));

    init_base64_module();


    // Load file

    // Decode base64 into unencoded (but encrypted) bytes

    // TODO - Write better english score


    return 0;
}