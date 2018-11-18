# Memanggil kode Go dari Julia (JVM)
#
# Ditulis untuk Julia v0.6+
# https://github.com/JuliaLang/julia
#
# Run:
#   $ julia invoke.jl 


# Beberapa tipe di Go dapat dipetakan secara langsung sebagai tipe primitif di C++, 
# sebagian lagi dapat dianggap seagai tipe majemuk / struktur.

# Umumnya ketika membuat DLL dengan Go, compiler akan membuat sebuah file header
# untuk memudahkan interfacing. Namun dalam kasus tertentu kita hanya mendapatkan file
# DLL tanpa adanya header. Untuk melakukan interfacing, kita perlu mendefinisikan sendiri
# fungsi-fungsi yang diekspor oleh DLL.

##### Definisikan tipe #####
struct GoSlice
    arr::Ptr{Void}
    len::Int64
    cap::Int64
end
GoSlice(a::Vector, cap=10) = GoSlice(pointer(a), length(a), cap)

struct GoStr
    p::Ptr{Cchar}
    len::Int64
end
GoStr(s::String) = GoStr(pointer(s), length(s))


##### Load library #####
const called = "called.dll"


##### Deskripsikan signature setiap fungsi #####
Add(x,y)   = ccall((:Add, called), Int,(Int,Int), x,y)
Cosine(x)  = ccall((:Cosine, called), Float64, (Float64,), x)
function Sort(vals)
    ccall((:Sort, called), Void, (GoSlice,), GoSlice(vals))
    return vals     # agar sesuai standard
end
Hello(msg) = ccall((:Log, called), Int, (GoStr,), GoStr(msg))

##### Pemanggilan fungsi #####
for ex in [:(Add(93, 42)), :(Cosine(1)), :(Sort([39, 15, 9, 135, 27, 34]))]
    println("$ex = $(eval(ex))")
end
Hello("Xathrya")
