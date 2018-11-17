/**
Memanggil kode Python dari C++

Diuji pada:
    - Python 3.7.1 (x64) dan Ms. Visual Studio 2017 Community Ed.
    - Python 3.7.1 (x64) dan TDM GCC
    - Python 3.7.1 (x64) dan LLVM Clang.

Semua percobaan dilakukan dengan skema "load script file".
Lihat juga file "schema.cpp" di dalam direktori "schemas" untuk ragam schema lainnya.

Flag:
    Include:
        - $PYTHON/include

    Lib:
        - $PYTHON/libs/

Compile:
    (GCC)
    $ g++ invoke.cpp -o invoke -I$PYTHON/include -L$PYTHON/libs -lpython37 -std=c++11

    (clang)
    $ clang++ invoke.cpp -o invoke.exe -I$PYTHON/include -L$PYTHON/libs -lpython37

    (MSVC)
    $ cl invoke.cpp /I $PYTHON/include $PYTHON/libs/python36.lib 

Run:
    $ invoke

*/
#include <iostream>
#include <Python.h>

//========================== Static & Globals ==========================
wchar_t * program;

//========================== Helper  Function ==========================

void vm_init(const char* argv0);
void vm_cleanup();

void call_return_int(PyObject* module, const char* funcname);
void call_list_args(PyObject* module, const char* funcname);
void call_multi_args(PyObject* module, const char* funcname);
void call_multi_ret(PyObject* module, const char* funcname);
void instantiate_class(PyObject* module, const char* classname);

//======================================================================

