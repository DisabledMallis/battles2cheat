#ifndef BENC_BENC
#define BENC_BENC

#include <stdlib.h>

namespace benc {
    void encode(char* data, size_t size, char** result, size_t* result_size);
    void decode(char* data, size_t size, char** result, size_t* result_size);
};

#endif /* BENC_BENC */
