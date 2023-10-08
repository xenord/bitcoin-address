#include <stdio.h>
#include <stdlib.h>
#include <secp256k1.h>
#include <string.h>
#include <bloom.h>

#include "hex_utils.h"

/*
secp256k1_pubkey pubkey;
secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
secp256k1_context_destroy(ctx);

*/

/*
    Compile with  "gcc hex_utils.c main.c -lsecp256k1 -lbloom.2 -o main.o"
*/

int print_public_key(char *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%c", arr[i]);
    }
    printf("\n");
}

int create_pubkey(secp256k1_context* ctx, secp256k1_pubkey* pubkey, unsigned char *priv_key) {
    if (secp256k1_ec_pubkey_create(ctx, pubkey, priv_key) != 1) {
        printf("Error creating public key.\n");
        return 0;
    }
    return 1;
}

void format_public_key(unsigned char *pubkey_data, char* hex_pubkey, size_t key_size) 
{
    char prefix[3];
    if (pubkey_data[0] == 0x04) { // Inizia con il prefisso "04" per chiavi non compresse
        strncpy(prefix, "04", 3);
    } else if (pubkey_data[0] == 0x03 || pubkey_data[0] == 0x02) { // Chiave pubblica compressa
        snprintf(prefix, 3, "%02x", pubkey_data[0]);
    } else {
        printf("Prefisso non valido.\n");
        return;
    }

    // Copia il prefisso nel buffer di destinazione
    strncpy(hex_pubkey, prefix, 3);

    // Formatta i byte rimanenti come esadecimali
    for (size_t i = 1; i < 65; i++) {
        snprintf(hex_pubkey + (2 * i), key_size - 2 * i, "%02x", pubkey_data[i]);
    }
    hex_pubkey[2 * 65] = '\0';  // Aggiungi il terminatore di stringa
}

/*
    unsigned char hex_pub_key[67];
*/
int get_compressed_pubkey(secp256k1_context* ctx, secp256k1_pubkey pubkey, unsigned char *serialized_pubkey, size_t serialized_pubkey_size)
{
    if (secp256k1_ec_pubkey_serialize(ctx, serialized_pubkey, &serialized_pubkey_size, &pubkey, SECP256K1_EC_COMPRESSED) != 1) {
        printf("Error serializing the public key.\n");
        return 0;
    }
    return 1;
}

/*
    unsigned char hex_pub_key[131];
*/
int get_uncompressed_pubkey(secp256k1_context* ctx, secp256k1_pubkey pubkey, unsigned char *serialized_pubkey, size_t serialized_pubkey_size)
{
    if (secp256k1_ec_pubkey_serialize(ctx, serialized_pubkey, &serialized_pubkey_size, &pubkey, SECP256K1_EC_UNCOMPRESSED) != 1) {
        printf("Error serializing the public key.\n");
        return 0;
    }
    return 1;
}

void initialize_bloom(char *path, struct bloom *bloom) {
    FILE *stream = fopen(path, "r");
    int items = 0;

	char line[1024];
	while (fgets(line, 1024, stream)) {
		char *tmp = strdup(line);
        if (tmp != NULL) {
            bloom_add(bloom, tmp, strlen(tmp));
            items++;
            free(tmp);
        }
	}
	fclose(stream);
}

int get_file_lines(char *path) {
    FILE *stream = fopen(path, "r");
    int items = 0;

	char line[1024];
	while (fgets(line, 1024, stream)) {
        if (line != NULL) {
            items++;
        }
	}
	fclose(stream);
    return items;
}

int main(int argc, char const *argv[]) {
    unsigned char privkey[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    struct bloom bloom;
    
    int n_lines = get_file_lines("pub_keys.txt");
    if (bloom_init2(&bloom, n_lines, 0.000001) == 1) {
        printf("Bloom filter initialization failed\n");
        return 1;
    }
    initialize_bloom("pub_keys.txt", &bloom);


    for (int i = 0; i < 100; i++) {

        secp256k1_pubkey pubkey;
        secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);

        if (create_pubkey(ctx, &pubkey, privkey) == 0) {
            return 1;
        }
        unsigned char serialized_pubkey[65];
        unsigned char hex_c_pubkey[131];
        if (get_compressed_pubkey(ctx, pubkey, serialized_pubkey, sizeof(serialized_pubkey)) == 0) {
            return 1;
        }

        format_public_key(serialized_pubkey, hex_c_pubkey, 67);

        if (bloom_check(&bloom, hex_c_pubkey, 67) == 1) {
            printf("FOUND\n");
            printf("Compressed pubkey "); print_public_key(hex_c_pubkey, 67);
            printf("Privkey ");
            for (int x = 0; x < 32; x++) {
                printf("%02x ", privkey[x]);
            }
            printf("\n\n");
        }

        /*char hex_u_pubkey[131];
        memset(serialized_pubkey, 0x00, 65);
        if (get_uncompressed_pubkey(ctx, pubkey, serialized_pubkey, sizeof(serialized_pubkey)) == 0) {
            return 1;
        }
        format_public_key(serialized_pubkey, hex_u_pubkey, sizeof(hex_u_pubkey));*/
        /*printf("Uncompressed pubkey "); print_public_key(hex_u_pubkey, 131);*/
        
        
        secp256k1_context_destroy(ctx);
        increment(privkey, 32, 0x01);
    }


    return 0;
}