/**
Memanggil kode Java dari C++

Diuji pada:
    - Oracle Java JDK 8 + Ms. Visual Studio 2017 Community Ed.
    - Oracle Java JDK 8 + MinGW GCC 7.3.0

Semua percobaan dilakukan dengan schema "load .class file".
Lihat juga file "schema.cpp" di dalam direktori "schemas" untuk ragam schema lainnya.

Flag:
    Include:
        - $JDK/include
        - $JDK/include/win32

    Lib:
        - $JDK/lib/jvm.lib

Compile:
    (GCC)
    $ g++ invoke.cpp -o invoke -I$JDK/include -I$JDK/include/win32 -L$JDK/lib -ljvm -std=c++11

    (Clang)
    $ clang++ invoke.cpp -o invoke.exe -I$JDK/include -I$JDK/include/win32 -L$JDK/lib -ljvm 

    (MSVC)
    $ cl invoke.cpp /I $JDK/include /I $JDK/include/win32 $JDK/lib/jvm.lib 

Run:
    $ invoke

Catatan:
    - JVM Run-Time support terletak di shared library jvm.dll (windows) atau libjvm.so (unix).
    - Jangan meletakkan jvm.dll di direktori yang sama dengan aplikasi.
    - Set PATH ke JRE dimana JVM.dll berada, misal:
        $JDK/jre/bin/server    , jika menggunakan JDK
        $JRE/bin/server        , jika menggunakan JRE

Future Works:
    - Penggunaan JVM.dll tanpa harus set PATH.
*/
#include <iostream>
#include <jni.h>


//============================== Static & Globals ==============================


//============================== Helper  Function ==============================

/*
Sebelum menggunakan JNI di C++, load dan inisialisasi Java Virtual Machine (JVM)
terlebih dahulu. Kemudian load bytecode, yang akan dieksekusi oleh JVM.
*/
JNIEnv* create_vm(JavaVM** jvm);

//=============================================================================

int main()
{
    JavaVM       *jvm;                 // Pointer ke JVM (Java Virtual Machine)
    JNIEnv       *env;                 // Pointer ke native interface
    jclass       cls, javclass;
    jmethodID    magic, hello, multi, ctor, showid;

    //===== Persiapan dan loading Java VM =====
    env = create_vm(&jvm);

    //===== Lakukan sesuatu dengan JVM =====
    try 
    {
        // Jika class dalam sebuah package, gunakan path utuh sesuai notasi "package/class"
        cls = env->FindClass("called");

        /* 
        Example 1: method statis yang mengembalikan integer
            setiap method dikenali dengan type-signature. Pastikan type-signature cocok.
        */
        magic  = env->GetStaticMethodID(cls, "magic", "()I");

        /* 
        Jika tidak ditemukan, fungsi tidak bisa dipanggil sehingga tidak ada yang bisa kita
        lakukan
        */
        if (magic == nullptr)
            std::cerr << "ERROR: method 'magic' is not found!" << std::endl;
        /* Jika ditemukan, panggil fungsi tersebut dan cetak kembaliannya */
        else 
        {
            jint res = env->CallStaticIntMethod(cls, magic);
            std::cout << "Result is: " << res << std::endl;
        }
        
        /*
        Example 2: method statis yang membutuhkan array of string sebagai argumen.
            - tipe data argumen memengaruhi type-signature sebuah fungsi.
            - sebuah array merupakan objek dengan data jamak, sehingga perlu dibentuk
              secara khusus sebelum mengirimkannya ke fungsi.
        */
        hello  = env->GetStaticMethodID(cls, "hello", "([Ljava/lang/String;)V");
        if (hello == nullptr)
            std::cerr << "ERROR: method 'hello' is not found!" << std::endl;
        else
        { 
            jobjectArray names = env->NewObjectArray(5,     // buat array dengan 5 buah ...
				env->FindClass("java/lang/String"),         // Strings ...
				env->NewStringUTF("Reversing.ID"));         // masing-masing diinisialisasi dengan kalimat "Reversing.ID"
			env->SetObjectArrayElement(names, 2, env->NewStringUTF("Xathrya"));  // Ubah element di index ke-2
			env->CallStaticVoidMethod(cls, hello, names);   // panggil method dengan array
			env->DeleteLocalRef(names);                     // rilis objek dari memory.
        }
        
        /*
        Example 3: method statis yang membutuhkan beberapa argumen
        */
        multi  = env->GetStaticMethodID(cls, "multi", "(II)V");
        if (multi == nullptr)
            std::cerr << "ERROR: method 'multi' is not found!" << std::endl;
        else 
            env->CallStaticVoidMethod(cls, multi, 10, 30);
    
        /*
        Example 4: Instantiasi class melalui constructor
        */
        ctor = env->GetMethodID(cls, "<init>", "()V");
        if (ctor == nullptr)
            std::cerr << "ERROR: constructor is not found!" << std::endl;
        else 
        {
            /* gunakan NewObject() untuk memanggil constructor */
            jobject called = env->NewObject(cls, ctor); 
            if (called)
            {
        
        /*
        Example 5: method yang mengembalikan string.
            - Java menggunakan string bertipe UTF termodifikasi
        */
                showid = env->GetMethodID(cls, "showId", "()Ljava/lang/String;");
                if (showid == nullptr)
                    std::cerr << "ERROR: method 'showId' is not found!" << std::endl;
                else
                {
                    jstring s = (jstring) env->CallObjectMethod(called, showid);
                    const char * id = env->GetStringUTFChars(s, NULL);
                    std::cout << id <<std:: endl;
                    env->ReleaseStringUTFChars(s, id);
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    //===== Destroy JVM =====
    jvm->DestroyJavaVM();
    // Setelah JVM dihancurkan, operasi dengan Java tidak dapat dilakukan lagi.

    return 0;
}

/* Membuat dan menginisialisasi JVM untuk menjalankan kode Java. */
JNIEnv* create_vm(JavaVM** jvm)
{
    JNIEnv* env;
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
    jint rc = JNI_CreateJavaVM(jvm, (void**) &env, &vm_args);
    if (rc != JNI_OK || env == nullptr) 
    {   // Error processing...
        std::cout <<  "Unable to launch JVM: " << rc << std::endl;
        exit(1);
    }
    std::cout << "JVM load successfully" << std::endl;
    version = env->GetVersion();
    std::cout << "Version: " << ((version >> 16) & 0x0F) << "." << (version & 0x0F) << std::endl;
    std::cout << std::endl;

    return env;
}