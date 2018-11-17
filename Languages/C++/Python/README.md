# Code Interop Reference

* Invoker: C++
* Called: Python
* Technique: Embedding

### Invocation Steps

- Initialize Python VM dengan Py_Initialize()
- Load module dengan PyImport_Import()
- Dapatkan method dari module (diimplementasi sebagai dictionary)
- Panggil method yang diperlukan.
- Uninitialize dengan Py_FinalizeEx()


### Build

- Build `called`
- Build `invoker`
- Jalankan invoker

### Alternative Schema

Alternatif skema dalam VM embedding:

- Menjalankan script dalam bentuk string.
- Menjalankan marshal code
- Menambahkan module pada runtime.