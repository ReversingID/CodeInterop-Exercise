/**
Memanggil kode C++ dari .NET CLR

Compile:
    $ csc /out:invokeCS.exe invoke.cs

Run:
    $ invokeCS
*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

/*
P/Invoke
   - Memanggil native code dari managed code.
   - Prototipe fungsi yang akan dipanggil (native code) harus dideklarasikan terlebih dahulu.

Reverse P/Invoke
   - Memanggil managed code dari native code.
   - Umumnya menggunakan callbackUsually.
   - Sebuah delegate function harus dideklarasikan terlebih dahulu.
   - Delegate function akan mem-forward pemanggilan ke fungsi spesifik.
*/

namespace InteropInvoker
{
    class Invoke 
    {
        //===== Native declaration ====================================
        [DllImport("called.dll")]
        private static extern void print_clr(string str);

        [DllImport("called.dll")]
        public static extern void dispatch(string str, int count, callback call);
        
        //===== Delegate declaration ==================================
        // Digunakan untuk Reverse P/Invoke.
        public delegate void callback(string str);

        //===== Callback declaration ==================================
        // Fungsi ini akan dipanggil oleh delegate function.
        public static void worker(string str)
        {
            System.Console.WriteLine("Called C#: " + str);
        }

        //===== Main functions ========================================
        static void Main(string[] args)
        {
            // P/Invoke
            // Memanggil print_clr() di called.dll
            print_clr("P/Invoke");

            // Reverse P/Invoke
            // Memanggil dispatch() di called.dll yang kemudian akan memanggil callback()
            // delegate akan meneruskan pemanggilan ke fungsi worker()
            dispatch("Reverse P/Invoke", 10, new callback(Invoke.worker));
        }
    }
}