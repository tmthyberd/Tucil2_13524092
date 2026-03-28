# Boundary Mesh Voxelization with Octree

## Deskripsi Singkat

Program ini merupakan implementasi **boundary voxelization** pada mesh 3D berbentuk file `.obj` menggunakan pendekatan **divide and conquer** dengan struktur data **octree**.

Program akan:

1. Membaca file `.obj` yang berisi vertex dan face segitiga.
2. Membentuk *bounding cube* dari objek.
3. Melakukan subdivisi ruang secara rekursif menggunakan octree.
4. Mengecek apakah suatu cube beririsan dengan boundary mesh.
5. Menyimpan *leaf node* boundary pada depth maksimum sebagai voxel.
6. Mengekspor hasil voxelisasi ke file `.obj` baru dengan representasi kubus menggunakan **triangle faces**.

Program ini hanya melakukan **boundary voxelization**, sehingga **tidak melakukan pengecekan inside-outside solid**.

---

## Requirement

Program ini ditulis dalam **C++** dan membutuhkan compiler yang mendukung standar modern C++.

### Requirement Utama

- `g++` atau compiler C++ lain
- Standar minimal: **C++17**

### Library yang Digunakan

Program hanya menggunakan library standar C++, seperti:

- `iostream`
- `fstream`
- `sstream`
- `vector`
- `string`
- `limits`
- `algorithm`

Tidak diperlukan instalasi library eksternal tambahan.

---

## Struktur Program

Contoh struktur folder proyek:

```text
Tucil2_13524092/
├── bin/
├── output/
├── src/
│   ├── algorithms/
│   │   ├── overlap_checker.cpp
│   │   ├── overlap_checker.hpp
│   │   ├── voxel_subdivider.cpp
│   │   └── voxel_subdivider.hpp
│   ├── input/
│   │   ├── objLoader.cpp
│   │   └── objLoader.hpp
│   ├── output/
│   │   ├── objOutput.cpp
│   │   └── objOutput.hpp
│   ├── struct/
│   │   └── octree.hpp
│   └── main.cpp
├── test/
└── README.md
```

Keterangan:

- Folder `test/` berisi file input `.obj`
- Folder `output/` berisi hasil voxelisasi `.obj`
- Folder `bin/` berisi executable hasil kompilasi

---

## Cara Kompilasi

Lakukan kompilasi dari root folder proyek.

Contoh command:

```bash
g++ -std=c++17 -o bin/voxelizer src/main.cpp src/input/objLoader.cpp src/output/objOutput.cpp src/algorithms/overlap_checker.cpp src/algorithms/voxel_subdivider.cpp
```

Jika ingin menambahkan warning:

```bash
g++ -std=c++17 -Wall -Wextra -o bin/voxelizer src/main.cpp src/input/objLoader.cpp src/output/objOutput.cpp src/algorithms/overlap_checker.cpp src/algorithms/voxel_subdivider.cpp
```

---

## Cara Menjalankan Program

Jalankan executable dari folder `bin/`.

Contoh pada Linux/macOS:

```bash
cd bin
./voxelizer
```

Contoh pada Windows PowerShell:

```powershell
cd bin
.\voxelizer.exe
```

---

## Cara Menggunakan Program

Saat program dijalankan, pengguna akan diminta memasukkan:

1. **Nama file input `.obj`**  
   File harus berada di folder `test/`

2. **Max octree depth**  
   Menentukan seberapa detail voxelisasi dilakukan

3. **Nama file output `.obj`**  
   File hasil akan disimpan ke folder `output/`

Contoh input:

```text
Input OBJ filename (inside test folder): cow.obj
Max octree depth: 5
Output OBJ filename (inside output folder): cow_depth5.obj
```

Setelah selesai, program akan menampilkan **report** berisi:

```text
Voxel count    : 1234
Vertex count   : 9872
Face count     : 14808

Octree node per depth:
  0 : 1
  1 : 8
  2 : 64
  3 : 248
  4 : 1120
  5 : 4560

Pruned node per depth (EMPTY, tidak perlu ditelusuri):
  0 : 0
  1 : 2
  2 : 24
  3 : 112
  4 : 520
  5 : 3326

Octree depth   : 5
Execution time : 152.37 ms
Output path    : ../output/cow_depth5.obj
```

Keterangan report:

| Informasi | Deskripsi |
|---|---|
| Voxel count | Jumlah voxel boundary yang terbentuk pada depth maksimum |
| Vertex count | Jumlah vertex output (voxel × 8) |
| Face count | Jumlah triangle face output (voxel × 12) |
| Octree node per depth | Banyaknya node octree yang terbentuk di setiap level kedalaman |
| Pruned node per depth | Banyaknya node EMPTY yang tidak perlu ditelusuri lebih lanjut |
| Octree depth | Kedalaman maksimum octree |
| Execution time | Lama waktu proses voxelisasi (ms) |
| Output path | Path file output `.obj` |

---

## Penjelasan Singkat Cara Kerja Program

Program menggunakan algoritma **divide and conquer** dengan alur sebagai berikut:

1. Root cube dibentuk dari *bounding box* objek.
2. Root cube dianggap beririsan dengan mesh.
3. Cube dibagi menjadi 8 child menggunakan octree.
4. Setiap child diperiksa apakah beririsan dengan boundary mesh:
   - Jika **tidak beririsan**, child dihentikan.
   - Jika **beririsan** dan depth **belum maksimum**, child dibagi lagi.
   - Jika **beririsan** dan depth **sudah maksimum**, child menjadi voxel boundary final.
5. Semua voxel boundary kemudian ditulis ke file `.obj` sebagai kumpulan kubus.

---

## Catatan

- Program hanya mendukung **boundary voxelization**
- Output voxel menggunakan **triangle faces**, bukan quad
- Setiap voxel direpresentasikan sebagai **12 triangle faces**
- Program mengasumsikan bahwa face pada input `.obj` berbentuk **segitiga**
- Program **hanya menerima format face dengan vertex index saja**, contoh:
  ```text
  f 1 2 3        Didukung
  f 1/2/3 4/5/6  Tidak didukung (format v/vt/vn)
  f 1//3 4//6    Tidak didukung (format v//vn)
  ```
  File `.obj` yang mengandung texture coordinate (`vt`) atau normal index (`vn`) pada face tidak akan terbaca dengan benar.

---

## Author

- **Nama:** Timothy Bernard Soeharto
- **NIM:** 13524092