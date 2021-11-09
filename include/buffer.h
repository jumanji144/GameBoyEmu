#ifndef BUFFER_H
#define BUFFER_H
#include "include.h"
#include <algorithm>

void expand_buffer(int* array, u32 new_size) {
    int* new_array = (int*)malloc(new_size);
    std::copy(array,array + sizeof(array)/sizeof(array[0]), new_array);
    delete[] array;
    array = new_array;
}


#endif
