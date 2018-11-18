/*
Memanggil Go dari Java.

Menggunakan JNA (Java Native Access) untuk memuat dan menjalankan kode native.
Ada beberapa alternative, lihat lebih banyak contohnya di repository `CodeInterop-Exercise`

Compile:
    $ javac -cp jna.jar invoke.java

Run: 
    (windows)
    $ java -cp "jna.jar;." invoke

    (linux)
    $ java -cp "jna.jar:." invoke

Pastikan JNA.jar berada dalam CLASSPATH atau dapat diakses oleh project.
*/

/*
Pada dasarnya, hanya JNI yang didukung sebagai satu-satunya cara untuk berinteraksi 
antara kode Java dan kode native.
Hal ini akan berbeda apabila proposal JEP 191 Foreign Function Interface benar-benar 
telah diimplementasikan dan dapat digunakan.

JNA merupakan library luar yang tidak disediakan secara default oleh Java (Oracle JDK 
maupun OpenJDK). JNA dapat diperoleh melalui

    https://github.com/java-native-access/jna

JNA 4.5.1 digunakan sebagai contoh.
*/

// Setidaknya ketiga modul ini harus tersedia.
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;
import com.sun.jna.Structure;
import com.sun.jna.Pointer;
import com.sun.jna.Memory;

import java.util.*;
import java.lang.Long;

/*
Beberapa tipe di Go dapat dipetakan secara langsung sebagai tipe primitif di C++, 
sebagian lagi dapat dianggap seagai tipe majemuk / struktur.

Umumnya ketika membuat DLL dengan Go, compiler akan membuat sebuah file header
untuk memudahkan interfacing. Namun dalam kasus tertentu kita hanya mendapatkan file
DLL tanpa adanya header. Untuk melakukan interfacing, kita perlu mendefinisikan sendiri
fungsi-fungsi yang diekspor oleh DLL.

*/

public class invoke
{
    // Alternatif ini merupakan cara standard dan stabil untuk memetakan 
    // fungsi native dan kode Java.
    public interface Called extends Library 
    {
        Called instance = (Called) Native.loadLibrary("Called.dll", Called.class);

        // Deklarasikan fungsi-fungsi yang dapat dipanggil (impor dari DLL)
        public long     Add(long a, long b);
        public double   Cosine(double val);
        public void     Sort(GoSlice.ByValue vals);
        public long     Hello(GoString.ByValue str);
        
        // GoSlice dipetakan sebagai:
        // C type struct { void *data; GoInt len; GoInt cap; }
        public class GoSlice extends Structure {
            public static class ByValue extends GoSlice implements Structure.ByValue {}
            public Pointer data;
            public long len;
            public long cap;
            protected List getFieldOrder(){
                return Arrays.asList(new String[]{"data","len","cap"});
            }
        }

        // GoString dipetakan sebagai
        // C type struct { const char *p; GoInt n; }
        public class GoString extends Structure {
            public static class ByValue extends GoString implements Structure.ByValue {}
            public String p;
            public long n;
            protected List getFieldOrder(){
                return Arrays.asList(new String[]{"p","n"});
            }
        }
    }


    public static void main(String[] args)
    {
        //  Example 1:
        //  fungsi yang membutuhkan 2 integer dan mengembalikan sebuah integer
        System.out.printf("Add(a, b) is: %s\n", Called.instance.Add(93, 42));

        //  Example 2:
        //  fungsi yang membutuhkan 1 float dan mengembalikan float
        System.out.printf("Cosine(1.0) is: %s\n", Called.instance.Cosine(1.0));

        //  Example 3:
        //  fungsi yang membutuhkan slice array
        long [] nums = new long[]{ 39, 15, 9, 135, 27, 34 };
        Memory arr   = new Memory(nums.length * Native.getNativeSize(Long.TYPE));
        arr.write(0, nums, 0, nums.length);
        
        Called.GoSlice.ByValue slice = new Called.GoSlice.ByValue();
        slice.data = arr;
        slice.len = nums.length;
        slice.cap = nums.length;

        Called.instance.Sort(slice);
        System.out.print("Sort([39, 15, 9, 135, 27, 34]) is: [");
        long[] sorted = slice.data.getLongArray(0, nums.length);
        for (int i = 0; i < sorted.length; i++)
            System.out.print(sorted[i] + ", ");
        System.out.println("]");

        //  Example 4: 
        //  fungsi yang membutuhkan String
        Called.GoString.ByValue msg = new Called.GoString.ByValue();
        msg.p = "Xathrya";
        msg.n = msg.p.length();
        Called.instance.Hello(msg);
    }
}