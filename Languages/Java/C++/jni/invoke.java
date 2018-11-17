/**
Memanggil kode C++ dari Java.

Compile:
    $ javac invoke.java

Create JNI Header:
    $ javah -jni invoke

Run:
    $ java invoke

*/
public class invoke 
{
    private native void print_jni();
    private        void print_java()
    {
        System.out.println("Calling Java from C++ (Lib)");
    }

    public static void main(String[] args)
    {
        // Instansiasi invoke kemudian panggil fungsi print_jni().
        new invoke().print_jni();
    }

    static 
    {
        // Load fungsi yang diimplementasikan sebagai shared library.
        System.loadLibrary("called");
    }
}