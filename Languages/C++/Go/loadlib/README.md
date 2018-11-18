# Code Interop Reference

* Invoker: C++
* Called: Go
* Technique: load library (Win32 API)

### Invocation Steps

- Load library dengan LoadLibrary()
- Dapatkan alamat fungsi di library dengan GetProcAddress()
- Panggil method yang diperlukan.

### Build

- Build `called`
- Build `invoke`
- Jalankan invoker

### Alternative Schema

-
