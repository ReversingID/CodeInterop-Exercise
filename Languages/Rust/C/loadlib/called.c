/**
Memanggil kode C dari Rust

Membuat shared library (dll / so)

Compile:
    (GCC) Windows
    $ gcc -shared called.c -o called.dll
    (GCC) Linux
    $ gcc -shared called.c -o called.so

    (MSVC)
    $ cl /nologo /LD /Ox /GS /sdl hello.c
*/
#include <stdio.h>

#ifdef _MSC_VER
#pragma comment(lib,"user32")
#endif

#ifdef __cplusplus
extern "C" {
#endif

//======== Type Definitions =========================================
// Callback yang akan dipanggil oleh module ini.
typedef void (*callback)(const char * str);

#if defined(_WIN32) || defined(WIN32)
#define API_TYPE __declspec(dllexport)
#else 
#define API_TYPE
#endif 

void API_TYPE
lib_print(const char *msg)
{
    printf("Call C: %s\n", msg);
}

void API_TYPE 
dispatch(const char * input, int count, callback func)
{
    int i;
    // Panggil managed code secara tak langsung dengan memanggil delegate function
    for (i = 0; i < count; i++)
        func(input);
}

#ifdef __cplusplus
}
#endif