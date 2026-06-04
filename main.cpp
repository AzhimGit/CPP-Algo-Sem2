#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <chrono>
#include <iomanip>
#include <pqxx/pqxx>
#include <regex>
#include <cctype>
#include <functional>

// ==========================================
// SISTEM VALIDASI INPUT (Reusable & Generic)
// ==========================================

// Helper: Trim whitespace
std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    auto end = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

// Helper: Konversi string ke uppercase
std::string toUpper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

// ==========================================
// 1. VALIDASI STRING (Tidak Kosong)
// ==========================================
std::string inputString(const std::string& prompt, bool allowEmpty = false) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        input = trim(input);
        if (!input.empty() || allowEmpty) return input;
        std::cout << "[!] Input tidak boleh kosong. Silakan ulangi.\n";
    }
}

// ==========================================
// 2. VALIDASI NIK & NO KK (16 DIGIT ANGKA)
// ==========================================
std::string inputNIK(const std::string& prompt) {
    std::string input;
    std::regex nikRegex(R"(\d{16})");
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        input = trim(input);
        if (std::regex_match(input, nikRegex)) return input;
        std::cout << "[!] Harus tepat 16 digit angka (contoh: 3201012345670001)\n";
    }
}

// ==========================================
// 3. VALIDASI TANGGAL (YYYY-MM-DD)
// ==========================================
bool isValidDate(const std::string& dateStr) {
    std::regex dateRegex(R"(\d{4}-\d{2}-\d{2})");
    if (!std::regex_match(dateStr, dateRegex)) return false;
    
    int y, m, d;
    if (sscanf(dateStr.c_str(), "%d-%d-%d", &y, &m, &d) != 3) return false;
    
    if (y < 1900 || y > 2100) return false;
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > 31) return false;
    
    // Jumlah hari per bulan (termasuk leap year)
    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) daysInMonth[1] = 29;
    return d <= daysInMonth[m - 1];
}

std::string inputDate(const std::string& prompt, bool allowEmpty = false) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        input = trim(input);
        if (input.empty() && allowEmpty) return "";
        if (isValidDate(input)) return input;
        std::cout << "[!] Format tanggal tidak valid. Gunakan YYYY-MM-DD (contoh: 2009-08-15)\n";
    }
}

// ==========================================
// 4. VALIDASI ANGKA (INTEGER & DOUBLE)
// ==========================================
int inputInt(const std::string& prompt, int min = 0, int max = 999999) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        input = trim(input);
        try {
            size_t pos;
            int val = std::stoi(input, &pos);
            if (pos == input.length() && val >= min && val <= max) return val;
        } catch (...) {}
        std::cout << "[!] Masukkan bilangan bulat antara " << min << " - " << max << "\n";
    }
}

double inputDouble(const std::string& prompt, double min = 0.0, double max = 9999.9) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        input = trim(input);
        try {
            size_t pos;
            double val = std::stod(input, &pos);
            if (pos == input.length() && val >= min && val <= max) return val;
        } catch (...) {}
        std::cout << "[!] Masukkan angka valid antara " << min << " - " << max << "\n";
    }
}

// ==========================================
// 5. VALIDASI PILIHAN ENUM (Y/N, L/P, STATUS, DLL)
// ==========================================
char inputYN(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt << " (Y/N): ";
        std::getline(std::cin, input);
        input = trim(toUpper(input));
        if (input == "Y" || input == "N") return input[0];
        std::cout << "[!] Hanya Y (Ya) atau N (Tidak) yang diperbolehkan.\n";
    }
}

char inputGender(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt << " (L/P): ";
        std::getline(std::cin, input);
        input = trim(toUpper(input));
        if (input == "L" || input == "P") return input[0];
        std::cout << "[!] Hanya L (Laki-laki) atau P (Perempuan) yang diperbolehkan.\n";
    }
}

int inputKategori(const std::string& prompt) {
    while (true) {
        int k = inputInt(prompt, 1, 4);
        return k;
    }
}

std::string inputStatus(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt << " (normal/beresiko): ";
        std::getline(std::cin, input);
        input = trim(input);
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);
        if (input == "normal" || input == "beresiko") return input;
        std::cout << "[!] Hanya 'normal' atau 'beresiko' yang diperbolehkan.\n";
    }
}

// Pilihan imunisasi (1/2/3/4 atau - untuk tidak)
std::string inputImunisasi(const std::string& prompt, int maxDosis = 3) {
    std::string input;
    while (true) {
        std::cout << prompt << " (1-" << maxDosis << " atau - jika belum): ";
        std::getline(std::cin, input);
        input = trim(input);
        if (input == "-") return "-";
        try {
            int dosis = std::stoi(input);
            if (dosis >= 1 && dosis <= maxDosis) return std::to_string(dosis);
        } catch (...) {}
        std::cout << "[!] Masukkan 1-" << maxDosis << " atau - untuk tidak.\n";
    }
}

// Pilihan opsi terbatas (template)
std::string inputChoice(const std::string& prompt, const std::vector<std::string>& options) {
    std::string input;
    while (true) {
        std::cout << prompt << " (";
        for (size_t i = 0; i < options.size(); ++i) {
            std::cout << options[i];
            if (i < options.size() - 1) std::cout << "/";
        }
        std::cout << "): ";
        std::getline(std::cin, input);
        input = trim(input);
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);
        for (const auto& opt : options) {
            std::string optLower = opt;
            std::transform(optLower.begin(), optLower.end(), optLower.begin(), ::tolower);
            if (input == optLower) return input;
        }
        std::cout << "[!] Pilihan tidak valid.\n";
    }
}

