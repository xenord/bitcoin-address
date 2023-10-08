#include "hex_utils.h"

void print_byte_array(char *arr, int len)
{
    printf("[");
    for(int x = 0; x < len; x++) {
        printf("%u", arr[x]);
        if (x < len - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

/*
    increment_value can be from 0x00 to 0xFF, 0 to 255
*/
void increment(unsigned char arr[], int size, unsigned char increment_value)
{
    int carry = increment_value;
    for (int i = size - 1; i >= 0; i--) {
        int sum = arr[i] + carry;
        arr[i] = sum & 0xFF;
        carry = sum >> 8;
        if (carry == 0) {
            break;
        }
    }
}

void subtract(unsigned char *arr1, unsigned char *arr2, unsigned char *result, int size)
{
    int borrow = 0;
    for (int i = size - 1; 0 <= i; i--) {
        int diff = (int)arr1[i] - (int)arr2[i] + borrow;
        borrow = (diff >> 8);
        result[i] = (unsigned char)(diff & 0xff);
    }
}

unsigned long long byte_array_to_long_long(unsigned char *arr, int size)
{
    unsigned long long decimal_value = 0;

    for (int i = 0; i < size; i++) {
        decimal_value |= ((unsigned long long)(arr[size - 1 - i] & 0xFF)) << (i * 8); // invertito i bit per correggere l'endianess
    }

    return decimal_value;
}