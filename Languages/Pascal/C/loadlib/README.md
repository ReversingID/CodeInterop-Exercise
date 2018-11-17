# Code Interop Reference

* Invoker: Pascal
* Called: C
* Technique: load library (Win32 API)

### Invocation Steps

- Load library dengan LoadLibrary()
- Dapatkan alamat fungsi di library dengan GetProcAddress()
- Panggil method yang diperlukan.

### Build

- Build `called`
- Build `invoker`
- Jalankan invoker

### Alternative Schema

-
