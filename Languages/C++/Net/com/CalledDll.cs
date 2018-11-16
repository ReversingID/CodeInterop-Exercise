/**
Memanggil kode C# .Net dari C++ (native).

Membuat "strong name"
    $ sn -k assembly-key.snk

Compile:
    $ csc /t:library /out:CalledDll.dll CalledDll.cs

Register (butuh administrator privilege):
    $ regasm CalledDll.dll /tlb:CalledDll.tlb /codebase

Lihat pula file CalledDll.tlh yang dihasilkan untuk definisi tipe-tipe yang ada.
*/
using System;
using System.Reflection;
using System.Runtime.InteropServices;

/*
Assembly info
Untuk meregistrasi COM interface, DLL harus memiliki strong name dan informasi ini.
*/

[assembly: AssemblyTitle("CalledDll")]
[assembly: AssemblyDescription("Reversing.ID - CodeInterop Exercise")]
[assembly: AssemblyConfiguration("")]
[assembly: AssemblyCompany("")]
[assembly: AssemblyProduct("CalledDll")]
[assembly: AssemblyCopyright("Copyright ©  2018")]
[assembly: AssemblyTrademark("")]
[assembly: AssemblyCulture("")]
[assembly: AssemblyVersion("1.0.0.0")]
[assembly: AssemblyFileVersion("1.0.0.0")]
[assembly: ComVisible(true)] 
[assembly: AssemblyDelaySign(false)] 
[assembly: AssemblyKeyFile("assembly-key.snk")]


/* 
COM Interop akan mengekspos kode dalam interface.
Segala hal yang berkaitan dengan instansiasi dan urusan internal akan 
ditangani oleh mekanisme COM Interop.
*/
namespace CalledDll
{
    // Interface declaration
    public interface ICalled
    {
        int  magic();
        void hello(string[] names);
        void multi(int x, int y);
    }

    // Interface implementation
    public class Called : ICalled
    {
        public Called()
        { }

        // Example 1: method yang mengembalikan integer
        public int magic()
        {
            return 135;
        }

        // Example 2: method yang membutuhkan array of string
        public void hello(string[] names)
        {
            System.Console.WriteLine("Hello, ");
            foreach (string name in names)
            {
                System.Console.WriteLine("  " + name);
            }
        }

        // Example 3: method yang membutuhkan beberapa argument
        public void multi(int x, int y)
        {
            System.Console.WriteLine("I was called with " + x + " and " + y);
        }
    }
}
