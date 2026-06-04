# 🏥 Sistem Pencatatan Posyandu (C++ & PostgreSQL)

Project ini adalah sistem manajemen data kesehatan Posyandu yang mengimplementasikan algoritma **Insertion Sort** dan **Binary Search** secara manual untuk tujuan akademik.

## 🛠️ Prasyarat 
1. **CMake** (v3.16+)
2. **Compiler C++** (Visual Studio Build Tools 2022 dengan workload C++ Desktop, atau MinGW)
3. **PostgreSQL** (v14+)
4. **vcpkg** (Package manager untuk C++)

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
