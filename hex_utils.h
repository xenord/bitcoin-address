#ifndef __HEX_UTILS_H__
#define __HEX_UTILS_H__

#include <stdio.h>
#include <stdint.h>

void print_byte_array(char *arr, int len);
unsigned long long byte_array_to_long_long(unsigned char * arr, int size);
void increment(unsigned char arr[], int size, unsigned char increment_value);
void subtract(unsigned char *arr1, unsigned char *arr2, unsigned char *result, int size);

#endif