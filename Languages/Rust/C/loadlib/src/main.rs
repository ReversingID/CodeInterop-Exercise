/*
Memanggil kode C dari Rust

Compile:
    $ cargo build --release
Run: 
    $ invoker.exe
*/

extern crate libloading;

use libloading::{Library, Symbol};
use std::ffi::CString;
use std::ffi::CStr;
use std::os::raw::c_char;

// Type alias / typedef
type LibPrintFunc = unsafe fn(*const c_char);
type DispatchFunc = unsafe fn(*const c_char, i32, extern fn(*const c_char));

// Deklarasi callback ke fungsi dispatch
extern fn callback(s: *const c_char) {
    let c_str     : &CStr  = unsafe { CStr::from_ptr(s) };
    let str_slice : &str   = c_str.to_str().unwrap();
    let str_buf   : String = str_slice.to_owned();
    println!("Rust: {}", str_buf);
}

fn main() {
    let unmanaged = Library::new("called.dll").unwrap();

    unsafe {
        // Dapatkan address dan assign sebagai fungsi
        let lib_print : Symbol<LibPrintFunc> = unmanaged.get(b"lib_print").unwrap();
        let dispatch  : Symbol<DispatchFunc> = unmanaged.get(b"dispatch").unwrap();
        
        // String yang akan diberikan ke lib_print
        let arg = CString::new("Halo dunia!").expect("CString::new failed");

        // Memanggil fungsi static world()
        lib_print(arg.as_ptr());

        // Memanggil dispatch dan memberikan callback
        dispatch(arg.as_ptr(), 10, callback);
    }
}