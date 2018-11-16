# Code Interop Reference

* Invoker: C++
* Called: C# (.Net)
* Technique: COM Interop

### Invocation Steps

- Initialize COM dengan CoInitialize()
- Dapatkan interface kelas sebagai pointer.
- Panggil method yang diperlukan
- Uninitialize dengan CoUninitialize()

### Build Example

- Compile `called`
- Compile `invoker`
- Register DLL di COM environments
- Jalankan invoker.

### Alternative Schema

None