std::string inputRTRW(const std::string& prompt, const std::string& fieldName) {
    std::string input;
    while (true) {
        std::cout << prompt << " (1-3 digit angka, contoh: 01, 5, 012): ";
        std::getline(std::cin, input);
        input = trim(input);
        
        // Cek panjang 1-3 karakter
        if (input.empty() || input.length() > 3) {
            std::cout << "[!] " << fieldName << " harus 1-3 digit (ditemukan " << input.length() << " karakter).\n";
            continue;
        }
        
        // Cek semua karakter adalah angka
        bool allDigits = std::all_of(input.begin(), input.end(), ::isdigit);
        if (!allDigits) {
            std::cout << "[!] " << fieldName << " hanya boleh berisi angka (0-9).\n";
            continue;
        }
        
        // Cek tidak dimulai dengan 00 (kecuali "0" tunggal)
        if (input.length() > 1 && input[0] == '0' && input[1] == '0') {
            std::cout << "[!] " << fieldName << " tidak boleh dimulai dengan '00'.\n";
            continue;
        }
        
        return input;
    }
}

// ==========================================
// 1. STRUKTUR DATA (SEMUA KATEGORI)
// ==========================================
struct Warga {
    // Umum
    std::string nik, no_kk, nama_lengkap, tempat_lahir, tgl_lahir;
    char jenis_kelamin = ' ';
    std::optional<std::string> no_hp, provinsi, kota, kecamatan, desa, rt, rw, disabilitas;
    int kategori = 0;
    std::string created_at, updated_at;

    // Kategori 1: Bayi
    std::optional<std::string> nama_ortu, usia_bulan, bb, tb, lla, lingkar_kepala;
    std::optional<std::string> imun_bcg, imun_dpt, imun_polio, imun_campak, imun_hepb;
    std::optional<std::string> vit_a, obat_cacing, buku_kia, buku_kms, pmt, kapsul_yodium;
    std::optional<std::string> sirup_besi, oralit, asi_eksklusif, mp_asi, meninggal_tgl;
    std::optional<std::string> status_bayi, rujukan_bayi, catatan;

    // Kategori 2: Ibu Hamil
    std::optional<std::string> hpht, kehamilan_ke, bb_sebelum, tb_ibu, punya_kia, bb_sekarang;
    std::optional<std::string> lla_ibu, dapat_pmt, pmt_minggu, ttdp_minggu, diberikan_ttd;
    std::optional<std::string> gejala_janin, muntah, demam, ketuban, pendarahan, bengkak;
    std::optional<std::string> sakit_kepala, menggigil, batuk, bak, gatal, sulit_tidur;
    std::optional<std::string> cemas, jantung, kons_gizi, kons_bahaya_ham, kons_bahaya_pers;
    std::optional<std::string> status_hamil, rujukan_hamil, catatan_hamil;

    // Kategori 3: Ibu Menyusui
    std::optional<std::string> nama_suami, tgl_melahirkan, bb_menyusui, tb_menyusui;
    std::optional<std::string> punya_kia_m, lla_m, dapat_pmt_m, pmt_minggu_m, ttdp_minggu_m;
    std::optional<std::string> diberikan_ttd_m, sedang_menyusui, keluhan_menyusui, keluhan_lain;
    std::optional<std::string> kons_gizi_m, kons_kebersihan, kons_posisi, kons_asi;
    std::optional<std::string> tekanan_darah_m, kondisi_payudara, status_menyusui;
    std::optional<std::string> rujukan_menyusui, catatan_menyusui;

    // Kategori 4: Lansia
    std::optional<std::string> tekanan_darah_l, gula_darah, kolesterol, pengobatan;
    std::optional<std::string> keluhan_lansia, nyeri_sendi, bisa_jalan, keseimbangan;
    std::optional<std::string> riwayat_makan, gangguan_penglihatan, nafsu_makan, masalah_gigi;
    std::optional<std::string> malnutrisi, hipertensi, riwayat_penyakit, status_lansia;
    std::optional<std::string> rujukan_lansia, catatan_lansia;
};

// Fungsi ini sekarang menerima tipe 'RowType' apa pun (row, row_ref, proxy, dll.)
template<typename RowType>
std::optional<std::string> getStr(const RowType& r, const std::string& col) {
    auto field = r[col];
    return field.is_null() ? std::optional<std::string>{} 
                           : std::optional<std::string>(field.as<std::string>());
}

template<typename RowType>
double getDoubleOpt(const RowType& r, const std::string& col) {
    return r[col].is_null() ? 0.0 : r[col].as<double>();
}

// ==========================================
// 2. ALGORITMA MANUAL (ACADEMIC FOCUS)
// ==========================================
void insertionSort(std::vector<Warga>& data) {
    for (size_t i = 1; i < data.size(); ++i) {
        Warga key = data[i];
        int j = static_cast<int>(i) - 1;
        while (j >= 0 && data[j].nik > key.nik) {
            data[j + 1] = data[j];
            --j;
        }
        data[j + 1] = key;
    }
}

int binarySearch(const std::vector<Warga>& data, const std::string& target) {
    int l = 0, r = static_cast<int>(data.size()) - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (data[mid].nik == target) return mid;
        if (data[mid].nik < target) l = mid + 1;
        else r = mid - 1;
    }
    return -1;
}

