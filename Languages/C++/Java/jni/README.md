# Code Interop Reference

* Invoker: C++
* Called: Java
* Technique: JNI

### Invocation Steps

- Buat instance Java VM dengan JNI_CreateJavaVM()
- Load class dengan FindClass()
- Dapatkan `method signature` dengan GetStaticMethodID() atau fungsi lain sejenis.
- Panggil method yang diperlukan.
- Destroy instance JVM.

### Build Example

- Compile `called`
- Compile `invoker`
- Jalankan invoker.

### Alternative Schema

Alternatif skema dalam JNI:

- Menjalankan file .class
- Menjalankan file .class dari JAR/WAR/EAR
- Menjalankan bytecode yang tersimpan di memory