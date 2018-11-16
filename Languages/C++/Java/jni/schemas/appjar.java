/**
Memanggil kode Java dari C++
Eksekusi fungsi di dalam file JAR

File ini akan dimuat (load) oleh schema.exe dan method add() akan dipanggil.

Reproduce:
    - Compile file.java
        $ javac appjar.java
    - Buat file JAR dengan manifest
        $ jar cf app.jar appjar.class
    - Test
        $ java -cp app.jar appjar
    - Delete the appjar.class

Dalam pengujian, pastikan untuk menghapus appjar.class!
*/

/*
ref: http://journals.ecs.soton.ac.uk/java/tutorial/native1.1/implementing/method.html
Type Signature Table List
Signature    Java Type
    Z            boolean
    B            byte
    C            char
    S            short
    I            int
    J            long
    F            float
    D            double
    V            void
    L            fully-qualified-class ;    fully-qualified-class
    [ type       type[]
*/

// Periksa signature fungsi dengan `javap -s -p called`
public class appjar
{
    // Type signature: (II)I
    public static int    add(int a, int b)
    {
        return a + b;
    }

    // Untuk keperluan testing dan menjalankan aplikasi di luar schema.exe
    public static void main(String[] args)
    {
        System.out.println("Add 100 and 35 = " + add(100, 35));
    }
}