// Benchmark untuk laporan skripsi
void runBenchmark(const std::vector<Warga>& originalData) {
    if (originalData.empty()) {
        std::cout << "\nData kosong, benchmark dilewati.\n";
        return;
    }
    // Buat copy non-const agar insertionSort bisa memodifikasinya
    auto dataCopy = originalData; 
    
    std::cout << "\n=== BENCHMARK ALGORITMA ===\n";
    auto t0 = std::chrono::high_resolution_clock::now();
    insertionSort(dataCopy);
    auto t1 = std::chrono::high_resolution_clock::now();
    auto durSort = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

    std::string testNik = dataCopy[dataCopy.size() / 2].nik;
    t0 = std::chrono::high_resolution_clock::now();
    binarySearch(dataCopy, testNik);
    t1 = std::chrono::high_resolution_clock::now();
    auto durSearch = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

    std::cout << "Insertion Sort: " << durSort << " us | n = " << originalData.size() << "\n";
    std::cout << "Binary Search : " << durSearch << " us | target = " << testNik << "\n";
    std::cout << "Benchmark selesai.\n\n";
}

// ==========================================
// 3. DATABASE CRUD (libpqxx)
// ==========================================
std::vector<Warga> fetchAll(pqxx::connection& conn) {
    std::vector<Warga> data;
    pqxx::work txn(conn);
    pqxx::result res = txn.exec("SELECT * FROM warga_posyandu");

    for (pqxx::result::size_type i = 0; i < res.size(); ++i) {
        const auto& row = res[i];
        Warga w; // Default construct, lalu assign manual (menghindari error aggregate init)
        
        // --- Data Umum ---
        w.nik = row["nik"].as<std::string>();
        w.no_kk = row["no_kk"].as<std::string>();
        w.nama_lengkap = row["nama_lengkap"].as<std::string>();
        w.tempat_lahir = getStr(row, "tempat_lahir").value_or("");
        w.tgl_lahir = row["tgl_lahir"].is_null() ? "" : row["tgl_lahir"].as<std::string>();
        auto jk_field = row["jenis_kelamin"];
        if (jk_field.is_null()) {
            w.jenis_kelamin = ' ';
        } else {
            std::string jk_str = jk_field.as<std::string>();
            w.jenis_kelamin = jk_str.empty() ? ' ' : jk_str[0];
        }
        w.no_hp = getStr(row, "no_hp");
        w.provinsi = getStr(row, "provinsi");
        w.kota = getStr(row, "kota");
        w.kecamatan = getStr(row, "kecamatan");
        w.desa = getStr(row, "desa");
        w.rt = getStr(row, "rt");
        w.rw = getStr(row, "rw");
        w.disabilitas = getStr(row, "status_disabilitas");
        w.kategori = row["kategori"].as<int>();
        w.created_at = row["created_at"].is_null() ? "" : row["created_at"].as<std::string>();
        w.updated_at = row["updated_at"].is_null() ? "" : row["updated_at"].as<std::string>();

        // --- Kategori 1 ---
        w.nama_ortu = getStr(row, "nama_orangtua"); w.bb = getStr(row, "bb");
        w.tb = getStr(row, "tb"); w.lla = getStr(row, "lingkar_lengan_atas");
        w.lingkar_kepala = getStr(row, "lingkar_kepala");
        w.imun_bcg = getStr(row, "imunisasi_bcg"); w.imun_dpt = getStr(row, "imunisasi_dpt");
        w.imun_polio = getStr(row, "imunisasi_polio"); w.imun_campak = getStr(row, "imunisasi_campak");
        w.imun_hepb = getStr(row, "imunisasi_hepatitis"); w.vit_a = getStr(row, "vitamin_a");
        w.obat_cacing = getStr(row, "obat_cacing"); w.buku_kia = getStr(row, "buku_kia");
        w.buku_kms = getStr(row, "buku_kms"); w.pmt = getStr(row, "pmt");
        w.kapsul_yodium = getStr(row, "kapsul_yodium"); w.sirup_besi = getStr(row, "sirup_besi");
        w.oralit = getStr(row, "oralit"); w.asi_eksklusif = getStr(row, "asi_eksklusif");
        w.mp_asi = getStr(row, "mp_asi"); w.meninggal_tgl = getStr(row, "bayi_meninggal_tgl");
        w.status_bayi = getStr(row, "status_kesehatan"); w.rujukan_bayi = getStr(row, "perlu_rujukan");
        w.catatan = getStr(row, "catatan_posyandu");

        // --- Kategori 2 ---
        w.hpht = getStr(row, "hpht"); w.kehamilan_ke = getStr(row, "kehamilan_ke");
        w.bb_sebelum = getStr(row, "bb_sebelum_hamil"); w.tb_ibu = getStr(row, "tb_ibu");
        w.punya_kia = getStr(row, "punya_kia"); w.bb_sekarang = getStr(row, "bb_sekarang");
        w.lla_ibu = getStr(row, "lingkar_lengan_atas_ibu"); w.dapat_pmt = getStr(row, "dapat_pmt");
        w.pmt_minggu = getStr(row, "pmt_seminggu"); w.ttdp_minggu = getStr(row, "ttpd_seminggu");
        w.diberikan_ttd = getStr(row, "diberikan_ttpd"); w.gejala_janin = getStr(row, "gerakan_janin");
        w.muntah = getStr(row, "muntah_terus"); w.demam = getStr(row, "demam_tinggi");
        w.ketuban = getStr(row, "ketuban_keluar"); w.pendarahan = getStr(row, "pendarahan");
        w.bengkak = getStr(row, "bengkak"); w.sakit_kepala = getStr(row, "sakit_kepala_kejang");
        w.menggigil = getStr(row, "demam_menggigil"); w.batuk = getStr(row, "batuk_2minggu");
        w.bak = getStr(row, "sakit_bak"); w.gatal = getStr(row, "gatal_kemaluan");
        w.sulit_tidur = getStr(row, "sulit_tidur"); w.cemas = getStr(row, "kecemasan");
        w.jantung = getStr(row, "jantung_berdebar"); w.kons_gizi = getStr(row, "konseling_gizi");
        w.kons_bahaya_ham = getStr(row, "konseling_bahaya_kehamilan");
        w.kons_bahaya_pers = getStr(row, "konseling_bahaya_persalinan");
        w.status_hamil = getStr(row, "status_kesehatan_ibu"); w.rujukan_hamil = getStr(row, "perlu_rujukan_ibu");
        w.catatan_hamil = getStr(row, "catatan_ibu");

        // --- Kategori 3 ---
        w.nama_suami = getStr(row, "nama_suami"); w.tgl_melahirkan = getStr(row, "tgl_melahirkan");
        w.bb_menyusui = getStr(row, "bb_ibu"); w.tb_menyusui = getStr(row, "tb_ibu_menyusui");
        w.punya_kia_m = getStr(row, "punya_kia_menyusui"); w.lla_m = getStr(row, "lingkar_lengan_atas_menyusui");
        w.dapat_pmt_m = getStr(row, "dapat_pmt_menyusui"); w.pmt_minggu_m = getStr(row, "pmt_seminggu_menyusui");
        w.ttdp_minggu_m = getStr(row, "ttpd_seminggu_menyusui"); w.diberikan_ttd_m = getStr(row, "diberikan_ttpd_menyusui");
        w.sedang_menyusui = getStr(row, "sedang_menyusui"); w.keluhan_menyusui = getStr(row, "keluhan_menyusui");
        w.keluhan_lain = getStr(row, "keluhan_selama_ini_menyusui");
        w.kons_gizi_m = getStr(row, "konseling_gizi_menyusui"); w.kons_kebersihan = getStr(row, "konseling_kebersihan");
        w.kons_posisi = getStr(row, "konseling_posisi"); w.kons_asi = getStr(row, "konseling_asi");
        w.tekanan_darah_m = getStr(row, "tekanan_darah"); w.kondisi_payudara = getStr(row, "kondisi_payudara");
        w.status_menyusui = getStr(row, "status_kesehatan_menyusui"); w.rujukan_menyusui = getStr(row, "perlu_rujukan_menyusui");
        w.catatan_menyusui = getStr(row, "catatan_menyusui");

        // --- Kategori 4 ---
        w.tekanan_darah_l = getStr(row, "tekanan_darah_lansia"); w.gula_darah = getStr(row, "gula_darah");
        w.kolesterol = getStr(row, "kolesterol"); w.pengobatan = getStr(row, "pengobatan_lansia");
        w.keluhan_lansia = getStr(row, "keluhan_lansia"); w.nyeri_sendi = getStr(row, "nyeri_sendi");
        w.bisa_jalan = getStr(row, "bisa_berjalan"); w.keseimbangan = getStr(row, "keseimbangan_tubuh");
        w.riwayat_makan = getStr(row, "riwayat_makan"); w.gangguan_penglihatan = getStr(row, "gangguan_penglihatan");
        w.nafsu_makan = getStr(row, "penurunan_nafsu_makan"); w.masalah_gigi = getStr(row, "masalah_gigi");
        w.malnutrisi = getStr(row, "malnutrisi"); w.hipertensi = getStr(row, "hipertensi");
        w.riwayat_penyakit = getStr(row, "riwayat_penyakit"); w.status_lansia = getStr(row, "status_kesehatan_lansia");
        w.rujukan_lansia = getStr(row, "perlu_rujukan_lansia"); w.catatan_lansia = getStr(row, "catatan_lansia");

        data.push_back(std::move(w));
    }
    return data;
}

