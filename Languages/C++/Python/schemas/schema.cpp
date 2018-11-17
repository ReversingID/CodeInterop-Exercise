/**
Memanggil kode Python dari C++

Diuji pada:
    - Python 3.7.1 (x64) dan Ms. Visual Studio 2017 Community Ed.
    - Python 3.7.1 (x64) dan TDM GCC
    - Python 3.7.1 (x64) dan LLVM Clang.

Schema:
    - Eksekusi string yang merepresentasikan script
    - Eksekusi file script
    - Eksekusi marshal code
    - Eksekusi embedded code

Include:
    - $PYTHON/include

Lib:
    - $PYTHON/libs/

Compile:
    (GCC)
    $ g++ schema.cpp -o schema -I$PYTHON/include -L$PYTHON/libs -lpython37 -std=c++11

    (clang)
    $ clang++ schema.cpp -o schema -I$PYTHON/include -L$PYTHON/libs -lpython37

    (MSVC)
    $ cl schema.cpp /I $PYTHON/include $PYTHON/libs/python37.lib 

Run:
    $ schema
*/
#include <iostream>
#include <Python.h>
#include <marshal.h>
#include <cstdlib>

//========================== Static & Globals ==========================
wchar_t * program;
static int numargs = 0;

//========================== Helper  Function ==========================

void vm_init(const char* argv0);
void vm_cleanup();

void execute_file(const char* filename, const char* funcname);
void execute_marshal(const char* code, int length);

static PyObject* PyInit_embedded(void);

//=====================================================================

int main(int argc, char* argv[])
{
    // Marshal code
    const char code[] = "\xe3\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x40\x00\x00\x00"
                        "\x73\x0c\x00\x00\x00\x65\x00\x64\x00\x83\x01\x01\x00\x64\x01\x53\x00\x29\x02\x7a\x43"
                        "\x52\x65\x76\x65\x72\x73\x69\x6e\x67\x2e\x49\x44\x20\x45\x78\x65\x72\x63\x69\x73\x65"
                        "\x20\x6f\x6e\x20\x43\x6f\x64\x65\x20\x49\x6e\x74\x65\x72\x6f\x70\x20\x28\x49\x6e\x76"
                        "\x6f\x6b\x65\x20\x50\x79\x74\x68\x6f\x6e\x20\x43\x6f\x64\x65\x20\x66\x72\x6f\x6d\x20"
                        "\x43\x2b\x2b\x29\x4e\x29\x01\xda\x05\x70\x72\x69\x6e\x74\xa9\x00\x72\x02\x00\x00\x00"
                        "\x72\x02\x00\x00\x00\xfa\x0e\x4d\x61\x72\x73\x68\x61\x6c\x44\x61\x74\x61\x2e\x70\x79"
                        "\xda\x08\x3c\x6d\x6f\x64\x75\x6c\x65\x3e\x0e\x00\x00\x00\x73\x00\x00\x00\x00";

    srand(time(NULL));
    
    /* 
    Dibutuhkan untuk Example 4
    Module tambahan harus dimasukkan sebelum memanggil Py_Initialize()
    */
    numargs = argc;
    PyImport_AppendInittab("embedded", &PyInit_embedded);

    //===== Persiapan dan loading Python VM =====
    vm_init(argv[0]);

    //===== Execution schema =====
    /*
    Example 1: Simpan script sebagai string dan eksekusi.
        - Eksekusi ini berbeda dengan fungsi eval().
        - Eksekusi script ini akan mencetak waktu saat ini
    */
    std::cout << "Example 1: Execute stored script" << std::endl;
    PyRun_SimpleString("from time import time, ctime"); 
    PyRun_SimpleString("print('Today is %s' % ctime(time()))");
    std::cout << std::endl;
    
    /*
    Example 2: Simpan kode sebagai script file dan eksekusi.
        - Eksekusi akan mencetak penjumlahan dua buah angka random.
    */
    std::cout << "Example 2: Execute function from a file" << std::endl;
    execute_file("file", "add");
    std::cout << std::endl;

    /*
    Example 3: Simpan kode sebagai marshalled binary dan eksekusi.
        - Eksekusi akan mencetak string.
    */
    std::cout << "Example 3: Execute stored marshal code" << std::endl;
    execute_marshal(code, sizeof(code));
    std::cout << std::endl;

    /*
    Example 4: Menambahkan module tambahan ke script.
        - Eksekusi ini akan melakukan pass dari C++ ke Python VM dan merujuke ke variable tersebut.
    */
    std::cout << "Example 4: Execute embedded code" << std::endl;
    PyRun_SimpleString("import embedded; print('Number of arguments %d' % embedded.numargs())");
    std::cout << std::endl;

    vm_cleanup();
    return 0;
}

