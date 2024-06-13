#include "bytes.h"

size_t hamming_distance(char* lbytes, char* rbytes, size_t size)
{
    size_t distance = 0;
    for (size_t i = 0; i < size; i++)
    {
        char overlap = lbytes[i] ^ rbytes[i];
        
        // Count the number of bits that are set to 1
        for (size_t shift = 0; shift < 8; shift++)
        {
            distance += (overlap >> shift) & 0x01;
        }
    }
    return distance;

}