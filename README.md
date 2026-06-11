# 🏥 Sistem Pencatatan Posyandu (C++ & PostgreSQL)

Project ini adalah sistem manajemen data kesehatan Posyandu yang mengimplementasikan algoritma **Insertion Sort** dan **Binary Search** secara manual + **Expand** untuk mencari kata kunci duplikat.

### 🎯 7 Fitur Utama

| No | Fitur | Deskripsi |
|----|-------|-----------|
| 1 | **Cari Data** | Pencarian data berdasarkan NIK menggunakan algoritma **Binary Search** |
| 2 | **Tambah Data Baru** | Input data warga baru dengan validasi ketat (NIK 16 digit, format tanggal, rentang angka, dll) |
| 3 | **Edit Data** | Edit data dengan 3 sub-menu: Data Diri & Alamat, Pemeriksaan Fisik, Status & Catatan Medis |
| 4 | **Hapus Data** | Hapus data berdasarkan NIK dari memori dan database |
| 5 | **Tampilkan Semua Data** | Menampilkan ringkasan seluruh data warga yang tersimpan |
| 6 | **Cari Nama Warga** | Pencarian data berdasarkan nama lengkap (multiple match) | Insertion Sort + Binary Search + Expand |
| 7 | **Cari Tempat Lahir** | Pencarian data berdasarkan tempat lahir (multiple match) | Insertion Sort + Binary Search + Expand |

## 🛠️ Prasyarat 
1. **Git**: [Download di sini](https://git-scm.com/download/win)
2. **CMake**: [Download di sini](https://cmake.org/download/) *(centang "Add CMake to the system PATH saat install")*
3. **Visual Studio Build Tools 2022**: [Download di sini](https://visualstudio.microsoft.com/visual-cpp-build-tools/) *(Pilih workload: "Desktop development with C++")*
4. **PostgreSQL**: [Download di sini](https://www.postgresql.org/download/windows/) 


## 🚀 Cara Setup & Menjalankan

### 1. Install Dependency via vcpkg
Buka terminal (PowerShell/CMD) dan jalankan:
```bash
# Jika belum punya vcpkg, clone dulu:
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat

# Install library yang dibutuhkan:
C:\vcpkg\vcpkg install libpqxx:x64-windows
C:\vcpkg\vcpkg integrate install