// === Input Data Khusus per Kategori ===
void inputBayi(Warga& w) {
    std::cin.ignore();
    std::cout << "\n=== DATA KHUSUS BAYI ===\n";
    w.nama_ortu = inputString("Nama Orang Tua: ");
    w.usia_bulan = std::to_string(inputInt("Usia (bulan, 0-60): ", 0, 60));
    w.bb = std::to_string(inputDouble("Berat Badan (kg, 1-30): ", 1.0, 30.0));
    w.tb = std::to_string(inputDouble("Tinggi Badan (cm, 30-100): ", 30.0, 100.0));
    w.lla = std::to_string(inputDouble("Lingkar Lengan Atas (cm, 5-25): ", 5.0, 25.0));
    w.lingkar_kepala = std::to_string(inputDouble("Lingkar Kepala (cm, 25-55): ", 25.0, 55.0));
    
    std::cout << "\n-- Imunisasi --\n";
    w.imun_bcg = std::string(1, inputYN("BCG"));
    w.imun_dpt = inputImunisasi("DPT", 3);
    w.imun_polio = inputImunisasi("Polio", 4);
    w.imun_campak = std::string(1, inputYN("Campak"));
    w.imun_hepb = inputImunisasi("Hepatitis B", 4);
    
    std::cout << "\n-- Gizi & Buku --\n";
    w.vit_a = inputChoice("Vitamin A", {"dapat", "belum"});
    w.obat_cacing = inputChoice("Obat Cacing", {"dapat", "belum"});
    w.buku_kia = inputChoice("Buku KIA", {"dapat", "belum"});
    w.buku_kms = inputChoice("Buku KMS", {"dapat", "belum"});
    w.pmt = inputChoice("PMT", {"terjadwal", "tidak"});
    w.kapsul_yodium = std::string(1, inputYN("Kapsul Yodium"));
    w.sirup_besi = inputChoice("Sirup Besi", {"mengonsumsi", "tidak"});
    w.oralit = inputChoice("Oralit", {"diberikan", "tidak"});
    w.asi_eksklusif = inputChoice("ASI Eksklusif", {"diberikan", "tidak"});
    w.mp_asi = std::string(1, inputYN("MP ASI"));
    
    std::cout << "\n-- Kondisi & Status --\n";
    char meninggal = inputYN("Bayi meninggal?");
    if (meninggal == 'Y') {
        w.meninggal_tgl = inputDate("Tanggal meninggal (YYYY-MM-DD): ");
    } else {
        w.meninggal_tgl = "";
    }
    w.status_bayi = inputStatus("Status kesehatan");
    
    char rujukan = inputYN("Perlu rujukan?");
    if (rujukan == 'Y') {
        w.rujukan_bayi = inputString("Rujukan ke mana: ");
    } else {
        w.rujukan_bayi = "tidak";
    }
    w.catatan = inputString("Catatan Posyandu: ", true);
}

