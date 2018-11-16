# Code Interop Exercise

Simple self-exercises for learning code interoperability of various programming language for reverse engineering purpose.

### Selayang Pandang

Repository ini digunakan untuk menghimpun informasi dan pengetahuan tentang code interoperability beserta penggunaannya. Terdapat berbagai referensi tentang karakteristik platform serta teknik interoperation yang diguakan baik secara spesifik maupun generik.

### Apa itu Code Interop?

Secara harfiah `interop` atau `interoperability` adalah kemampuan sebuah sistem atau perangkat lunak untuk bertukar informasi dan memanfaatkannya. Code Interop dapat dimaknai sebagai suatu interoperability antar kode dengan platform berbeda, sehingga masing-masing dapat saling berkomunikasi dan memanfaatkan satu sama lain.

Interop dilakukan dengan mengekspos kode suatu platform kepada kode lain yang ditulis dalam bahasa maupun untuk platform berbeda. Hal ini menyebabkan pemanggilan fungsi dan komponen dapat dilakukan. Tujuan utamanya adalah untuk mempergunakan kode yang sudah ada sehingga tidak diperlukan usaha lebih untuk menulis ulang semua kode ke dalam platform lain.

Umumnya Code Interop terjadi antara `native code` dan `interpreted code`. Namun dalam hal ini kita melakukan generalisasi sehingga Code Interop dapat berarti penggunaan kode dari platform manapun oleh platform manapun.

### Code Interop dan Reverse Engineering

Dalam pengembangan software, software yang kompleks dipecah menjadi beberapa komponen atau modul. Tak jarang beberapa komponen telah dikembangkan, diimplementasikan, dan disempurnakan untuk platform tertentu atau menggunakan suatu bahasa pemrograman tertentu. Tentu akan sangat efisien apabila komponen tersebut digunakan ulang sehingga tidak diperlukan usaha untuk menulis semua komponen dari awal.

Ada pula kasus dimana sebuah `interpreted code` (Java, Python, dsb) dibungkus bersama dengan pustaka dan kode lain sebagai `native code`. Sebuah kode kecil (stub) akan dijalankan pertama kali dan menginisiasi environment yang tepat untuk dapat menjalankan interpreted code. Pendekatan ini disebut sebagai wrapping dan memberikan beberapa keuntungan, seperti:

* portability
* protection

Dalam sudut pandang Reverse Engineering, setidaknya Code Interop digunakan untuk dua hal:

1. analysis & identifikasi.
2. reengineering & rewriting.

### Struktur dan Konten

Repository ini memiliki beberapa direktori dengan pembagian tertentu yang membentuk struktur hirarki. Secara umum terdapat beberapa jenis direktori dalam repository ini, yaitu:

* Invoker
* Called
* Schema

Di bagian puncak (root) terdapat beberapa direktori yang masing-masing mewakili sebuah bahasa pemrograman / platform tertentu. Direktori ini disebut sebagai `invoker` atau kode yang memanggil kode lain. Di dalamnya terdapat informasi tentang karakteristik platform serta hal-hal lain yang perlu diperhatikan dalam kaitannya dengan Code Interop.

Di dalam `invoker` terdapat beberapa direktori yang disebut sebagai `called`. Serupa dengan `invoker`, called merupakan direktori yang masing-masing mewakili sebuah bahasa pemrograman / platform tertentu. Direktori `called` merujuk kepada kode yang dipanggil oleh kode lain, dalam hal ini dipanggil oleh `invoker`. 

Di setiap direktori `called` terdapat direktori yang merepresentasikan teknik code interop. Teknik-teknik ini dapat berupa teknik spesifik maupun teknik generik. Teknik spesifik merupakan teknik yang secara umum disediakan oleh platform, misal .Net dengan P/Invoke dan Java dengan JNI. Di dalamnya terdapat sampel penggunaan teknik lengkap dengan material yang dibutuhkan, seperti source code `invoker` dan `called`.

Di dalam direktori teknik ini juga terdapat sebuah direktori yang disebut sebagai `schema` atau `execution schema`. Direktori ini berisi alternatif cara pemanggilan kode berdasarkan teknik yang dibahas. Misal, kode yang dipanggil (called) dapat direpresentasikan sebagai file terpisah maupun embedded di dalam invoker, sehingga kedua hal tersebut memiliki cara pemanggilan berbeda meskipun teknik yang digunakan sama.

### Bagaimana Cara Berkontribusi?

Ini adalah projek terbuka. 

Kamu bisa memberikan sumbangan seperti kode dan praktik dalam Code Interop platform tertentu ataupun memberikan informasi-informasi terkait Code Interop yang belum dibahas sebelumnya. Tidak ada batasan dalam platform yang dibahas, baik bersifat open maupun closed.

Yang harus kamu lakukan: 

- melakukan pull request.
- mengirimkan email ke pengurus [at] reversing.id
- memberi tahu di telegram @ReversingID

Diharapkan agar segala referensi yang ada menggunakan Bahasa Indonesia sebagai sarana penyampaian informasi.