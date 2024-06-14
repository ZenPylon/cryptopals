#include <string.h>
#include <stdio.h>
#include "../common/encodings.h"

char base64_encode_map[64] = {0};
char base64_decode_map[256] = {0};
float monogram_freq[256] = {0};

void init_base64_module()
{
    populate_base64_encode_map();
    populate_base64_decode_map();
    populate_monogram_freq();
}

void populate_monogram_freq()
{
    // Uppercase and lower case letters are considered identical 
    //(this likely isn't true, but probably reasonable approximation))
    
    // Uppercase
    monogram_freq['A'] = 8.55;
    monogram_freq['B'] = 1.60;
    monogram_freq['C'] = 3.16;
    monogram_freq['D'] = 3.87;
    monogram_freq['E'] = 2.10;
    monogram_freq['F'] = 2.18;
    monogram_freq['G'] = 2.09;
    monogram_freq['H'] = 4.96;
    monogram_freq['I'] = 7.33;
    monogram_freq['J'] = 0.22;
    monogram_freq['K'] = 0.81;
    monogram_freq['L'] = 4.21;
    monogram_freq['M'] = 2.53;
    monogram_freq['N'] = 7.17;
    monogram_freq['O'] = 7.47;
    monogram_freq['P'] = 2.07;
    monogram_freq['Q'] = 0.10;
    monogram_freq['R'] = 6.33;
    monogram_freq['S'] = 6.73;
    monogram_freq['T'] = 8.94;
    monogram_freq['U'] = 2.68;
    monogram_freq['V'] = 1.06;
    monogram_freq['W'] = 1.83;
    monogram_freq['X'] = 0.19;
    monogram_freq['Y'] = 1.72;
    monogram_freq['Z'] = 0.11;

    // Lowercase
    monogram_freq['a'] = 8.55;
    monogram_freq['b'] = 1.60;
    monogram_freq['c'] = 3.16;
    monogram_freq['d'] = 3.87;
    monogram_freq['e'] = 2.10;
    monogram_freq['f'] = 2.18;
    monogram_freq['g'] = 2.09;
    monogram_freq['h'] = 4.96;
    monogram_freq['i'] = 7.33;
    monogram_freq['j'] = 0.22;
    monogram_freq['k'] = 0.81;
    monogram_freq['l'] = 4.21;
    monogram_freq['m'] = 2.53;
    monogram_freq['n'] = 7.17;
    monogram_freq['o'] = 7.47;
    monogram_freq['p'] = 2.07;
    monogram_freq['q'] = 0.10;
    monogram_freq['r'] = 6.33;
    monogram_freq['s'] = 6.73;
    monogram_freq['t'] = 8.94;
    monogram_freq['u'] = 2.68;
    monogram_freq['v'] = 1.06;
    monogram_freq['w'] = 1.83;
    monogram_freq['x'] = 0.19;
    monogram_freq['y'] = 1.72;
    monogram_freq['z'] = 0.11;
}

void populate_base64_encode_map()
{
    // Upper case letters
    for (size_t i = 0; i < 26; i++)
    {
        // 65 for 'A'
        base64_encode_map[i] = (char)(65 + i);
    }
    for (size_t i = 0; i < 26; i++)
    {
        // 97 for 'a', with offset 26 for the upper case letters added in loop above
        base64_encode_map[26 + i] = (char)(97 + i);
    }
    for (size_t i = 0; i < 10; i++)
    {
        base64_encode_map[i + 52] = (char)(i + 48);
    }
    base64_encode_map[62] = '+';
    base64_encode_map[63] = '/';
}

void populate_base64_decode_map()
{
    char A = 'A';
    char a = 'a';
    char zero = '0';
    for (size_t i = 0; i < 26; i++)
    {
        base64_decode_map[A + i] = i;
    }
    for (size_t i = 0; i < 26; i++)
    {
        base64_decode_map[a + i] = 26 + i;
    }
    for (size_t i = 0; i < 10; i++)
    {
        base64_decode_map[zero + i] = 52 + i;
    }
    base64_decode_map['+'] = 62;
    base64_decode_map['/'] = 63;
    base64_decode_map['='] = 0;
}

char base64_decode_char(char input)
{
    // 'A' and '=' map to 0. Anything else is an encoding error
    if (base64_decode_map[input] == 0 && input != 'A' && input != '=' && input != '\n')
    {
        fprintf(stderr, "Invalid base64 char\n");
        exit(EXIT_FAILURE);
    }
    return base64_decode_map[input];
}

