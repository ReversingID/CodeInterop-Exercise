# Code Interop Reference

* Invoker: Rust
* Called: C
* Technique: load library (Win32 API)

### Invocation Steps

- Import external crate "libloading"
- Definisikan type alias untuk setiap fungsi
- Dapatkan alamat fungsi di library simbol
- Panggil method yang diperlukan.

### Build

- Build `called`
- Build `invoker`
- Jalankan invoker

### Alternative Schema

-