int main(int argc, char* argv[])
{
    PyObject *name, *module;

    //===== Persiapan dan loading Python VM =====
    vm_init(argv[0]);

    //===== Load file =====
    name = PyUnicode_DecodeFSDefault("called");     // Convert filename sehingga sesuai dengan Python string
    module = PyImport_Import(name);                 // Import file sebagai Python module
    Py_DECREF(name);

    if (module == nullptr)
    {
        std::cerr << "Cannot find module" << std::endl;
        PyErr_Print();
        exit(1);
    }
    
    /* 
    Example 1: panggil method yang mengembalikan integer
    Variation: fungsi merupakan sebuah objek yang disimpan di dalam dictionary.
    */
    std::cout << "Example 1: Function that return integer" << std::endl;
    call_return_int(module, "random_magic");
    std::cout << std::endl;
    
    /* 
    Example 2: panggil method yang membutuhkan array of string sebagai argumen.
    */
    std::cout << "Example 2: Function that requre array of string" << std::endl;
    call_list_args(module, "hello");
    std::cout << std::endl;
    
        
    /*
    Example 3: panggil method yang membutuhkan beberapa argumen
    */
    std::cout << "Example 3: Function that require multiple argument" << std::endl;
    call_multi_args(module, "add");
    std::cout << std::endl;
    
    /*
    Example 4: panggil method yang mengembalikan beberapa return value
    */
    std::cout << "Example 4: Function that return multiple values" << std::endl;
    call_multi_ret(module, "split_string");
    std::cout << std::endl;
    
    /*
    Example 5: Instantiasi class melalui constructor
    */
    std::cout << "Example 5: Instantiate a class" << std::endl;
    instantiate_class(module, "called");
    std::cout << std::endl;

    Py_DECREF(module);
        

    //===== Clean up Python VM =====
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
Sample pemanggilan untuk Example 1 (return integer)
    - Cast return value sebagai integer
*/
void call_return_int(PyObject* module, const char* funcname)
{
    PyObject *dict, *func, *args, *value;

    // Buat sebuah dictionary untuk content module.
    dict = PyModule_GetDict(module);

    // Dapatkan method dari dictionary dan periksa validitas objek.
    func = PyDict_GetItemString(dict, funcname);
    if (func && PyCallable_Check(func))
    {
        /*
        Panggil fungsi dan terima return value.
        Fungsi di python selalu membutuhkan:
            1. Pointer ke python function
            2. Pointer ke argument list (berupa tuple)
        Jika tidak ada argument untuk fungsi, berikan (pass) tuple kosong.
        */
        args = PyTuple_New(0);
        
        value = PyObject_CallObject(func, args);
        Py_DECREF(args);
        if (value == nullptr)
        {
            std::cerr << "Cannot call the function " << funcname << std::endl;
            PyErr_Print();
        }
        else 
        {
            std::cout << "Result of call: " << PyLong_AsLong(value) << std::endl;
            Py_DECREF(value);
        }
        Py_DECREF(func);
    }
    else 
    {
        std::cerr << "Cannot find function " << funcname << std::endl;
        if (PyErr_Occurred())
            PyErr_Print();
    }
    Py_DECREF(dict);
}

/*
Sample pemanggilan untuk Example 2 (list of string as argument)
Argument merupakan tuple sehingga list of string dapat dijadikan sebagai argument dengan.
    - Membuat tuple
    - Membuat dan mengembangkan list
    - Set list sebagai content tuple.
*/
void call_list_args(PyObject* module, const char* funcname)
{
    #define ELEMCOUNT 2     // Ukuran awal, dapat di-extend
    PyObject *func, *args, *list, *value;
    int i;

    /*
    Asumsikan module merupakan sebuah objek dengan setiap objek memiliki atribute.
    Dapatkan atribut dari objek dan kembalikan sebagai fungsi.
    Periksa validitas dari fungsi.
    */
    func = PyObject_GetAttrString(module, funcname);
    if (func && PyCallable_Check(func))
    {
        /*
        Panggil fungsi dan terima return value.
        Fungsi di python selalu membutuhkan:
            1. Pointer ke python function
            2. Pointer ke argument list (berupa tuple)
        Pastikan ukuran tuple berkorespondensi dengan jumlah argument.
        */
        args = PyTuple_New(1);

        // Buat list dan kembangkan.
        list = PyList_New(ELEMCOUNT);

        // Set item ke dalam list berdasarkan index.
        PyList_SetItem(list, 0, PyLong_FromLong(13510));
        PyList_SetItem(list, 1, PyUnicode_FromString("Xathrya"));

        // Append, tambahkan item baru dari belakang.
        for (i = 0; i < 10; i++)
            PyList_Append(list, PyLong_FromLong((long)i));

        // Item baru dapat ditambahkan dari posisi tertentu (index dimulai dari 0)
        PyList_Insert(list, 0, PyUnicode_FromString("ReversingID"));
        
        // Set list ke tuple
        PyTuple_SetItem(args, 0, list);
        
        PyObject_CallObject(func, args);
        Py_DECREF(args);
    }
    else 
    {
        std::cerr << "Cannot find function " << funcname << std::endl;
        if (PyErr_Occurred())
            PyErr_Print();
    }
    Py_DECREF(func);
}

/*
Sample pemanggilan untuk Example 3 (multiple arguments)
Argument merupakan tuple sehingga menambahkan element tuple berarti menambahkan argument.

Contoh di sample ini:
    - Membuat tuple
    - Set item di tuple
*/
void call_multi_args(PyObject* module, const char* funcname)
{
    #define ARGCOUNT 2
    PyObject *func, *args, *value;
    int i, tmp_arg;

    /*
    Asumsikan module merupakan sebuah objek dengan setiap objek memiliki atribute.
    Dapatkan atribut dari objek dan kembalikan sebagai fungsi.
    Periksa validitas dari fungsi.
    */
    func = PyObject_GetAttrString(module, funcname);
    if (func && PyCallable_Check(func))
    {
        /*
        Panggil fungsi dan terima return value.
        Fungsi di python selalu membutuhkan:
            1. Pointer ke python function
            2. Pointer ke argument list (berupa tuple)
        Pastikan ukuran tuple berkorespondensi dengan jumlah argument.
        */
        args = PyTuple_New(ARGCOUNT);

        for (i = 0; i < ARGCOUNT; i++)
        {
            tmp_arg = rand() - i + 10;
            std::cout << "   Args[" << i << "] is " << tmp_arg << std::endl;

            // Assign tuple dengan integer.
            value = PyLong_FromLong(tmp_arg);
            if (!value)
            {
                Py_DECREF(args);
                Py_DECREF(func);
                std::cerr << "Cannot convert arguments" << std::endl;
                return;
            }
            PyTuple_SetItem(args, i, value);
        }
        
        value = PyObject_CallObject(func, args);
        Py_DECREF(args);
        if (value != nullptr)
        {
            std::cout << "Result of call: " << PyLong_AsLong(value) << std::endl;
            Py_DECREF(value);
        }
        else 
        {
            std::cerr << "Cannot call the function " << funcname << std::endl;
            PyErr_Print();
        }
    }
    else 
    {
        std::cerr << "Cannot find function " << funcname << std::endl;
        if (PyErr_Occurred())
            PyErr_Print();
    }
    Py_DECREF(func);
}

/*
Sample pemanggilan untuk Example 4 (return multiple value)
Multiple return value merupakan sebuah objek tuple yang berisi objek-objek lainnya.

Contoh di sample ini:
    - Tentukan panjang tuple (banyaknya return value)
    - Cast setiap item ke tipe yang tepat. 
*/
void call_multi_ret(PyObject* module, const char* funcname)
{
    PyObject *func, *args, *value;
    int i;

    /*
    Asumsikan module merupakan sebuah objek dengan setiap objek memiliki atribute.
    Dapatkan atribut dari objek dan kembalikan sebagai fungsi.
    Periksa validitas dari fungsi.
    */
    func = PyObject_GetAttrString(module, funcname);
    if (func && PyCallable_Check(func))
    {
        /*
        Panggil fungsi dan terima return value.
        Fungsi di python selalu membutuhkan:
            1. Pointer ke python function
            2. Pointer ke argument list (berupa tuple)
        Pastikan ukuran tuple berkorespondensi dengan jumlah argument.
        */
        args = PyTuple_New(1);

        // Set argument untuk memanggil fungsi
        PyTuple_SetItem(args, 0, PyUnicode_FromString("Reversing.ID Community Indonesia"));

        // Panggil fungsi
        PyObject* list = PyObject_CallObject(func, args);
        Py_DECREF(args);
        if (list != nullptr)
        {
            const char * retval;

            // Dapatkan banyaknya return value.
            int length = PyList_Size(list);
            
            std::cout << "Returning " << length << " elements" << std::endl;

            for (i = 0; i < length; i++)
            {
                // Dapatkan setiap item
                value = PyList_GetItem(list, i);
                if (!PyUnicode_Check(value))
                {
                    Py_DECREF(list);
                    Py_DECREF(func);
                    std::cerr << "Return items must be string" << std::endl;
                    return;
                }

                // Cetak item. String dalam bentuk unicode sehingga perlu dilakukan decode
                PyObject* str  = PyUnicode_AsEncodedString(value, "utf-8", "~E~");
                retval = PyBytes_AsString(str); 
                std::cout << "   RetVal[" << i << "] is " << retval << std::endl;
                Py_DECREF(str);
            }
            Py_DECREF(list);
        }
        else 
        {
            std::cerr << "Cannot call the function " << funcname << std::endl;
            PyErr_Print();
        }        
    }
    else 
    {
        std::cerr << "Cannot find function " << funcname << std::endl;
        if (PyErr_Occurred())
            PyErr_Print();
    }
    Py_DECREF(func);
}

/*
Sample pemanggilan untuk Example 5 (instantiate class)

Contoh ini berisi:
    - Load the reference to class
    - Load the constructor (or __init__)
    - Call the constructor and supply necessary parameters
*/
void instantiate_class(PyObject* module, const char* classname)
{
    PyObject *klass, *args, *instance, *method;

    // Dapatkan referensi ke class
    klass = PyObject_GetAttrString(module, classname);

    // Instansiasi
    // Kelas ini tidak memiliki parameter sehingga kita dapat berikan NULL ke konstruktor.
    args = Py_BuildValue("()");
    instance = PyObject_CallObject(klass, args);
    Py_DECREF(args);
    
    // Panggil method dari instance
    // Fungsi ini tidak memiliki parameter sehingga kita dapat berikan NULL sebagai parameter.
    method = PyObject_GetAttrString(instance, "showId");
    args   = Py_BuildValue("()");
    PyEval_CallObject(method, args);
    
    Py_DECREF(klass);
    Py_DECREF(method);
    Py_DECREF(args);    
    Py_DECREF(instance);

    /* Alternative ================
    // Instansiasi class secara langsung tanpa parameter
    instance = PyObject_CallObject(klass, NULL);

    // Panggil method instance secara langsung tanpa parameter
    PyObject_CallMethod(instance, "showId", NULL);
    */
}