/* Inisialisasi Python VM, persiapkan environment untuk menjalankan kode Python */
void vm_init(const char* argv0)
{
    program = Py_DecodeLocale(argv0, NULL);
    if (program == nullptr)
    {
        std::cerr << "Fatal error: cannot decode " << argv0 << std::endl;
        exit(1);
    }
    Py_SetProgramName(program);
    Py_Initialize();
}

/* Cleanup the VM */
void vm_cleanup()
{
    if (Py_FinalizeEx() < 0)
        exit(120);
    
    PyMem_RawFree(program);
}

/* 
Pemanggilan Example 2 (File)

Contoh ini berisi:
    - Load dan import file
    - Dapatkan attribute fungsi add() 
    - Buat tuple untuk menampung argument
    - Panggil function
*/
void execute_file(const char* filename, const char * funcname)
{
    #define ARGCOUNT 2

    PyObject *pyname, *pymodule, *pydict, *pyfunc, *pyargs, *pyvalue;
    int i, tmp_arg;

    // Convert filename sehingga sesuai dengan Python string
    pyname = PyUnicode_DecodeFSDefault(filename);

    // Import file sebagai Python module
    pymodule = PyImport_Import(pyname);
    Py_DECREF(pyname);      // 

    if (pymodule != nullptr)
    {
        // Dapatkan method dari dictionary dan periksa validitas objek.
        pyfunc = PyObject_GetAttrString(pymodule, funcname);

        if (pyfunc && PyCallable_Check(pyfunc))
        {
            // Buat tuple untuk menampung argument.
            pyargs = PyTuple_New(ARGCOUNT);

            for (i = 0; i < ARGCOUNT; i++)
            {
                tmp_arg = rand() - i + 10;
                std::cout << "   Args[" << i << "] is " << tmp_arg << std::endl;

                // Assign tuple dengan integer.
                pyvalue = PyLong_FromLong(tmp_arg);
                if (!pyvalue)
                {
                    Py_DECREF(pyargs);
                    Py_DECREF(pymodule);
                    std::cerr << "Cannot convert arguments" << std::endl;
                    return;
                }
                PyTuple_SetItem(pyargs, i, pyvalue);
            }
            pyvalue = PyObject_CallObject(pyfunc, pyargs);
            Py_DECREF(pyargs);
            if (pyvalue != nullptr)
            {
                std::cout << "Result of call: " << PyLong_AsLong(pyvalue) << std::endl;
                Py_DECREF(pyvalue);
            }
            else 
            {
                Py_DECREF(pyfunc);
                Py_DECREF(pymodule);
                std::cerr << "Call failed" << std::endl;
                PyErr_Print();
                return;
            }
        }
        else
        {
            std::cerr << "Cannot find function " << funcname << std::endl;
            if (PyErr_Occurred())
                PyErr_Print();
        }
        Py_XDECREF(pyfunc);
        Py_DECREF(pymodule);
    }
    else 
    {
        std::cerr << "Cannot find function " << funcname << std::endl;
        PyErr_Print();
    }
}

/* 
Pemanggilan Example 3 (Marshal)

Contoh ini berisi:
    - Load marshal code
    - Eksekusi code
*/
void execute_marshal(const char* code, int length)
{
    PyObject *pycode, *pymain;

    // Load string sebagai marshal code
    pycode = PyMarshal_ReadObjectFromString(code, length);

    // Eksekusi modul secara langsung dari kode yang telah dimuat.
    pymain = PyImport_ExecCodeModule("MarshalData", pycode);
}

/* 
Pemanggilan Example 4 

Contoh ini berisi:
    - Definisi fungsi sebagai plugin.
    - Registrasi fungsi ke dalam module.
    - Registrasi module ke Python VM.
*/
static PyObject* 
emb_numargs(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":numargs"))
        return NULL;
    return PyLong_FromLong(numargs);
}
static PyMethodDef emb_methods[] = {
    { "numargs", emb_numargs, METH_VARARGS, "Return the number of arguments received by the process" },
    { NULL, NULL, 0, NULL }
};
static PyModuleDef emb_module = {
    PyModuleDef_HEAD_INIT,
    "embedded", "Interop example (embedded)",
    -1, emb_methods,
    NULL, NULL, NULL, NULL 
};
static PyObject* 
PyInit_embedded(void)
{
    return PyModule_Create(&emb_module);
}