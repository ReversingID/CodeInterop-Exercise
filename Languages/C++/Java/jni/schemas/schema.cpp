/**
Memanggil kode Java dari C++

Diuji pada:
    - Oracle Java JDK 8 + Ms. Visual Studio 2017 Community Ed.
    - Oracle Java JDK 8 + MinGW GCC 7.3.0

Schemas:
    - Menjalankan file .class
    - Menjalankan file .class dari JAR/WAR/EAR
    - Menjalankan bytecode yang tersimpan di memory

Include:
    - $JDK/include
    - $JDK/include/win32

Lib:
    - $JDK/lib/jvm.lib

Compile:
    (GCC)
    $ g++ schema.cpp -o schema -I$JDK/include -I$JDK/include/win32 -L$JDK/lib -ljvm -std=c++11

    (MSVC)
    $ cl schema.cpp /I $JDK/include /I $JDK/include/win32 $JDK/lib/jvm.lib 

Run:
    $ schema

Catatan:
    - Jangan meletakkan jvm.dll di direktori yang sama dengan aplikasi.
    - Set PATH ke JRE dimana JVM.dll berada, misal:
        $JDK/jre/bin/server    , jika menggunakan JDK
        $JRE/bin/server        , jika menggunakan JRE

Future Works:
    - Penggunaan JVM.dll tanpa harus set PATH.
*/
#include <iostream>
#include <jni.h>
#include <jvmti.h>
#include <cstdlib>
#include <ctime>

#include <fstream>

//========================== Static & Globals ==========================
JavaVM *jvm;                 // Pointer ke JVM (Java Virtual Machine)
JNIEnv *env;                 // Pointer ke native interfacee

//========================== Helper  Function ==========================

/*
Sebelum menggunakan JNI di C++, load dan inisialisasi Java Virtual Machine (JVM)
terlebih dahulu. Kemudian load bytecode, yang akan dieksekusi oleh JVM.
*/
void create_vm();

//============================ Test  Module ============================

void execute_file(const char* filename, const char* funcname, const char* signature);
void execute_archive(const char* archive, const char* filename, const char* funcname, const char* signature);
void execute_bytecode(const char* bytecode, int length);

//=====================================================================