char hex_digit_to_int_val(char digit)
{
    // 0-9 digits
    if (digit >= 48 && digit <= 57)
    {
        return (digit - 48);
    }
    // Lowercase hex values
    else if (digit >= 97 && digit <= 102)
    {
        // Render 'a' as 10, etc.
        return (digit - 87);
    }
    else
    {
        fprintf(stderr, "Encountered invalid hex digit\n");
        return -1;
    }
}

char *parse_hex_string(const char *input)
{
    size_t input_num_chars = strlen(input);
    size_t input_num_bytes = input_num_chars / 2;
    char *output_buf = malloc(input_num_bytes);

    // Make sure our string-encoded hex input is valid
    if (input_num_bytes % 2 != 0 || input_num_chars % 2 != 0)
    {
        fprintf(stderr, "Invalid hex string in input");
        return NULL;
    }

    for (size_t i = 0; i < input_num_bytes; i++)
    {
        output_buf[i] = hex_digit_to_int_val(input[2 * i]) << 4 | hex_digit_to_int_val(input[2 * i + 1]);
    }
    return output_buf;
}

char *base64_encode(char *input, size_t input_num_bytes)
{
    // 8 bits of a base64 ASCII character correspond to 6 bits of input, which is a 4/3 ratio in bytes.
    // Then add any remaining bytes for padding (encoded as '=')
    size_t output_padding = input_num_bytes % 3;
    size_t output_size = 4 * (input_num_bytes / 3);

    // +1 for null byte
    char *output = calloc(output_size + output_padding + 1, 1);
    size_t input_bit_offset = 0;

    for (size_t i = 0; i < output_size; i++)
    {
        size_t left_byte = input_bit_offset / 8;
        size_t left_bit_offset = input_bit_offset % 8;
        size_t right_byte = (input_bit_offset + BITS_PER_BASE64_LETTER) / 8;
        size_t right_bit_offset = 8 - (input_bit_offset + BITS_PER_BASE64_LETTER) % 8;

        // For the left bits, extract from left_bit_offset to either 6 bits (using mask 0xFC to zero out the last two bits of the byte)
        // or until the end of the byte (whichever comes first)
        char left_bits = (0xFC >> left_bit_offset) & input[left_byte];

        // Select a quantity of "right_bit_offset" bits from the start of the right byte
        char right_bits = (0xFF << right_bit_offset) & input[right_byte];
        char bit_val;

        if (left_bit_offset == 0)
        {
            // Just grab bits 0 through 5, and shift over two to ensure it's a 6-bit value
            bit_val = left_bits >> 2;
        }
        else if (left_bit_offset == 6)
        {
            bit_val = (left_bits << 4) | (right_bits >> 4);
        }
        else if (left_bit_offset == 4)
        {
            bit_val = (left_bits << 2) | (right_bits >> 6);
        }
        else // left_bit_offset == 2
        {
            bit_val = left_bits;
        }
        output[i] = base64_encode_map[bit_val];
        input_bit_offset += BITS_PER_BASE64_LETTER;
    }
    // TODO -- add padding with '=' character(s)
    return output;
}

char *base64_decode(char *input, size_t num_chars)
{
    if (num_chars % 4 != 0)
    {
        fprintf(stderr, "Invalid base64 string");
        exit(EXIT_FAILURE);
    }

    char *output = calloc(3 * num_chars / 4, 1);

    for (size_t i = 0; i < num_chars; i++)
    {
        char decoded_byte = base64_decode_char(input[i]);
        // 6 bits per base64 char
        size_t left_byte = 6 * i / 8;
        size_t remainder = i % 4;

        if (remainder == 0)
        {
            output[left_byte] = decoded_byte << 2;
        }
        else if (remainder == 1)
        {
            output[left_byte] |= decoded_byte >> 4;
            output[left_byte + 1] |= decoded_byte << 4;
        }
        else if (remainder == 2)
        {
            output[left_byte] |= decoded_byte >> 2;
            output[left_byte + 1] |= decoded_byte << 6;
        }
        else
        {
            output[left_byte] |= decoded_byte;
        }
    }
    return output;
}

float get_english_score(char *string, size_t length)
{
    float score = 0.f;
    for (size_t i = 0; i < length; i++)
    {
        score += monogram_freq[string[i]];
    }
    return score;
}