/**
Memanggil kode C dari Pascal

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

#if defined(_WIN32) || defined(WIN32)
#define API_TYPE __declspec(dllexport)
#else 
#define API_TYPE
#endif 

void API_TYPE
lib_print(const char *msg)
{
    printf("Call C: %s", msg);
}

#ifdef __cplusplus
}
#endif