int main()
{
    // Bytecode string
    const char code[] = "\xca\xfe\xba\xbe\x00\x00\x00\x34\x00\x1c\x0a\x00\x06\x00\x0e\x09"
                        "\x00\x0f\x00\x10\x08\x00\x11\x0a\x00\x12\x00\x13\x07\x00\x14\x07"
                        "\x00\x15\x01\x00\x06\x3c\x69\x6e\x69\x74\x3e\x01\x00\x03\x28\x29"
                        "\x56\x01\x00\x04\x43\x6f\x64\x65\x01\x00\x0f\x4c\x69\x6e\x65\x4e"
                        "\x75\x6d\x62\x65\x72\x54\x61\x62\x6c\x65\x01\x00\x0c\x70\x72\x69"
                        "\x6e\x74\x5f\x73\x74\x72\x69\x6e\x67\x01\x00\x0a\x53\x6f\x75\x72"
                        "\x63\x65\x46\x69\x6c\x65\x01\x00\x0a\x6b\x6c\x61\x7a\x7a\x2e\x6a"
                        "\x61\x76\x61\x0c\x00\x07\x00\x08\x07\x00\x16\x0c\x00\x17\x00\x18"
                        "\x01\x00\x1f\x4d\x65\x74\x68\x6f\x64\x20\x66\x72\x6f\x6d\x20\x6d"
                        "\x65\x6d\x6f\x72\x79\x2d\x6c\x6f\x61\x64\x65\x64\x20\x63\x6c\x61"
                        "\x73\x73\x07\x00\x19\x0c\x00\x1a\x00\x1b\x01\x00\x05\x6b\x6c\x61"
                        "\x7a\x7a\x01\x00\x10\x6a\x61\x76\x61\x2f\x6c\x61\x6e\x67\x2f\x4f"
                        "\x62\x6a\x65\x63\x74\x01\x00\x10\x6a\x61\x76\x61\x2f\x6c\x61\x6e"
                        "\x67\x2f\x53\x79\x73\x74\x65\x6d\x01\x00\x03\x6f\x75\x74\x01\x00"
                        "\x15\x4c\x6a\x61\x76\x61\x2f\x69\x6f\x2f\x50\x72\x69\x6e\x74\x53"
                        "\x74\x72\x65\x61\x6d\x3b\x01\x00\x13\x6a\x61\x76\x61\x2f\x69\x6f"
                        "\x2f\x50\x72\x69\x6e\x74\x53\x74\x72\x65\x61\x6d\x01\x00\x07\x70"
                        "\x72\x69\x6e\x74\x6c\x6e\x01\x00\x15\x28\x4c\x6a\x61\x76\x61\x2f"
                        "\x6c\x61\x6e\x67\x2f\x53\x74\x72\x69\x6e\x67\x3b\x29\x56\x00\x21"
                        "\x00\x05\x00\x06\x00\x00\x00\x00\x00\x02\x00\x01\x00\x07\x00\x08"
                        "\x00\x01\x00\x09\x00\x00\x00\x1d\x00\x01\x00\x01\x00\x00\x00\x05"
                        "\x2a\xb7\x00\x01\xb1\x00\x00\x00\x01\x00\x0a\x00\x00\x00\x06\x00"
                        "\x01\x00\x00\x00\x0f\x00\x09\x00\x0b\x00\x08\x00\x01\x00\x09\x00"
                        "\x00\x00\x25\x00\x02\x00\x00\x00\x00\x00\x09\xb2\x00\x02\x12\x03"
                        "\xb6\x00\x04\xb1\x00\x00\x00\x01\x00\x0a\x00\x00\x00\x0a\x00\x02"
                        "\x00\x00\x00\x13\x00\x08\x00\x14\x00\x01\x00\x0c\x00\x00\x00\x02"
                        "\x00\x0d";


    srand(time(NULL));

    //===== Persiapan dan loading Java VM =====
    create_vm();

    //===== Execution schemas =====
    /*
    Example 1: Simpan bytecode Java sebagai .class dan eksekusi fungsi.
        Percobaan ini akan menghasilkan penjumlahan dua angka random.
    */
    std::cout << "Example 1: Execute function from a file" << std::endl;
    execute_file("file", "add", "(II)I");
    std::cout << std::endl;

    /*
    Example 2: Simpan file .class di dalam file JAR dan eksekusi fungsi.
        Percobaan ini akan menghasilkan penjumlahan dua angka random.
    */
    std::cout << "Example 2: Execute function from a file inside JAR" << std::endl;
    execute_archive("app.jar", "appjar", "add", "(II)I");
    std::cout << std::endl;

    /*
    Example 3: Store bytecode in application and execute it
        Percobaan ini akan mencetak sebuah string
    */
    std::cout << "Example 3: Execute stored bytecode" << std::endl;
    execute_bytecode(code, sizeof(code) - 1);   // Tanpa NULL termination.
    std::cout << std::endl;

    //===== Destroy JVM =====
    jvm->DestroyJavaVM();
    // Setelah JVM dihancurkan, operasi dengan Java tidak dapat dilakukan lagi.

    return 0;
}

/* Membuat dan menginisialisasi JVM untuk menjalankan kode Java. */
void create_vm()
{
    jint version;
    JavaVMInitArgs vm_args;                                 // argumen untuk inisialisasi   
    JavaVMOption  options[2];                               // Options untuk JVM, seperti ketika jalankan JVM di command line.

    // Set options untuk JVM
    options[0].optionString = "-Djava.class.path=.";        // Lokasi classpath (mencari .class)
    options[1].optionString = "-DXcheck:jni:pedantic";

    // Set arguments untuk JVM
    vm_args.version = JNI_VERSION_1_6;                      // Versi java minimum yang diinginkan
    vm_args.nOptions = 2;                                   // Banyaknya options yang diberikan
    vm_args.options = options;                         
    vm_args.ignoreUnrecognized = false;                     // Jika ada invalid options maka inisialisasi JVM akan gagal.

    //===== Load dan inisialisasi JVM serta JNI interface =====
    jint rc = JNI_CreateJavaVM(&jvm, (void**) &env, &vm_args);
    if (rc != JNI_OK || env == nullptr) 
    {   // Error processing...
        std::cout <<  "Unable to launch JVM: " << rc << std::endl;
        exit(1);
    }
    std::cout << "JVM load successfully" << std::endl;
    version = env->GetVersion();
    std::cout << "Version: " << ((version >> 16) & 0x0F) << "." << (version & 0x0F) << std::endl;
    std::cout << std::endl;
}


