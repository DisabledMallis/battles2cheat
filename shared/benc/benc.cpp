#include "benc.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define BIT_MASK (1<<1) | (1<<4) | (1<<7)

void benc::encode(char* data, size_t size, char** result, size_t* result_size) {
    char* new_data = (char*)malloc(size);
    if (!new_data) {
        return;
    }
    for(size_t i = 0; i < size; i++) {
        new_data[size-i-1] = (data[i] ^ (BIT_MASK)) + (i ^ BIT_MASK);
    }
    if(result) {
        *result = new_data;
    }
    if(result_size) {
        *result_size = size;
    }
};
void benc::decode(char* data, size_t size, char** result, size_t* result_size) {
    char* new_data = (char*)malloc(size);
    if (!new_data) {
        return;
    }
    for(size_t i = 0; i < size; i++) {
        new_data[size-i-1] = ((data[i]-((size-i-1) ^ BIT_MASK)) ^ (BIT_MASK));
    }
    if(result) {
        *result = new_data;
    }
    if(result_size) {
        *result_size = size;
    }
};

#ifdef TEST_BENC
int main() {
    //Test that the encode & decode works
    const char* data = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()-=_+";
    int len = strlen(data);
    char* enc_result;
    benc::encode((char*)data, len, &enc_result, 0);
    printf("%s\n", enc_result);
    char* dec_result;
    benc::decode(enc_result, len, &dec_result, 0);
    printf("%s\n", dec_result);
};
#endif