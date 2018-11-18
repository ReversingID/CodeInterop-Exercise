# Memanggil kode Go dari Python
# 
# Invoke the exported functions
#
# Reference: https://docs.python.org/3/library/ctypes.html
#
# Run:
#    $ python invoke.py

from ctypes import *
import os

# Beberapa tipe di Go dapat dipetakan secara langsung sebagai tipe primitif di C++, 
# sebagian lagi dapat dianggap seagai tipe majemuk / struktur.

# Umumnya ketika membuat DLL dengan Go, compiler akan membuat sebuah file header
# untuk memudahkan interfacing. Namun dalam kasus tertentu kita hanya mendapatkan file
# DLL tanpa adanya header. Untuk melakukan interfacing, kita perlu mendefinisikan sendiri
# fungsi-fungsi yang diekspor oleh DLL.

##### Load library #####
if os.name == 'nt':
    # Windows
    called = cdll.LoadLibrary("./called.dll")
else:
    # Linux and other
    called = cdll.LoadLibrary("./called.so")

##### Definisikan tipe #####
class GoSlice(Structure):
    _fields_ = [ ("data", POINTER(c_void_p)), ("len", c_longlong), ("cap", c_longlong) ]

class GoString(Structure):
    _fields_ = [ ("p", c_char_p), ("n", c_longlong) ]


##### Deskripsikan signature setiap fungsi #####
called.Add.argtypes = [ c_longlong, c_longlong ]
called.Add.restype  = c_longlong

called.Cosine.argtypes = [ c_double ]
called.Cosine.restype  = c_double

called.Sort.argtypes = [ GoSlice ]
called.Sort.restype  = None 

called.Hello.argtypes = [ GoString ]
called.Hello.restype  = None


##### Pemanggilan fungsi #####

# Example 1:
# fungsi yang membutuhkan 2 integer dan mengembalikan sebuah integer
a = 93
b = 42
print("Add(a, b) is: {}".format(called.Add(a, b)))

# Example 2:
# fungsi yang membutuhkan 1 float dan mengembalikan float
print("Cosine(1.0) is: {}".format(called.Cosine(1.0)))

# Example 3:
# fungsi yang membutuhkan slice array
nums = GoSlice((c_void_p * 6)(39, 15, 9, 135, 27, 34), 6, 6)
called.Sort(nums)
for i in range(nums.len):
    print("data[{}] = {}".format(i, nums.data[i]))

# Example 4: 
# fungsi yang membutuhkan String
msg = GoString(b"Xathrya", 7)
called.Hello(msg)