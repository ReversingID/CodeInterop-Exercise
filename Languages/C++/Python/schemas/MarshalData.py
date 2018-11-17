# Memanggil kode Python dari C++
# Schema: eksekusi marshal data
#
# Konversi script ini menjadi binary, dan embed ke schema.cpp
#
# Convert:
#   $ python -m compileall MarshalData.py
#
# Read binary dan copy seluruh data (asumsikan hasilnya 64-bit)
#   with open("__pycache__/MarshalData.cpython-37.pyc", "rb") as f:
#       magic, timestamp = f.read(8), f.read(8)
#       print("\\x" + "\\x".join("{:02x}".format(x) for x in f.read()))
print("Reversing.ID Exercise on Code Interop (Invoke Python Code from C++)")

# Result (len=166):
# \xe3\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x40\x00\x00\x00\x73\x0c\x00\x00\x00\x65\x00\x64\x00\x83\x01\x01\x00\x64\x01\x53\x00\x29\x02\x7a\x43\x52\x65\x76\x65\x72\x73\x69\x6e\x67\x2e\x49\x44\x20\x45\x78\x65\x72\x63\x69\x73\x65\x20\x6f\x6e\x20\x43\x6f\x64\x65\x20\x49\x6e\x74\x65\x72\x6f\x70\x20\x28\x49\x6e\x76\x6f\x6b\x65\x20\x50\x79\x74\x68\x6f\x6e\x20\x43\x6f\x64\x65\x20\x66\x72\x6f\x6d\x20\x43\x2b\x2b\x29\x4e\x29\x01\xda\x05\x70\x72\x69\x6e\x74\xa9\x00\x72\x02\x00\x00\x00\x72\x02\x00\x00\x00\xfa\x0e\x4d\x61\x72\x73\x68\x61\x6c\x44\x61\x74\x61\x2e\x70\x79\xda\x08\x3c\x6d\x6f\x64\x75\x6c\x65\x3e\x0e\x00\x00\x00\x73\x00\x00\x00\x00

# Module ini bernama MarshalData (sesuai dengan nama file)
# Hal ini dapat diverifikasi berdasarkan hexdump.