void inputIbuHamil(Warga& w) {
    std::cin.ignore();
    std::cout << "\n=== DATA KHUSUS IBU HAMIL ===\n";
    w.hpht = inputDate("HPHT (YYYY-MM-DD): ");
    w.kehamilan_ke = std::to_string(inputInt("Kehamilan ke- (1-10): ", 1, 10));
    w.bb_sebelum = std::to_string(inputDouble("BB Sebelum Hamil (kg, 30-150): ", 30.0, 150.0));
    w.tb_ibu = std::to_string(inputDouble("TB Ibu (cm, 130-200): ", 130.0, 200.0));
    w.punya_kia = std::string(1, inputYN("Punya KIA"));
    w.bb_sekarang = std::to_string(inputDouble("BB Sekarang (kg, 30-150): ", 30.0, 150.0));
    w.lla_ibu = std::to_string(inputDouble("Lingkar Lengan Atas (cm, 15-40): ", 15.0, 40.0));
    
    std::cout << "\n-- Nutrisi --\n";
    w.dapat_pmt = std::string(1, inputYN("Dapat PMT"));
    w.pmt_minggu = std::string(1, inputYN("PMT seminggu terakhir"));
    w.ttdp_minggu = std::string(1, inputYN("Mengonsumsi tablet tambah darah seminggu terakhir"));
    char ttd = inputYN("Diberikan tablet tambah darah?");
    if (ttd == 'Y') {
        w.diberikan_ttd = std::to_string(inputInt("Berapa tablet (1-90): ", 1, 90));
    } else {
        w.diberikan_ttd = "";
    }
    
    std::cout << "\n-- Tanda Bahaya --\n";
    w.gejala_janin = std::string(1, inputYN("Gerakan janin berkurang"));
    w.muntah = std::string(1, inputYN("Muntah terus menerus"));
    w.demam = std::string(1, inputYN("Demam tinggi"));
    w.ketuban = std::string(1, inputYN("Air ketuban keluar"));
    w.pendarahan = std::string(1, inputYN("Pendarahan"));
    w.bengkak = std::string(1, inputYN("Kaki/tangan/wajah bengkak"));
    w.sakit_kepala = std::string(1, inputYN("Sakit kepala disertai kejang"));
    w.menggigil = std::string(1, inputYN("Demam menggigil berkeringat"));
    w.batuk = std::string(1, inputYN("Batuk lebih dari 2 minggu"));
    w.bak = std::string(1, inputYN("Sakit saat BAK"));
    w.gatal = std::string(1, inputYN("Gatal area kemaluan"));
    w.sulit_tidur = std::string(1, inputYN("Sulit tidur"));
    w.cemas = std::string(1, inputYN("Kecemasan"));
    w.jantung = std::string(1, inputYN("Jantung berdebar-debar"));
    
    std::cout << "\n-- Konseling --\n";
    w.kons_gizi = std::string(1, inputYN("Pernah konseling KIE gizi seimbang"));
    w.kons_bahaya_ham = std::string(1, inputYN("Pernah konseling tanda bahaya kehamilan"));
    w.kons_bahaya_pers = std::string(1, inputYN("Pernah konseling tanda bahaya persalinan"));
    
    std::cout << "\n-- Status --\n";
    w.status_hamil = inputStatus("Status kesehatan");
    char rujukan = inputYN("Perlu rujukan?");
    if (rujukan == 'Y') {
        w.rujukan_hamil = inputString("Rujukan ke mana: ");
    } else {
        w.rujukan_hamil = "tidak";
    }
    w.catatan_hamil = inputString("Catatan: ", true);
}

void inputIbuMenyusui(Warga& w) {
    std::cin.ignore();
    std::cout << "\n=== DATA KHUSUS IBU MENYUSUI ===\n";
    w.nama_suami = inputString("Nama Suami: ");
    w.tgl_melahirkan = inputDate("Tgl Melahirkan (YYYY-MM-DD): ");
    w.bb_menyusui = std::to_string(inputDouble("BB (kg, 30-150): ", 30.0, 150.0));
    w.tb_menyusui = std::to_string(inputDouble("TB (cm, 130-200): ", 130.0, 200.0));
    w.punya_kia_m = std::string(1, inputYN("Punya KIA"));
    w.lla_m = std::to_string(inputDouble("Lingkar Lengan Atas (cm, 15-40): ", 15.0, 40.0));
    
    std::cout << "\n-- Nutrisi --\n";
    w.dapat_pmt_m = std::string(1, inputYN("Dapat PMT"));
    w.pmt_minggu_m = std::string(1, inputYN("PMT seminggu terakhir"));
    w.ttdp_minggu_m = std::string(1, inputYN("Mengonsumsi tablet tambah darah seminggu terakhir"));
    char ttd = inputYN("Diberikan tablet tambah darah?");
    if (ttd == 'Y') {
        w.diberikan_ttd_m = std::to_string(inputInt("Berapa tablet (1-90): ", 1, 90));
    } else {
        w.diberikan_ttd_m = "";
    }
    
    std::cout << "\n-- Menyusui --\n";
    w.sedang_menyusui = std::string(1, inputYN("Sedang menyusui"));
    w.keluhan_menyusui = std::string(1, inputYN("Keluhan saat menyusui"));
    char keluhan = inputYN("Keluhan selama ini");
    if (keluhan == 'Y') {
        w.keluhan_lain = inputString("Sebutkan keluhan: ");
    } else {
        w.keluhan_lain = "";
    }
    
    std::cout << "\n-- Konseling --\n";
    w.kons_gizi_m = std::string(1, inputYN("Konseling KIE gizi seimbang"));
    w.kons_kebersihan = std::string(1, inputYN("Konseling kebersihan"));
    w.kons_posisi = std::string(1, inputYN("Konseling posisi menyusui"));
    w.kons_asi = std::string(1, inputYN("Konseling ASI Eksklusif"));
    
    std::cout << "\n-- Pemeriksaan --\n";
    w.tekanan_darah_m = inputString("Tekanan Darah (contoh: 120/80): ");
    w.kondisi_payudara = inputString("Kondisi Payudara: ");
    
    std::cout << "\n-- Status --\n";
    w.status_menyusui = inputStatus("Status kesehatan");
    char rujukan = inputYN("Perlu rujukan?");
    if (rujukan == 'Y') {
        w.rujukan_menyusui = inputString("Rujukan ke mana: ");
    } else {
        w.rujukan_menyusui = "tidak";
    }
    w.catatan_menyusui = inputString("Catatan: ", true);
}

