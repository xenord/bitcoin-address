#include <stdio.h>
#include <stdlib.h>
#include <secp256k1.h>
#include "../hex_utils.h"

// Compile with: gcc ../hex_utils.c test.c -lsecp256k1 -o test.o

int main(int argc, char const *argv[])
{
    
    unsigned char num1[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    unsigned char num2[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10};
    unsigned char result[32];
    subtract(num2, num1, result, 32);

    print_byte_array(result, 32);

    unsigned long long decimal_value = byte_array_to_long_long(result, 32);

    printf("Hex substraction: %llu\n", decimal_value);
    printf("num1: %llu\n", byte_array_to_long_long(num1, 32));
    printf("num2: %llu\n", byte_array_to_long_long(num2, 32));

    secp256k1_pubkey pubkey;
    secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);

    int return_val = secp256k1_ec_pubkey_create(ctx, &pubkey, num1);


    unsigned char compressed_pubkey[33];
    size_t len = sizeof(compressed_pubkey);
    return_val = secp256k1_ec_pubkey_serialize(ctx, compressed_pubkey, &len, &pubkey, SECP256K1_EC_COMPRESSED);

    char hex_pubkey[67];
    snprintf(hex_pubkey, sizeof(hex_pubkey), "03%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
             compressed_pubkey[1], compressed_pubkey[2], compressed_pubkey[3], compressed_pubkey[4], compressed_pubkey[5],
             compressed_pubkey[6], compressed_pubkey[7], compressed_pubkey[8], compressed_pubkey[9], compressed_pubkey[10],
             compressed_pubkey[11], compressed_pubkey[12], compressed_pubkey[13], compressed_pubkey[14], compressed_pubkey[15],
             compressed_pubkey[16], compressed_pubkey[17], compressed_pubkey[18], compressed_pubkey[19], compressed_pubkey[20],
             compressed_pubkey[21], compressed_pubkey[22], compressed_pubkey[23], compressed_pubkey[24], compressed_pubkey[25],
             compressed_pubkey[26], compressed_pubkey[27], compressed_pubkey[28], compressed_pubkey[29], compressed_pubkey[30],
             compressed_pubkey[31], compressed_pubkey[32]);

    for (int i = 0; i < 32; i++) {
        printf("%02x", num1[i]);
    }
    printf("\n");


    secp256k1_context_destroy(ctx);
    
    printf("%s\n", hex_pubkey);

    return 0;
}
