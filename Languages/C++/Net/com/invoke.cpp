/**
Memanggil kode C# .Net dari C++ (native).

Diuji pada:
    - Ms. Visual Studio 2017 Community Ed.

Kode ini menggunakan directive yang unik diimplementasikan oleh MSVC.

Compile:
    (MSVC)
    $ cl invoke.cpp

Run:
    $ invoke
*/
#include <iostream>
#include <atlsafe.h>

/*
Import type library (TLB)
File ini dibangkitkan / generate oleh regasm.exe
*/

#import "CalledDll.tlb" raw_interfaces_only
using namespace CalledDll;

//========================== Static & Globals ==========================

//========================== Helper  Function ==========================

//======================================================================
int main()
{
    HRESULT result;
    long    retval;

    //===== Persiapan dan inisialisasi COM =====

    // Inisialisasi COM
    result = CoInitialize(NULL);

    // Buat interface pointer
    ICalledPtr icalc(__uuidof(Called));

    
    //===== Lakukan sesuatu dengan interface yang terbentuk =====
    try 
    {
    /*
    Example 1: method yang mengembalikan integer
        Pemanggilan fungsi dilakukan menggunakan interface.
    */
        std::cout << "Example 1: Function that return integer" << std::endl;

        // Pemanggilan secara langsung dengan return value sebagai argumen pertama.
        // Lihat interface.
        icalc->magic(&retval);
        std::cout << "The magic is " << retval << std::endl;
        std::cout << std::endl;

    /*
    Example 2: method yang membutuhkan array of string
        - Sebuah array merupakan objek dengan data jamak.
        - Setiap objek di .Net memiliki padanan di COM, dalam hal ini sebuah array
          di .Net memiliki padanan CComSafeArray di COM Interface
    */
        std::cout << "Example 2: Function that requre array of string" << std::endl;

        // Deklarasikan array dengan 5 elemen
        CComSafeArray<BSTR>  array(5);

        // Inisiasi setiap elemen di array.
        for (int i = 0; i < 5; i++)
            array.SetAt(i, SysAllocString(CT2W("Reversing.ID")), false);

        // Modifikasi elemen array di index ke-2
        array.SetAt(2, SysAllocString(CT2W("Xathrya")), false);

        // Pemanggilan fungsi dengan array sebagai argumen.
        icalc->hello(array);
        std::cout << std::endl;

    /*
    Example 3: method yang membutuhkan beberapa argument
    */
        std::cout << "Example 3: Function that require multiple argument" << std::endl;
        icalc->multi(135, 182);
        std::cout << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    // Uninitialize COM
    CoUninitialize();    

    return 0;
}