/* 
Fungsi untuk Example 1 (.class File)
    - Load dan import .class file
    - Dapatkan pointer ke fungsi add()
    - Buat tuple untuk menyimpan argumen
    - Panggil fungsi
*/
void execute_file(const char* filename, const char* funcname, const char* signature)
{
    jclass       cls;
    jmethodID    add;
    
    cls = env->FindClass(filename);     // If the class is in package, then use "package/class" notation
    add = env->GetStaticMethodID(cls, funcname, signature);
    if (add == nullptr)
        std::cerr << "method " << funcname << " cannot be found" << std::endl;
    else 
    {
        int args[2] = {rand() + 10, rand() + 9};

        std::cout << "   Args[0] is " << args[0] << std::endl;
        std::cout << "   Args[1] is " << args[1] << std::endl;

        jint res = env->CallStaticIntMethod(cls, add, args[0],args[1]);
        std::cout << "Result of call: " << res << std::endl;
    }
}

/*
Fungsi untuk Example 1 (.class File)
    - Tambahkan archive (file JAR) ke classpath
    - Load dan import file .class
    - Dapatkan pointer ke fungsi add()
    - Buat tuple untuk menyimpan argumen
    - Panggil fungsi
*/
void execute_archive(const char* archive, const char* filename, const char* funcname, const char* signature)
{
    jvmtiError   error;
    jvmtiEnv*    jvmti_env;

    // Get the JVM TI environment (not usual env)
    jint r = jvm->GetEnv((void **)&jvmti_env, JVMTI_VERSION);
    
    // Add the archive file (.jar) to class loader path
    std::cout << "Adding path \"" << archive << "\" to classpath" << std::endl;
    jvmti_env->AddToSystemClassLoaderSearch(archive);

    // The rest is similar to technnique (1)
    std::cout << "Executing " << filename << "." << funcname << "()" << std::endl;
    execute_file(filename, funcname, signature);
}


/*
Function for Example 3 (bytecode)
- Load the bytecode (if necessary)
- Use classloader to dynamically load classes
*/
void execute_bytecode(const char* bytecode, int length)
{
    jclass       klazz;
    jmethodID    print_string;

    // Create a ClassLoader for loading the class
    jclass loaderClass = env->FindClass("java/lang/ClassLoader"); 
    jmethodID loaderMethod = env->GetStaticMethodID(loaderClass, "getSystemClassLoader", "()Ljava/lang/ClassLoader;");
    jobject loader = env->CallStaticObjectMethod(loaderClass, loaderMethod);

    // Create a class from byte
    klazz = env->DefineClass("klazz", loader, (jbyte*) bytecode, (jint) length);
    if (klazz != nullptr)
    {
        print_string = env->GetStaticMethodID(klazz, "print_string", "()V");
        if (print_string == nullptr)
            std::cerr << "method \"print_string\" cannot be found" << std::endl;
        else 
            env->CallStaticVoidMethod(klazz, print_string);
    }
}

// Reference code
// Dumping classes to byte array using JVM TI
jbyteArray dump_class(JNIEnv* env, jclass cls, jobject method)
{
    jvmtiError      error;
    jvmtiEnv*       jvmti_env;
    jbyteArray      byte_array;
    jint            bytecode_size;
    unsigned char*  bytecodes;

    // Get the JVM TI environment (not usual env)
    jint r = jvm->GetEnv((void **)&jvmti_env, JVMTI_VERSION);

    jmethodID method_id = env->FromReflectedMethod(method);
    if (jvmti_env->GetBytecodes(method_id, &bytecode_size, &bytecodes))
        return NULL;
    
    byte_array = env->NewByteArray(bytecode_size);
    if (byte_array != nullptr)
        env->SetByteArrayRegion(byte_array, 0, bytecode_size, (jbyte*) bytecodes);
    
    jvmti_env->Deallocate(bytecodes);
    return byte_array;
}