void inputLansia(Warga& w) {
    std::cin.ignore();
    std::cout << "\n=== DATA KHUSUS LANSIA ===\n";
    w.tekanan_darah_l = inputString("Tekanan Darah (contoh: 120/80): ");
    w.gula_darah = std::to_string(inputDouble("Gula Darah (mg/dL, 50-500): ", 50.0, 500.0));
    w.kolesterol = std::to_string(inputDouble("Kolesterol (mg/dL, 100-400): ", 100.0, 400.0));
    w.pengobatan = inputString("Pengobatan yang dilakukan: ", true);
    w.keluhan_lansia = inputString("Keluhan saat ini: ", true);
    
    std::cout << "\n-- Kondisi Fisik --\n";
    w.nyeri_sendi = std::string(1, inputYN("Nyeri sendi"));
    w.bisa_jalan = std::string(1, inputYN("Bisa berjalan"));
    w.keseimbangan = inputChoice("Keseimbangan tubuh", {"normal", "perlu bantuan"});
    w.riwayat_makan = inputString("Riwayat makanan: ", true);
    
    std::cout << "\n-- Gangguan --\n";
    char gangguang = inputYN("Gangguan penglihatan");
    if (gangguang == 'Y') {
        w.gangguan_penglihatan = inputString("Sebutkan gangguan: ");
    } else {
        w.gangguan_penglihatan = "";
    }
    w.nafsu_makan = std::string(1, inputYN("Penurunan nafsu makan"));
    w.masalah_gigi = inputString("Masalah gigi: ", true);
    w.malnutrisi = std::string(1, inputYN("Malnutrisi"));
    w.hipertensi = std::string(1, inputYN("Hipertensi"));
    char riwayat = inputYN("Ada riwayat penyakit");
    if (riwayat == 'Y') {
        w.riwayat_penyakit = inputString("Sebutkan riwayat: ");
    } else {
        w.riwayat_penyakit = "";
    }
    
    std::cout << "\n-- Status --\n";
    w.status_lansia = inputStatus("Status kesehatan");
    char rujukan = inputYN("Perlu rujukan?");
    if (rujukan == 'Y') {
        w.rujukan_lansia = inputString("Rujukan ke mana: ");
    } else {
        w.rujukan_lansia = "tidak";
    }
    w.catatan_lansia = inputString("Catatan: ", true);
}

