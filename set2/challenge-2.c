#include "../common/encodings.h"
#include "../common/cbc.h"

int main(int argc, char **argv)
{
    init_base64_module();
    size_t decoded_len = 0;
    int ret = 1;
    char *decoded = base64_decode_file("test.txt", &decoded_len);
    
    if (decoded == NULL)
    {
        // Fail
    }
    
    return ret;
}