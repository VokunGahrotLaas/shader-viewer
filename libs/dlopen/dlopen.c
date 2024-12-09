#define _GNU_SOURCE
#include <dlfcn.h>
#include <link.h>

// Override dlopen() function and inject RTLD_NODELETE so the library
// doesn't get deleted on close().
// This helps with asan traces with <unknown module>
void* dlopen(const char* filename, int flags){
    typedef void* (*dlopen_t)(const char*, int);
    dlopen_t original_dlopen = (dlopen_t)dlsym(RTLD_NEXT, "dlopen");
    flags |= RTLD_NODELETE;
    return original_dlopen(filename, flags);
}
