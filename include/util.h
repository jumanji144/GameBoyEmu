#include "include.h"
#include <string>

template <typename I> const char* n2hexstr(I w, bool prefix, size_t hex_len = sizeof(I)<<1) {
    static const char* digits = "0123456789ABCDEF";
    std::string rc(hex_len,'0');
    for (size_t i=0, j=(hex_len-1)*4 ; i<hex_len; ++i,j-=4)
        rc[i] = digits[(w>>j) & 0x0f];
    if(prefix)
        return ("0x" + rc).c_str();
    else
        return rc.c_str();
}