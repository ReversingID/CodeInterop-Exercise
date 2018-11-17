/**
Membuat DLL 

Compile:
    (GCC)
    $ g++ -shared called.cpp -o called.dll

    (clang)
    $ clang++ -shared called.cpp -o called.dll
    
    (MSVC)
    $ cl called.cpp /LD /Ox /GS /sdl
*/
#include <cstdio>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif 

//======== Type Definitions =========================================
// Callback yang akan dipanggil oleh module ini.
typedef void (*callback)(wchar_t* str);

// Fungsi yang akan dipanggil oleh managed code.
void __declspec(dllexport) 
print_clr(const char* str)
{
    printf("Call C++: %s\n", str);
}

void __declspec(dllexport) 
dispatch(wchar_t* input, int count, callback func)
{
    int i;
    // Panggil managed code secara tak langsung dengan memanggil delegate function
    for (i = 0; i < count; i++)
        func(input);
}

#ifdef __cplusplus
}
#endif 