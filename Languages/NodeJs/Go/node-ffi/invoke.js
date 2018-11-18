/*
Memanggil kode Go dari NodeJs

Run:
   $ node invoke.js
*/

var ref = require("ref");
var ffi = require("ffi");
var Struct    = require("ref-struct");
var ArrayType = require("ref-array");

/*
 Beberapa tipe di Go dapat dipetakan secara langsung sebagai tipe primitif di C++, 
 sebagian lagi dapat dianggap seagai tipe majemuk / struktur.

 Umumnya ketika membuat DLL dengan Go, compiler akan membuat sebuah file header
 untuk memudahkan interfacing. Namun dalam kasus tertentu kita hanya mendapatkan file
 DLL tanpa adanya header. Untuk melakukan interfacing, kita perlu mendefinisikan sendiri
 fungsi-fungsi yang diekspor oleh DLL.
*/

// ===== Load library =====
var called = ffi.Library("called.dll", {
// ===== Deskripsikan signature setiap fungsi =====
    // Signature dalam bentuk [return, [args]]
    Add:     ["longlong", ["longlong", "longlong"]],
    Cosine:  ["double",   ["double"]],
    Sort:    ["void",     [GoSlice]],
    Hello:   ["longlong", [GoString]]
});


// ===== Definisikan tipe =====
var longlong = ref.types.longlong;
var LongArray = ArrayType(longlong);

var GoSlice = Struct({
    data: LongArray,
    len:  "longlong",
    cap:  "longlong"
});

var GoString = Struct({
    p: "string",
    n: "longlong"
});


// ===== Pemanggilan fungsi =====

//  Example 1:
//  fungsi yang membutuhkan 2 integer dan mengembalikan sebuah integer
console.log("Add(a, b) is: ", called.Add(a, b));

//  Example 2:
//  fungsi yang membutuhkan 1 float dan mengembalikan float
console.log("Cosine(1.0) is: ", called.Cosine(1.0));

//  Example 3:
//  fungsi yang membutuhkan slice array
nums = LongArray([39, 15, 9, 135, 27, 34]);
var slice = new GoSlice();
slice["data"] = nums;
slice["len"] = 6;
slice["cap"] = 6;
called.Sort(slice);
console.log("Sort([39, 15, 9, 135, 27, 34]) is: ", nums.toArray());

//  Example 4: 
//  fungsi yang membutuhkan String
msg = new GoString();
msg["p"] = "Xathrya";
msg["n"] = 7;
called.Hello(msg);