#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "../common/encodings.h"
#include "../common/bytes.h"

int main(int argc, char **argv)
{
    const char filename[] = "challenge-6.txt";
    
    // Load file
    struct stat file_stat;
    int ret = stat(filename, &file_stat);
    if (ret == -1)
    {
        perror("Could not stat file");
        exit(EXIT_FAILURE);
    }

    FILE *challenge_file = fopen(filename, "r");
    if (challenge_file == NULL)
    {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }
    
    char *buf = malloc(file_stat.st_size);
    if (buf == NULL)
    {
        perror("Could not allocate memory");
        exit(EXIT_FAILURE);
    }

    if (!fread(buf, 1, file_stat.st_size, challenge_file))
    {
        perror("Could not read file");
        exit(EXIT_FAILURE);
    }
    
    // Decode base64 into unencoded (but encrypted) bytes
    init_base64_module();
    char *encrypted_input = base64_decode(buf, file_stat.st_size);
    

    

    // TODO - Write better english score

    size_t keysize = 2;
    size_t max_keysize = 40;

    free (buf);

    return 0;
}