void saveOrUpdate(pqxx::connection& conn, const Warga& w) {
    pqxx::work txn(conn);
    auto to_db = [](const std::optional<std::string>& opt) -> const char* {
        return (opt.has_value() && !opt.value().empty()) ? opt.value().c_str() : nullptr;
    };
    auto to_db_str = [](const std::string& s) -> const char* {
        return s.empty() ? nullptr : s.c_str();
    };
    
    txn.exec(
        "INSERT INTO warga_posyandu ("
        "nik, no_kk, nama_lengkap, tempat_lahir, tgl_lahir, jenis_kelamin, "
        "no_hp, provinsi, kota, kecamatan, desa, rt, rw, status_disabilitas, kategori, "
        "nama_orangtua, bb, tb, lingkar_lengan_atas, lingkar_kepala, "
        "imunisasi_bcg, imunisasi_dpt, imunisasi_polio, vitamin_a, asi_eksklusif, mp_asi, "
        "status_kesehatan, catatan_posyandu, "
        "hpht, kehamilan_ke, bb_sebelum_hamil, bb_sekarang, punya_kia, dapat_pmt, "
        "gerakan_janin, pendarahan, bengkak, konseling_gizi, status_kesehatan_ibu, catatan_ibu, "
        "nama_suami, tgl_melahirkan, bb_ibu, tb_ibu_menyusui, sedang_menyusui, dapat_pmt_menyusui, "
        "konseling_asi, kondisi_payudara, tekanan_darah, status_kesehatan_menyusui, catatan_menyusui, "
        "tekanan_darah_lansia, gula_darah, kolesterol, pengobatan_lansia, keluhan_lansia, "
        "nyeri_sendi, bisa_berjalan, gangguan_penglihatan, malnutrisi, hipertensi, "
        "riwayat_penyakit, status_kesehatan_lansia, catatan_lansia"
        ") VALUES ("
        "$1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14,$15,"
        "$16,$17,$18,$19,$20,$21,$22,$23,$24,$25,$26,$27,$28,"
        "$29,$30,$31,$32,$33,$34,$35,$36,$37,$38,$39,$40,"
        "$41,$42,$43,$44,$45,$46,$47,$48,$49,$50,$51,"
        "$52,$53,$54,$55,$56,$57,$58,$59,$60,$61,$62,$63,$64"
        ") ON CONFLICT (nik) DO UPDATE SET "
        "no_kk=$2, nama_lengkap=$3, tempat_lahir=$4, tgl_lahir=$5, jenis_kelamin=$6, "
        "kategori=$15, "
        "nama_orangtua=$16, bb=$17, tb=$18, lingkar_lengan_atas=$19, lingkar_kepala=$20, "
        "imunisasi_bcg=$21, imunisasi_dpt=$22, imunisasi_polio=$23, vitamin_a=$24, asi_eksklusif=$25, mp_asi=$26, "
        "status_kesehatan=$27, catatan_posyandu=$28, "
        "hpht=$29, kehamilan_ke=$30, bb_sebelum_hamil=$31, bb_sekarang=$32, punya_kia=$33, dapat_pmt=$34, "
        "gerakan_janin=$35, pendarahan=$36, bengkak=$37, konseling_gizi=$38, status_kesehatan_ibu=$39, catatan_ibu=$40, "
        "nama_suami=$41, tgl_melahirkan=$42, bb_ibu=$43, tb_ibu_menyusui=$44, sedang_menyusui=$45, dapat_pmt_menyusui=$46, "
        "konseling_asi=$47, kondisi_payudara=$48, tekanan_darah=$49, status_kesehatan_menyusui=$50, catatan_menyusui=$51, "
        "tekanan_darah_lansia=$52, gula_darah=$53, kolesterol=$54, pengobatan_lansia=$55, keluhan_lansia=$56, "
        "nyeri_sendi=$57, bisa_berjalan=$58, gangguan_penglihatan=$59, malnutrisi=$60, hipertensi=$61, "
        "riwayat_penyakit=$62, status_kesehatan_lansia=$63, catatan_lansia=$64, updated_at=CURRENT_TIMESTAMP",
        pqxx::params{
            to_db_str(w.nik), to_db_str(w.no_kk), to_db_str(w.nama_lengkap),
            to_db_str(w.tempat_lahir), to_db_str(w.tgl_lahir),
            to_db(std::string(1, w.jenis_kelamin)),
            to_db(w.no_hp), to_db(w.provinsi), to_db(w.kota),
            to_db(w.kecamatan), to_db(w.desa), to_db(w.rt),
            to_db(w.rw), to_db(w.disabilitas), std::to_string(w.kategori),
            // Bayi
            to_db(w.nama_ortu), to_db(w.bb), to_db(w.tb),
            to_db(w.lla), to_db(w.lingkar_kepala),
            to_db(w.imun_bcg), to_db(w.imun_dpt), to_db(w.imun_polio),
            to_db(w.vit_a), to_db(w.asi_eksklusif), to_db(w.mp_asi),
            to_db(w.status_bayi), to_db(w.catatan),
            // Ibu Hamil
            to_db(w.hpht), to_db(w.kehamilan_ke), to_db(w.bb_sebelum),
            to_db(w.bb_sekarang), to_db(w.punya_kia), to_db(w.dapat_pmt),
            to_db(w.gejala_janin), to_db(w.pendarahan), to_db(w.bengkak),
            to_db(w.kons_gizi), to_db(w.status_hamil), to_db(w.catatan_hamil),
            // Ibu Menyusui
            to_db(w.nama_suami), to_db(w.tgl_melahirkan), to_db(w.bb_menyusui),
            to_db(w.tb_menyusui), to_db(w.sedang_menyusui), to_db(w.dapat_pmt_m),
            to_db(w.kons_asi), to_db(w.kondisi_payudara), to_db(w.tekanan_darah_m),
            to_db(w.status_menyusui), to_db(w.catatan_menyusui),
            // Lansia
            to_db(w.tekanan_darah_l), to_db(w.gula_darah), to_db(w.kolesterol),
            to_db(w.pengobatan), to_db(w.keluhan_lansia),
            to_db(w.nyeri_sendi), to_db(w.bisa_jalan), to_db(w.gangguan_penglihatan),
            to_db(w.malnutrisi), to_db(w.hipertensi), to_db(w.riwayat_penyakit),
            to_db(w.status_lansia), to_db(w.catatan_lansia)
        }
    );
    txn.commit();
    std::cout << "[OK] Data tersimpan ke PostgreSQL.\n";
}

void deleteByNIK(pqxx::connection& conn, const std::string& nik) {
    pqxx::work txn(conn);
    // libpqxx v8: bungkus param dengan pqxx::params{...}
    txn.exec("DELETE FROM warga_posyandu WHERE nik = $1", pqxx::params{nik});
    txn.commit();
    std::cout << "Data dihapus dari PostgreSQL.\n";
}

// ==========================================
// 4. UI / MENU UTAMA
// ==========================================
void printWarga(const Warga& w) {
    std::cout << "\nDATA WARGA\n"
              << "NIK        : " << w.nik << "\n"
              << "Nama       : " << w.nama_lengkap << "\n"
              << "Kategori   : " << (w.kategori==1?"Bayi":w.kategori==2?"Ibu Hamil":w.kategori==3?"Ibu Menyusui":"Lansia") << "\n"
              << "Tgl Lahir  : " << w.tgl_lahir << " | JK: " << w.jenis_kelamin << "\n"
              << "Alamat     : " << w.desa.value_or("-") << ", Kec. " << w.kecamatan.value_or("-") << "\n"
              << "Status     : " << (w.status_bayi.has_value()? w.status_bayi.value() : 
                                     w.status_hamil.has_value()? w.status_hamil.value() :
                                     w.status_menyusui.has_value()? w.status_menyusui.value() :
                                     w.status_lansia.has_value()? w.status_lansia.value() : "-") << "\n\n";
}

