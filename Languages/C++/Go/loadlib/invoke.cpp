/**
Memanggil kode Go dari C++

Diuji pada:
    - Go 1.11.2 windows/amd64 + MinGW GCC 7.3.0

Compile:
    (GCC)
    % g++ invoke.cpp -o invoke 
*/

#include <iostream>
#include <windows.h>
#include "go-types.h"

/*
Beberapa tipe di Go dapat dipetakan secara langsung sebagai tipe primitif di C++, 
sebagian lagi dapat dianggap seagai tipe majemuk / struktur.

Umumnya ketika membuat DLL dengan Go, compiler akan membuat sebuah file header
untuk memudahkan interfacing. Namun dalam kasus tertentu kita hanya mendapatkan file
DLL tanpa adanya header. Untuk melakukan interfacing, kita perlu mendefinisikan sendiri
fungsi-fungsi yang diekspor oleh DLL.

File "go-types.h" merupakan versi strip dari setiap header yang dibentuk oleh compiler Go.
File ini berisi tipe-tipe primitif yang ada di Go sehingga interfacing dapat dilakukan.

*/

/** Daftar fungsi yang diekspor oleh DLL **/
typedef GoInt     (__stdcall* AddFunc)   (GoInt p0, GoInt p1);
typedef GoFloat64 (__stdcall* CosineFunc)(GoFloat64 p0);
typedef void      (__stdcall* SortFunc)  (GoSlice p0);
typedef void      (__stdcall* HelloFunc) (GoString p0);


int main()
{
    int result;

    // Memuat modul DLL ================================================
    // Sebelum mendapatkan alamat dari fungsi terlebih dahulu kita harus memuat pustaka
	// ke memory dan memetakannya ke area yang masih tersedia.
	// Kita dapat menyerahkan tanggung jawab ini ke API
	auto called = LoadLibrary("called.dll");
	if (called == nullptr) 
	{
		MessageBox(nullptr, "Cannot locate the DLL file", "Error", MB_OK);
		return 1;
	}
	
	// Cari fungsi berdasarkan nama ====================================
	AddFunc     Add    = (AddFunc)     GetProcAddress(called, "Add");
    CosineFunc  Cosine = (CosineFunc)  GetProcAddress(called, "Cosine");
    SortFunc    Sort   = (SortFunc)    GetProcAddress(called, "Sort");
    HelloFunc   Hello  = (HelloFunc)   GetProcAddress(called, "Hello");

	// Panggil fungsi ==================================================
	
    /*
    Example 1: fungsi yang membutuhkan 2 integer dan 
        mengembalikan sebuah integer
    */
    GoInt a = 93;
    GoInt b = 42;
    std::cout << "Add(a, b) is: " << Add(a, b) << std::endl;

    /*
    Example 2: fungsi yang membutuhkan 1 float dan mengembalikan float
    */
    std::cout << "Cosine(1.0) is: " << Cosine(1.0) << std::endl;

    /*
    Example 3: fungsi yang membutuhkan slice array
    */
    GoInt data[6] = { 39, 15, 9, 135, 27, 34 };
    GoSlice nums = { data, 6, 6 };
    Sort(nums);
    for (int i = 0; i < 6; i++)
        std::cout << "data[" << i << "] = " << data[i] << std::endl;

    /*
    Example 4: fungsi yang membutuhkan String
    */
    GoString msg = { "Xathrya", 7 };
    Hello(msg);

	// Bebaskan area memory jika modul DLL tak terpakai lagi.	
	FreeLibrary(called);
}