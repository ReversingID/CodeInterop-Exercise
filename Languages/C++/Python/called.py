# Memanggil kode Python dari C++
#
# Diuji pada:
#     - Python 3.7.1 (x64) dan Ms. Visual Studio 2017 Community Ed.
#     - Python 3.7.1 (x64) dan TDM GCC
#     - Python 3.7.1 (x64) dan LLVM Clang.
# 

# Example 1: Method yang mengembalikan integer
def random_magic():
    return 135

# Example 2: Method yang memerlukan array of string
def hello(names):
    print("Hello, ")
    for name in names:
        print("  {}".format(name))

# Example 3: Method yang memerlukan beberapa argument
def add(a, b):
    return a+b
 
# Example 4: Method yang mengembalikan beberapa return values
def split_string(compound):
     return compound.split()

# Example 5: Instansiasi class melalui constructor
class called:
    magic = 135

    def __init__(self):
        print("Instantiate")
    
    def showId(self):
        print("My ID is {}".format(self.magic))