int main() {
    try {
        // GANTI DENGAN CONNECTION STRING KAMU
        // Format: "host=localhost dbname=posyandu user=postgres password=123456"
        pqxx::connection conn("host=localhost dbname=posyandu user=postgres password=root");
        
        std::cout << "Terhubung ke PostgreSQL.\n";
        auto data = fetchAll(conn);
        std::cout << "Memuat " << data.size() << " record ke memori...\n";
        
        // Jalankan Insertion Sort otomatis saat startup
        insertionSort(data);
        std::cout << "Data diurutkan berdasarkan NIK (Insertion Sort).\n";

        while (true) {
            std::cout << "\n=== POSYANDU SYSTEM (C++ + PostgreSQL) ===\n"
                      << "1. Cari Data (Binary Search)\n"
                      << "2. Tambah Data Baru\n"
                      << "3. Edit Data\n"
                      << "4. Hapus Data\n"
                      << "5. Benchmark Algoritma\n"
                      << "6. Tampilkan Semua Data\n"
                      << "7. Keluar\n"
                      << "Pilih: ";
            int choice; std::cin >> choice;
            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; }

            if (choice == 1) {
                std::string nik; std::cout << "Masukkan NIK: "; std::cin >> nik;
                auto t0 = std::chrono::high_resolution_clock::now();
                int idx = binarySearch(data, nik);
                auto t1 = std::chrono::high_resolution_clock::now();
                auto dur = std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count();

                if (idx != -1) {
                    printWarga(data[idx]);
                    std::cout << "Waktu pencarian: " << dur << " us\n";
                } else {
                    std::cout << "NIK tidak ditemukan.\n";
                }
            } else if (choice == 2) {
                Warga w;
                std::cout << "\n╔════════════════════════════════════╗\n";
                std::cout <<   "║      INPUT DATA WARGA BARU        ║\n";
                std::cout <<   "╚════════════════════════════════════╝\n";
                std::cout << "\n--- Data Umum ---\n";
                
                w.nik = inputNIK("NIK (16 digit): ");
                w.no_kk = inputNIK("No KK (16 digit): ");
                w.nama_lengkap = inputString("Nama Lengkap: ");
                w.tempat_lahir = inputString("Tempat Lahir: ");
                w.tgl_lahir = inputDate("Tgl Lahir (YYYY-MM-DD): ");
                w.jenis_kelamin = inputGender("Jenis Kelamin");
                
                std::cout << "\n--- Alamat ---\n";
                w.no_hp = inputString("No HP (boleh kosong jika tekan Enter): ", true);
                w.provinsi = inputString("Provinsi: ");
                w.kota = inputString("Kota/Kabupaten: ");
                w.kecamatan = inputString("Kecamatan: ");
                w.desa = inputString("Desa/Kelurahan: ");
                w.rt = inputRTRW("RT", "RT");
                w.rw = inputRTRW("RW", "RW");
                
                char dis = inputYN("Status disabilitas");
                if (dis == 'Y') {
                    w.disabilitas = inputString("Jenis disabilitas: ");
                } else {
                    w.disabilitas = "N";
                }
                
                std::cout << "\n--- Kategori ---\n";
                std::cout << "1 = Bayi\n2 = Ibu Hamil\n3 = Ibu Menyusui\n4 = Lansia\n";
                w.kategori = inputKategori("Pilih Kategori (1-4): ");
                
                // Input data spesifik per kategori
                switch (w.kategori) {
                    case 1: inputBayi(w); break;
                    case 2: inputIbuHamil(w); break;
                    case 3: inputIbuMenyusui(w); break;
                    case 4: inputLansia(w); break;
                }
                
                // Simpan ke memory & DB
                data.push_back(w);
                insertionSort(data);
                try {
                    saveOrUpdate(conn, w);
                    std::cout << "\nData berhasil disimpan & diurutkan (Insertion Sort)!\n";
                } catch (const pqxx::sql_error& e) {
                    std::cerr << "\n[!] Database Error: " << e.what() << "\n";
                    data.pop_back(); // Rollback dari memory
                }
            } else if (choice == 3) {
                std::string nik; std::cout << "NIK yang diedit: "; std::cin >> nik;
                int idx = binarySearch(data, nik);
                if (idx != -1) {
                    std::cin.ignore();
                    std::cout << "Nama Baru: "; std::getline(std::cin, data[idx].nama_lengkap);
                    insertionSort(data);
                    saveOrUpdate(conn, data[idx]);
                } else std::cout << "Tidak ditemukan.\n";
            } else if (choice == 4) {
                std::string nik; std::cout << "NIK yang dihapus: "; std::cin >> nik;
                int idx = binarySearch(data, nik);
                if (idx != -1) {
                    data.erase(data.begin() + idx);
                    deleteByNIK(conn, nik);
                } else std::cout << "Tidak ditemukan.\n";
            } else if (choice == 5) {
                runBenchmark(data);
            } else if (choice == 6) {
                for (const auto& w : data) printWarga(w);
            } else if (choice == 7) {
                std::cout << "Program ditutup.\n";
                break;
            }
        }
    } catch (const pqxx::sql_error& e) {
        std::cerr << "SQL Error: " << e.what() << "\nQuery: " << e.query() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    return 0;
}

//editnya masih cuman nama
//emoji ga kerender