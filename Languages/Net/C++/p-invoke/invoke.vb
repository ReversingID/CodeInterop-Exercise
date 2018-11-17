' Memanggil kode C++ dari .NET CLR
'
' Compile: 
'   $ vbc /out:invokeVB.exe invoke.vb
'
' Run:
'   $ invokeVB
imports System
imports System.Runtime.InteropServices

' P/Invoke
'   - Memanggil native code dari managed code.
'   - Prototipe fungsi yang akan dipanggil (native code) harus dideklarasikan terlebih dahulu.

' Reverse P/Invoke
'   - Memanggil managed code dari native code.
'   - Umumnya menggunakan callbackUsually.
'   - Sebuah delegate function harus dideklarasikan terlebih dahulu.
'   - Delegate function akan mem-forward pemanggilan ke fungsi spesifik.

public class Invoke 

    '===== Native declaration ====================================
    ' Kedua jenis deklarasi di bawah ini valid
    <DllImport("called.dll", EntryPoint:="print_clr", CallingConvention:=CallingConvention.StdCall)>
    private shared  sub print_clr(str as string)
    end sub

    public  declare sub dispatch lib "called.dll" (str as string, count as integer, [call] as Invoke.callback)

    '===== Delegate declaration ==================================
    ' Digunakan untuk Reverse P/Invoke.
    public delegate sub callback(str as string)

    '===== Callback declaration ==================================
    ' Fungsi ini akan dipanggil oleh delegate function.
    public shared sub worker(str as string)
        System.Console.WriteLine("Called VB: " & str)
    end sub


    '===== Main functions ========================================
    public shared sub Main()    
        ' P/Invoke
        ' Memanggil print_clr() di called.dll
        print_clr("P/Invoke")

        ' Reverse P/Invoke
        ' Memanggil dispatch() di called.dll yang kemudian akan memanggil callback()
        ' delegate akan meneruskan pemanggilan ke fungsi worker()
        dispatch("Reverse P/Invoke", 10, AddressOf Invoke.worker)
    end sub 'Main
end class   'Invoke