# Optimasi-Penggunaan-Malloc-Pada-Pengelolaan-Heap-Memori
## 1. Identifikasi Problem

Penggunaan `malloc()` yang tidak efisien dalam pengelolaan heap memori dapat menyebabkan beberapa masalah umum dalam program, seperti:
- **Fragmentasi Memori**: Terjadinya blok-blok memori yang tidak bersebelahan karena alokasi dan deallocation yang tidak teratur, mengakibatkan ketersediaan memori menurun meskipun masih ada banyak ruang bebas.
- **Memory Leaks**: Alokasi memori yang tidak dibebaskan dengan benar menyebabkan memori terbuang dan tidak dapat digunakan kembali oleh program, mengarah pada penggunaan memori yang tidak terkendali.

Optimasi penggunaan `malloc()` diperlukan untuk mengurangi fragmentasi memori, mencegah memory leaks, dan memastikan performa aplikasi yang lebih stabil dan efisien.

## 2. Deskripsi Problem

Dalam pengelolaan heap memori menggunakan `malloc()`, terdapat beberapa tantangan yang dihadapi:
1. **Fragmentasi Internal dan Eksternal**: Alokasi dan dealokasi memori yang tidak teratur dapat menyebabkan blok-blok memori yang dialokasikan tidak bersebelahan, sehingga memori yang tersedia terpecah menjadi potongan-potongan kecil (fragmentasi eksternal). Fragmentasi internal terjadi ketika blok yang dialokasikan lebih besar dari yang diperlukan.
2. **Memory Leaks**: Tanpa pelacakan yang baik terhadap blok-blok memori yang dialokasikan, ada risiko beberapa blok tidak dibebaskan, menyebabkan kebocoran memori.
3. **Performa Program**: Pengelolaan memori yang buruk dapat menyebabkan performa program menurun seiring waktu karena penurunan efisiensi penggunaan memori.

## 3. Metodologi Eksperimen

### Tujuan:
Eksperimen ini bertujuan untuk membandingkan performa dua versi program dalam mengelola heap memori menggunakan `malloc()` dan `free()`. Kami akan mengukur tingkat fragmentasi memori, memory leaks, dan performa secara keseluruhan.

### Langkah-langkah Eksperimen:
1. **Versi Program dengan Optimasi yang Buruk**:
   - Tidak ada strategi khusus untuk manajemen memori.
   - Alokasi dan dealokasi memori dilakukan secara acak tanpa pelacakan yang tepat, berpotensi menyebabkan fragmentasi memori dan memory leaks.

2. **Versi Program dengan Optimasi yang Lebih Baik**:
   - Menggunakan linked list untuk melacak blok-blok memori yang dialokasikan dan memastikan semua memori yang dialokasikan di-deallocate pada akhir program.
   - Mengimplementasikan strategi untuk mengelola blok memori long-lived untuk mengurangi fragmentasi memori dan meningkatkan efisiensi penggunaan memori.

3. **Pengukuran**:
   - Penggunaan memori maksimum (peak memory usage).
   - Total memori yang dialokasikan dan dibebaskan.
   - Jumlah fragmentasi memori.
   - Memory leaks (jumlah memori yang tidak dibebaskan).
   - Waktu eksekusi program.

## 4. Pelaksanaan Eksperimen

### Langkah-langkah:
1. **Implementasi Program**:
   - Dua versi program telah diimplementasikan: satu dengan optimasi memori yang buruk dan satu lagi dengan optimasi yang lebih baik.
   
2. **Simulasi Alokasi dan Dealokasi**:
   - Program melakukan alokasi dan dealokasi memori secara acak dalam beberapa iterasi.
   - Pada setiap iterasi, statistik penggunaan memori dicatat, termasuk penggunaan memori saat ini, memori puncak, dan jumlah fragmentasi.

3. **Variasi Waktu Hidup Blok Memori**:
   - Beberapa blok memori dialokasikan sebagai long-lived dan dipertahankan selama beberapa iterasi sebelum dibebaskan. Hal ini bertujuan untuk mensimulasikan objek yang digunakan lebih lama dalam program nyata.

4. **Pengukuran**:
   - Di akhir eksperimen, total memori yang dialokasikan dan dibebaskan dihitung untuk mengidentifikasi adanya memory leaks.
   - Total fragmentasi dan penggunaan memori puncak juga diukur untuk membandingkan performa kedua versi program.

### Hasil Eksperimen:

Data hasil eksperimen dapat diperoleh melalui penggunaan alat analisis seperti `Dr. Memory` untuk memeriksa fragmentasi dan memory leaks.

## 5. Analisis Hasil Eksperimen

### **Performa Program:**
- **Versi Optimasi yang Buruk** menunjukkan fragmentasi memori yang lebih tinggi dan rentan terhadap memory leaks karena tidak ada struktur yang melacak blok-blok memori yang dialokasikan. 
  - Total memori yang dialokasikan dan dibebaskan tidak seimbang, yang menunjukkan adanya memory leaks.
  - Penggunaan memori puncak lebih tinggi karena fragmentasi memori.
  - Waktu eksekusi lebih lama karena overhead yang disebabkan oleh fragmentasi.

- **Versi Optimasi yang Lebih Baik** menunjukkan pengelolaan memori yang lebih efisien. 
  - Semua blok memori yang dialokasikan dilacak dengan baik dan di-deallocate di akhir program, sehingga tidak ada memory leaks.
  - Fragmentasi memori berkurang karena penggunaan strategi linked list dan manajemen blok long-lived.
  - Penggunaan memori puncak lebih rendah, dan waktu eksekusi lebih konsisten.
