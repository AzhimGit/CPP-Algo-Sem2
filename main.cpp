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

using namespace std;


string trim(const string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    auto end = s.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

string toUpper(string s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}


string inputNama(const string& prompt) {
    string input;
    regex namaRegex(R"([A-Za-z\s'\-\.]+)");
    while (true) {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        if (input.empty()) { //
            cout << "Input tidak boleh kosong.\n";
            continue;
        }
        if (input.length() < 2) { //
            cout << "Nama minimal 2 karakter.\n";
            continue;
        }
        if (regex_match(input, namaRegex)) {
            return input;
        }
        cout << "Hanya boleh berisi huruf, spasi, tanda petik ('), strip (-), atau titik (.).\n";
    }
}

string inputNamaOptional(const string& prompt) {
    string input;
    regex namaRegex(R"([A-Za-z\s'\-\.]+)");
    while (true) {
        cout << prompt << " (boleh kosong): ";
        getline(cin, input);
        input = trim(input);
        if (input.empty()) return "";
        if (input.length() < 2) {
            cout << "Sabar, Nama minimal 2 karakter.\n";
            continue;
        }
        if (regex_match(input, namaRegex)) {
            return input;
        }
        cout << "Sabar, Hanya boleh berisi huruf, spasi, tanda petik ('), strip (-), atau titik (.).\n";
    }
}

// === VALIDASI NO HP (10-15 digit angka) ===
string inputNoHP(const string& prompt) {
    string input;
    regex hpRegex(R"(\d{10,15})");
    while (true) {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        if (input.empty()) return ""; // boleh kosong
        if (regex_match(input, hpRegex)) return input;
        cout << "Sabar, No HP harus 10-15 digit angka (contoh: 081234567890).\n";
    }
}

// === VALIDASI TEKS BEBAS (boleh angka, untuk catatan) ===
string inputString(const string& prompt, bool allowEmpty = false) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        if (!input.empty() || allowEmpty) return input;
        cout << "Sabar, Input tidak boleh kosong. Silakan ulangi.\n";
    }
}

// === VALIDASI NIK (16 digit angka) ===
string inputNIK(const string& prompt) {
    string input;
    regex nikRegex(R"(\d{16})");
    while (true) {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        if (regex_match(input, nikRegex)) return input;
        cout << "Sabar, Harus tepat 16 digit angka (contoh: 3201012345670001)\n";
    }
}

// === VALIDASI TANGGAL ===
bool isValidDate(const string& dateStr) {
    regex dateRegex(R"(\d{4}-\d{2}-\d{2})");
    if (!regex_match(dateStr, dateRegex)) return false;
    
    int y, m, d;
    if (sscanf(dateStr.c_str(), "%d-%d-%d", &y, &m, &d) != 3) return false;
    
    if (y < 1900 || y > 2100) return false;
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > 31) return false;
    
    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) daysInMonth[1] = 29;
    return d <= daysInMonth[m - 1];
}

string inputDate(const string& prompt, bool allowEmpty = false) { //
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        if (input.empty() && allowEmpty) return "";
        if (isValidDate(input)) return input;
        cout << "Format tanggal tidak valid. Gunakan YYYY-MM-DD (contoh: 2009-08-15)\n";
    }
}


int inputInt(const string& prompt, int min = 0, int max = 999999) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        try {
            size_t pos;
            int val = stoi(input, &pos);
            if (pos == input.length() && val >= min && val <= max) return val; //
        } catch (...) {}
        cout << "Masukkan bilangan bulat antara " << min << " - " << max << "\n";
    }
}

double inputDouble(const string& prompt, double min = 0.0, double max = 9999.9) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        try {
            size_t pos;
            double val = stod(input, &pos); //
            if (pos == input.length() && val >= min && val <= max) return val; //
        } catch (...) {}
        cout << "Masukkan angka valid antara " << min << " - " << max << "\n";
    }
}


char inputYN(const string& prompt) {
    string input;
    while (true) {
        cout << prompt << " (Y/N): ";
        getline(cin, input);
        input = trim(toUpper(input));
        if (input == "Y" || input == "N") return input[0]; //
        cout << "Hanya Y (Ya) atau N (Tidak) yang diperbolehkan.\n";
    }
}


char inputGender(const string& prompt) {
    string input;
    while (true) {
        cout << prompt << " (L/P): ";
        getline(cin, input);
        input = trim(toUpper(input));
        if (input == "L" || input == "P") return input[0]; //
        cout << "Hanya L (Laki-laki) atau P (Perempuan) yang diperbolehkan.\n";
    }
}

int inputKategori(const string& prompt) {
    while (true) {
        int k = inputInt(prompt, 1, 4);
        return k;
    }
}

string inputStatus(const string& prompt) {

    string input;
    while (true) {
        cout << prompt << " (normal/beresiko): ";
        getline(cin, input);
        input = trim(input);
        transform(input.begin(), input.end(), input.begin(), ::tolower);
        if (input == "normal" || input == "beresiko") return input;
        cout << "Hanya 'normal' atau 'beresiko' yang diperbolehkan.\n";
    }
}


string inputImunisasi(const string& prompt, int maxDosis = 3) {
    string input;
    while (true) {
        cout << prompt << " (1-" << maxDosis << " atau - jika belum): ";
        getline(cin, input);
        input = trim(input);
        if (input == "-") return "-";
        try {
            int dosis = stoi(input);
            if (dosis >= 1 && dosis <= maxDosis) return to_string(dosis);
        } catch (...) {} //
        cout << "Masukkan 1-" << maxDosis << " atau - untuk tidak.\n";
    }
}


string inputChoice(const string& prompt, const vector<string>& options) {
    string input;
    while (true) {
        cout << prompt << " (";
        for (size_t i = 0; i < options.size(); ++i) {
            cout << options[i];
            if (i < options.size() - 1) cout << "/";
        }
        cout << "): ";
        getline(cin, input);
        input = trim(input);
        transform(input.begin(), input.end(), input.begin(), ::tolower);
        for (const auto& opt : options) {
            string optLower = opt;
            transform(optLower.begin(), optLower.end(), optLower.begin(), ::tolower);
            if (input == optLower) return input;
        } //
        cout << "Pilihan tidak valid.\n";
    }
}


string inputRTRW(const string& prompt, const string& fieldName) {
    string input;
    while (true) {
        cout << prompt << " (1-3 digit angka, contoh: 01, 5, 012): ";
        getline(cin, input);
        input = trim(input);
        
        if (input.empty() || input.length() > 3) { //
            cout << fieldName << " harus 1-3 digit (ditemukan " << input.length() << " karakter).\n";
            continue;
        }
        
        bool allDigits = all_of(input.begin(), input.end(), ::isdigit);
        if (!allDigits) {
            cout << fieldName << " hanya boleh berisi angka (0-9).\n";
            continue;
        }
        
        if (input.length() > 1 && input[0] == '0' && input[1] == '0') {
            cout << fieldName << " tidak boleh dimulai dengan '00'.\n";
            continue;
        }
        
        return input;
    }
}


struct Warga {
    string nik, no_kk, nama_lengkap, tempat_lahir, tgl_lahir;
    char jenis_kelamin = ' ';
    optional<string> no_hp, provinsi, kota, kecamatan, desa, rt, rw, disabilitas;
    int kategori = 0;
    string created_at, updated_at;

    optional<string> nama_ortu, usia_bulan, bb, tb, lla, lingkar_kepala;
    optional<string> imun_bcg, imun_dpt, imun_polio, imun_campak, imun_hepb;
    optional<string> vit_a, obat_cacing, buku_kia, buku_kms, pmt, kapsul_yodium;
    optional<string> sirup_besi, oralit, asi_eksklusif, mp_asi, meninggal_tgl;
    optional<string> status_bayi, rujukan_bayi, catatan;

    optional<string> hpht, kehamilan_ke, bb_sebelum, tb_ibu, punya_kia, bb_sekarang;
    optional<string> lla_ibu, dapat_pmt, pmt_minggu, ttdp_minggu, diberikan_ttd;
    optional<string> gejala_janin, muntah, demam, ketuban, pendarahan, bengkak;
    optional<string> sakit_kepala, menggigil, batuk, bak, gatal, sulit_tidur;
    optional<string> cemas, jantung, kons_gizi, kons_bahaya_ham, kons_bahaya_pers;
    optional<string> status_hamil, rujukan_hamil, catatan_hamil;

    optional<string> nama_suami, tgl_melahirkan, bb_menyusui, tb_menyusui;
    optional<string> punya_kia_m, lla_m, dapat_pmt_m, pmt_minggu_m, ttdp_minggu_m;
    optional<string> diberikan_ttd_m, sedang_menyusui, keluhan_menyusui, keluhan_lain;
    optional<string> kons_gizi_m, kons_kebersihan, kons_posisi, kons_asi;
    optional<string> tekanan_darah_m, kondisi_payudara, status_menyusui;
    optional<string> rujukan_menyusui, catatan_menyusui;

    optional<string> tekanan_darah_l, gula_darah, kolesterol, pengobatan;
    optional<string> keluhan_lansia, nyeri_sendi, bisa_jalan, keseimbangan;
    optional<string> riwayat_makan, gangguan_penglihatan, nafsu_makan, masalah_gigi;
    optional<string> malnutrisi, hipertensi, riwayat_penyakit, status_lansia;
    optional<string> rujukan_lansia, catatan_lansia;
};


template<typename RowType>
optional<string> getStr(const RowType& r, const string& col) {
    auto field = r[col];
    return field.is_null() ? optional<string>{} 
                           : optional<string>(field.as<string>());
}


void insertionSort(vector<Warga>& data) {
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

int binarySearch(const vector<Warga>& data, const string& target) {
    int l = 0, r = static_cast<int>(data.size()) - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (data[mid].nik == target) return mid;
        if (data[mid].nik < target) l = mid + 1;
        else r = mid - 1;
    }
    return -1;
}


vector<Warga> fetchAll(pqxx::connection& conn) {
    vector<Warga> data;
    pqxx::work txn(conn);
    pqxx::result res = txn.exec("SELECT * FROM warga_posyandu");

    for (pqxx::result::size_type i = 0; i < res.size(); ++i) {
        const auto& row = res[i];
        Warga w;
        
        w.nik = row["nik"].as<string>();
        w.no_kk = row["no_kk"].as<string>();
        w.nama_lengkap = row["nama_lengkap"].as<string>();
        w.tempat_lahir = getStr(row, "tempat_lahir").value_or("");
        w.tgl_lahir = row["tgl_lahir"].is_null() ? "" : row["tgl_lahir"].as<string>();
        auto jk_field = row["jenis_kelamin"];
        if (jk_field.is_null()) {
            w.jenis_kelamin = ' ';
        } else {
            string jk_str = jk_field.as<string>();
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
        w.created_at = row["created_at"].is_null() ? "" : row["created_at"].as<string>();
        w.updated_at = row["updated_at"].is_null() ? "" : row["updated_at"].as<string>();

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

        w.tekanan_darah_l = getStr(row, "tekanan_darah_lansia"); w.gula_darah = getStr(row, "gula_darah");
        w.kolesterol = getStr(row, "kolesterol"); w.pengobatan = getStr(row, "pengobatan_lansia");
        w.keluhan_lansia = getStr(row, "keluhan_lansia"); w.nyeri_sendi = getStr(row, "nyeri_sendi");
        w.bisa_jalan = getStr(row, "bisa_berjalan"); w.keseimbangan = getStr(row, "keseimbangan_tubuh");
        w.riwayat_makan = getStr(row, "riwayat_makan"); w.gangguan_penglihatan = getStr(row, "gangguan_penglihatan");
        w.nafsu_makan = getStr(row, "penurunan_nafsu_makan"); w.masalah_gigi = getStr(row, "masalah_gigi");
        w.malnutrisi = getStr(row, "malnutrisi"); w.hipertensi = getStr(row, "hipertensi");
        w.riwayat_penyakit = getStr(row, "riwayat_penyakit"); w.status_lansia = getStr(row, "status_kesehatan_lansia");
        w.rujukan_lansia = getStr(row, "perlu_rujukan_lansia"); w.catatan_lansia = getStr(row, "catatan_lansia");

        data.push_back(move(w));
    }
    return data;
}


void inputBayi(Warga& w) {
    cin.ignore();
    cout << "\n=== DATA KHUSUS BAYI ===\n";
    w.nama_ortu = inputNama("Nama Orang Tua: ");
    w.usia_bulan = to_string(inputInt("Usia (bulan, 0-60): ", 0, 60));
    w.bb = to_string(inputDouble("Berat Badan (kg, 1-30): ", 1.0, 30.0));
    w.tb = to_string(inputDouble("Tinggi Badan (cm, 30-100): ", 30.0, 100.0));
    w.lla = to_string(inputDouble("Lingkar Lengan Atas (cm, 5-25): ", 5.0, 25.0));
    w.lingkar_kepala = to_string(inputDouble("Lingkar Kepala (cm, 25-55): ", 25.0, 55.0));
    
    cout << "\n-- Imunisasi --\n";
    w.imun_bcg = string(1, inputYN("BCG"));
    w.imun_dpt = inputImunisasi("DPT", 3);
    w.imun_polio = inputImunisasi("Polio", 4);
    w.imun_campak = string(1, inputYN("Campak"));
    w.imun_hepb = inputImunisasi("Hepatitis B", 4);
    
    cout << "\n-- Gizi & Buku --\n";
    w.vit_a = inputChoice("Vitamin A", {"dapat", "belum"});
    w.obat_cacing = inputChoice("Obat Cacing", {"dapat", "belum"});
    w.buku_kia = inputChoice("Buku KIA", {"dapat", "belum"});
    w.buku_kms = inputChoice("Buku KMS", {"dapat", "belum"});
    w.pmt = inputChoice("PMT", {"terjadwal", "tidak"});
    w.kapsul_yodium = string(1, inputYN("Kapsul Yodium"));
    w.sirup_besi = inputChoice("Sirup Besi", {"mengonsumsi", "tidak"});
    w.oralit = inputChoice("Oralit", {"diberikan", "tidak"});
    w.asi_eksklusif = inputChoice("ASI Eksklusif", {"diberikan", "tidak"});
    w.mp_asi = string(1, inputYN("MP ASI"));
    
    cout << "\n-- Kondisi & Status --\n";
    char meninggal = inputYN("Bayi meninggal?");
    if (meninggal == 'Y') {
        w.meninggal_tgl = inputDate("Tanggal meninggal (YYYY-MM-DD): ");
    } else {
        w.meninggal_tgl = "";
    }
    w.status_bayi = inputStatus("Status kesehatan");
    
    char rujukan = inputYN("Perlu rujukan?");
    if (rujukan == 'Y') {
        w.rujukan_bayi = inputNama("Rujukan ke mana: ");
    } else {
        w.rujukan_bayi = "tidak";
    }
    w.catatan = inputString("Catatan Posyandu: ", true);
}

void inputIbuHamil(Warga& w) {
    cin.ignore();
    cout << "\n=== DATA KHUSUS IBU HAMIL ===\n";
    w.hpht = inputDate("HPHT (YYYY-MM-DD): ");
    w.kehamilan_ke = to_string(inputInt("Kehamilan ke- (1-10): ", 1, 10));
    w.bb_sebelum = to_string(inputDouble("BB Sebelum Hamil (kg, 30-150): ", 30.0, 150.0));
    w.tb_ibu = to_string(inputDouble("TB Ibu (cm, 130-200): ", 130.0, 200.0));
    w.punya_kia = string(1, inputYN("Punya KIA"));
    w.bb_sekarang = to_string(inputDouble("BB Sekarang (kg, 30-150): ", 30.0, 150.0));
    w.lla_ibu = to_string(inputDouble("Lingkar Lengan Atas (cm, 15-40): ", 15.0, 40.0));
    
    cout << "\n-- Nutrisi --\n";
    w.dapat_pmt = string(1, inputYN("Dapat PMT"));
    w.pmt_minggu = string(1, inputYN("PMT seminggu terakhir"));
    w.ttdp_minggu = string(1, inputYN("Mengonsumsi tablet tambah darah seminggu terakhir"));
    char ttd = inputYN("Diberikan tablet tambah darah?");
    if (ttd == 'Y') {
        w.diberikan_ttd = to_string(inputInt("Berapa tablet (1-90): ", 1, 90));
    } else {
        w.diberikan_ttd = "";
    }
    
    cout << "\n-- Tanda Bahaya --\n";
    w.gejala_janin = string(1, inputYN("Gerakan janin berkurang"));
    w.muntah = string(1, inputYN("Muntah terus menerus"));
    w.demam = string(1, inputYN("Demam tinggi"));
    w.ketuban = string(1, inputYN("Air ketuban keluar"));
    w.pendarahan = string(1, inputYN("Pendarahan"));
    w.bengkak = string(1, inputYN("Kaki/tangan/wajah bengkak"));
    w.sakit_kepala = string(1, inputYN("Sakit kepala disertai kejang"));
    w.menggigil = string(1, inputYN("Demam menggigil berkeringat"));
    w.batuk = string(1, inputYN("Batuk lebih dari 2 minggu"));
    w.bak = string(1, inputYN("Sakit saat BAK"));
    w.gatal = string(1, inputYN("Gatal area kemaluan"));
    w.sulit_tidur = string(1, inputYN("Sulit tidur"));
    w.cemas = string(1, inputYN("Kecemasan"));
    w.jantung = string(1, inputYN("Jantung berdebar-debar"));
    
    cout << "\n-- Konseling --\n";
    w.kons_gizi = string(1, inputYN("Pernah konseling KIE gizi seimbang"));
    w.kons_bahaya_ham = string(1, inputYN("Pernah konseling tanda bahaya kehamilan"));
    w.kons_bahaya_pers = string(1, inputYN("Pernah konseling tanda bahaya persalinan"));
    
    cout << "\n-- Status --\n";
    w.status_hamil = inputStatus("Status kesehatan");
    char rujukan = inputYN("Perlu rujukan?");
    if (rujukan == 'Y') {
        w.rujukan_hamil = inputNama("Rujukan ke mana: ");
    } else {
        w.rujukan_hamil = "tidak";
    }
    w.catatan_hamil = inputString("Catatan: ", true);
}

void inputIbuMenyusui(Warga& w) {
    cin.ignore();
    cout << "\n=== DATA KHUSUS IBU MENYUSUI ===\n";
    w.nama_suami = inputNama("Nama Suami: ");
    w.tgl_melahirkan = inputDate("Tgl Melahirkan (YYYY-MM-DD): ");
    w.bb_menyusui = to_string(inputDouble("BB (kg, 30-150): ", 30.0, 150.0));
    w.tb_menyusui = to_string(inputDouble("TB (cm, 130-200): ", 130.0, 200.0));
    w.punya_kia_m = string(1, inputYN("Punya KIA"));
    w.lla_m = to_string(inputDouble("Lingkar Lengan Atas (cm, 15-40): ", 15.0, 40.0));
    
    cout << "\n-- Nutrisi --\n";
    w.dapat_pmt_m = string(1, inputYN("Dapat PMT"));
    w.pmt_minggu_m = string(1, inputYN("PMT seminggu terakhir"));
    w.ttdp_minggu_m = string(1, inputYN("Mengonsumsi tablet tambah darah seminggu terakhir"));
    char ttd = inputYN("Diberikan tablet tambah darah?");
    if (ttd == 'Y') {
        w.diberikan_ttd_m = to_string(inputInt("Berapa tablet (1-90): ", 1, 90));
    } else {
        w.diberikan_ttd_m = "";
    }
    
    cout << "\n-- Menyusui --\n";
    w.sedang_menyusui = string(1, inputYN("Sedang menyusui"));
    w.keluhan_menyusui = string(1, inputYN("Keluhan saat menyusui"));
    char keluhan = inputYN("Keluhan selama ini");
    if (keluhan == 'Y') {
        w.keluhan_lain = inputString("Sebutkan keluhan: ");
    } else {
        w.keluhan_lain = "";
    }
    
    cout << "\n-- Konseling --\n";
    w.kons_gizi_m = string(1, inputYN("Konseling KIE gizi seimbang"));
    w.kons_kebersihan = string(1, inputYN("Konseling kebersihan"));
    w.kons_posisi = string(1, inputYN("Konseling posisi menyusui"));
    w.kons_asi = string(1, inputYN("Konseling ASI Eksklusif"));
    
    cout << "\n-- Pemeriksaan --\n";
    w.tekanan_darah_m = inputString("Tekanan Darah (contoh: 120/80): ");
    w.kondisi_payudara = inputString("Kondisi Payudara: ");
    
    cout << "\n-- Status --\n";
    w.status_menyusui = inputStatus("Status kesehatan");
    char rujukan = inputYN("Perlu rujukan?");
    if (rujukan == 'Y') {
        w.rujukan_menyusui = inputNama("Rujukan ke mana: ");
    } else {
        w.rujukan_menyusui = "tidak";
    }
    w.catatan_menyusui = inputString("Catatan: ", true);
}

void inputLansia(Warga& w) {
    cin.ignore();
    cout << "\n=== DATA KHUSUS LANSIA ===\n";
    w.tekanan_darah_l = inputString("Tekanan Darah (contoh: 120/80): ");
    w.gula_darah = to_string(inputDouble("Gula Darah (mg/dL, 50-500): ", 50.0, 500.0));
    w.kolesterol = to_string(inputDouble("Kolesterol (mg/dL, 100-400): ", 100.0, 400.0));
    w.pengobatan = inputString("Pengobatan yang dilakukan: ", true);
    w.keluhan_lansia = inputString("Keluhan saat ini: ", true);
    
    cout << "\n-- Kondisi Fisik --\n";
    w.nyeri_sendi = string(1, inputYN("Nyeri sendi"));
    w.bisa_jalan = string(1, inputYN("Bisa berjalan"));
    w.keseimbangan = inputChoice("Keseimbangan tubuh", {"normal", "perlu bantuan"});
    w.riwayat_makan = inputString("Riwayat makanan: ", true);
    
    cout << "\n-- Gangguan --\n";
    char gangguang = inputYN("Gangguan penglihatan");
    if (gangguang == 'Y') {
        w.gangguan_penglihatan = inputString("Sebutkan gangguan: ");
    } else {
        w.gangguan_penglihatan = "";
    }
    w.nafsu_makan = string(1, inputYN("Penurunan nafsu makan"));
    w.masalah_gigi = inputString("Masalah gigi: ", true);
    w.malnutrisi = string(1, inputYN("Malnutrisi"));
    w.hipertensi = string(1, inputYN("Hipertensi"));
    char riwayat = inputYN("Ada riwayat penyakit");
    if (riwayat == 'Y') {
        w.riwayat_penyakit = inputString("Sebutkan riwayat: ");
    } else {
        w.riwayat_penyakit = "";
    }
    
    cout << "\n-- Status --\n";
    w.status_lansia = inputStatus("Status kesehatan");
    char rujukan = inputYN("Perlu rujukan?");
    if (rujukan == 'Y') {
        w.rujukan_lansia = inputNama("Rujukan ke mana: ");
    } else {
        w.rujukan_lansia = "tidak";
    }
    w.catatan_lansia = inputString("Catatan: ", true);
}


void saveOrUpdate(pqxx::connection& conn, const Warga& w) {
    pqxx::work txn(conn);
    auto to_db = [](const optional<string>& opt) -> const char* {
        return (opt.has_value() && !opt.value().empty()) ? opt.value().c_str() : nullptr;
    };
    auto to_db_str = [](const string& s) -> const char* {
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
            to_db(string(1, w.jenis_kelamin)),
            to_db(w.no_hp), to_db(w.provinsi), to_db(w.kota),
            to_db(w.kecamatan), to_db(w.desa), to_db(w.rt),
            to_db(w.rw), to_db(w.disabilitas), to_string(w.kategori),
            to_db(w.nama_ortu), to_db(w.bb), to_db(w.tb),
            to_db(w.lla), to_db(w.lingkar_kepala),
            to_db(w.imun_bcg), to_db(w.imun_dpt), to_db(w.imun_polio),
            to_db(w.vit_a), to_db(w.asi_eksklusif), to_db(w.mp_asi),
            to_db(w.status_bayi), to_db(w.catatan),
            to_db(w.hpht), to_db(w.kehamilan_ke), to_db(w.bb_sebelum),
            to_db(w.bb_sekarang), to_db(w.punya_kia), to_db(w.dapat_pmt),
            to_db(w.gejala_janin), to_db(w.pendarahan), to_db(w.bengkak),
            to_db(w.kons_gizi), to_db(w.status_hamil), to_db(w.catatan_hamil),
            to_db(w.nama_suami), to_db(w.tgl_melahirkan), to_db(w.bb_menyusui),
            to_db(w.tb_menyusui), to_db(w.sedang_menyusui), to_db(w.dapat_pmt_m),
            to_db(w.kons_asi), to_db(w.kondisi_payudara), to_db(w.tekanan_darah_m),
            to_db(w.status_menyusui), to_db(w.catatan_menyusui),
            to_db(w.tekanan_darah_l), to_db(w.gula_darah), to_db(w.kolesterol),
            to_db(w.pengobatan), to_db(w.keluhan_lansia),
            to_db(w.nyeri_sendi), to_db(w.bisa_jalan), to_db(w.gangguan_penglihatan),
            to_db(w.malnutrisi), to_db(w.hipertensi), to_db(w.riwayat_penyakit),
            to_db(w.status_lansia), to_db(w.catatan_lansia)
        }
    );
    txn.commit();
    cout << "Data tersimpan ke PostgreSQL.\n";
}

void deleteByNIK(pqxx::connection& conn, const string& nik) {
    pqxx::work txn(conn);
    txn.exec("DELETE FROM warga_posyandu WHERE nik = $1", pqxx::params{nik});
    txn.commit();
    cout << "Data dihapus dari PostgreSQL.\n";
}

void editWarga(pqxx::connection& conn, vector<Warga>& data, int idx) {
    Warga& w = data[idx];

    while (true) {
        cout << "\n================================================================\n";
        cout << "|                    EDIT DATA WARGA                           |\n";
        cout << "================================================================\n";
        cout << "Nama     : " << w.nama_lengkap << "\n";
        cout << "NIK      : " << w.nik << " (tidak bisa diedit)\n";
        cout << "Kategori : " << (w.kategori==1?"Bayi":w.kategori==2?"Ibu Hamil":w.kategori==3?"Ibu Menyusui":"Lansia") << "\n";
        cout << "\n--- A. DATA UMUM ---\n";
        cout << " 1. No KK              : " << w.no_kk << "\n";
        cout << " 2. Nama Lengkap       : " << w.nama_lengkap << "\n";
        cout << " 3. Tempat Lahir       : " << w.tempat_lahir << "\n";
        cout << " 4. Tanggal Lahir      : " << w.tgl_lahir << "\n";
        cout << " 5. Jenis Kelamin      : " << w.jenis_kelamin << "\n";
        cout << " 6. No HP              : " << w.no_hp.value_or("-") << "\n";
        cout << " 7. Provinsi           : " << w.provinsi.value_or("-") << "\n";
        cout << " 8. Kota/Kabupaten     : " << w.kota.value_or("-") << "\n";
        cout << " 9. Kecamatan          : " << w.kecamatan.value_or("-") << "\n";
        cout << "10. Desa/Kelurahan     : " << w.desa.value_or("-") << "\n";
        cout << "11. RT                 : " << w.rt.value_or("-") << "\n";
        cout << "12. RW                 : " << w.rw.value_or("-") << "\n";
        cout << "13. Disabilitas        : " << w.disabilitas.value_or("-") << "\n";
        
        cout << "\n--- B. DATA KHUSUS KATEGORI ---\n";
        
        if (w.kategori == 1) {
            cout << "21. Nama Orang Tua    : " << w.nama_ortu.value_or("-") << "\n";
            cout << "22. Usia (bulan)      : " << w.usia_bulan.value_or("-") << "\n";
            cout << "23. BB (kg)           : " << w.bb.value_or("-") << "\n";
            cout << "24. TB (cm)           : " << w.tb.value_or("-") << "\n";
            cout << "25. LLA (cm)          : " << w.lla.value_or("-") << "\n";
            cout << "26. Lingkar Kepala    : " << w.lingkar_kepala.value_or("-") << "\n";
            cout << "27. Imunisasi BCG     : " << w.imun_bcg.value_or("-") << "\n";
            cout << "28. Imunisasi DPT     : " << w.imun_dpt.value_or("-") << "\n";
            cout << "29. Imunisasi Polio   : " << w.imun_polio.value_or("-") << "\n";
            cout << "30. Imunisasi Campak  : " << w.imun_campak.value_or("-") << "\n";
            cout << "31. Imunisasi Hep B   : " << w.imun_hepb.value_or("-") << "\n";
            cout << "32. Vitamin A         : " << w.vit_a.value_or("-") << "\n";
            cout << "33. Obat Cacing       : " << w.obat_cacing.value_or("-") << "\n";
            cout << "34. Buku KIA          : " << w.buku_kia.value_or("-") << "\n";
            cout << "35. Buku KMS          : " << w.buku_kms.value_or("-") << "\n";
            cout << "36. PMT               : " << w.pmt.value_or("-") << "\n";
            cout << "37. Status Kesehatan  : " << w.status_bayi.value_or("-") << "\n";
            cout << "38. Rujukan           : " << w.rujukan_bayi.value_or("-") << "\n";
            cout << "39. Catatan           : " << w.catatan.value_or("-") << "\n";
        } else if (w.kategori == 2) {
            cout << "21. HPHT              : " << w.hpht.value_or("-") << "\n";
            cout << "22. Kehamilan Ke-     : " << w.kehamilan_ke.value_or("-") << "\n";
            cout << "23. BB Sebelum Hamil  : " << w.bb_sebelum.value_or("-") << "\n";
            cout << "24. TB Ibu            : " << w.tb_ibu.value_or("-") << "\n";
            cout << "25. BB Sekarang       : " << w.bb_sekarang.value_or("-") << "\n";
            cout << "26. LLA Ibu           : " << w.lla_ibu.value_or("-") << "\n";
            cout << "27. Punya KIA         : " << w.punya_kia.value_or("-") << "\n";
            cout << "28. Dapat PMT         : " << w.dapat_pmt.value_or("-") << "\n";
            cout << "29. Gerakan Janin     : " << w.gejala_janin.value_or("-") << "\n";
            cout << "30. Pendarahan        : " << w.pendarahan.value_or("-") << "\n";
            cout << "31. Bengkak           : " << w.bengkak.value_or("-") << "\n";
            cout << "32. Konseling Gizi    : " << w.kons_gizi.value_or("-") << "\n";
            cout << "33. Status Kesehatan  : " << w.status_hamil.value_or("-") << "\n";
            cout << "34. Rujukan           : " << w.rujukan_hamil.value_or("-") << "\n";
            cout << "35. Catatan           : " << w.catatan_hamil.value_or("-") << "\n";
        } else if (w.kategori == 3) {
            cout << "21. Nama Suami        : " << w.nama_suami.value_or("-") << "\n";
            cout << "22. Tgl Melahirkan    : " << w.tgl_melahirkan.value_or("-") << "\n";
            cout << "23. BB Ibu            : " << w.bb_menyusui.value_or("-") << "\n";
            cout << "24. TB Ibu            : " << w.tb_menyusui.value_or("-") << "\n";
            cout << "25. Tekanan Darah     : " << w.tekanan_darah_m.value_or("-") << "\n";
            cout << "26. Kondisi Payudara  : " << w.kondisi_payudara.value_or("-") << "\n";
            cout << "27. Sedang Menyusui   : " << w.sedang_menyusui.value_or("-") << "\n";
            cout << "28. Konseling ASI     : " << w.kons_asi.value_or("-") << "\n";
            cout << "29. Status Kesehatan  : " << w.status_menyusui.value_or("-") << "\n";
            cout << "30. Rujukan           : " << w.rujukan_menyusui.value_or("-") << "\n";
            cout << "31. Catatan           : " << w.catatan_menyusui.value_or("-") << "\n";
        } else if (w.kategori == 4) {
            cout << "21. Tekanan Darah     : " << w.tekanan_darah_l.value_or("-") << "\n";
            cout << "22. Gula Darah        : " << w.gula_darah.value_or("-") << "\n";
            cout << "23. Kolesterol        : " << w.kolesterol.value_or("-") << "\n";
            cout << "24. Pengobatan        : " << w.pengobatan.value_or("-") << "\n";
            cout << "25. Keluhan           : " << w.keluhan_lansia.value_or("-") << "\n";
            cout << "26. Nyeri Sendi       : " << w.nyeri_sendi.value_or("-") << "\n";
            cout << "27. Bisa Berjalan     : " << w.bisa_jalan.value_or("-") << "\n";
            cout << "28. Gangguan Penglihat: " << w.gangguan_penglihatan.value_or("-") << "\n";
            cout << "29. Malnutrisi        : " << w.malnutrisi.value_or("-") << "\n";
            cout << "30. Hipertensi        : " << w.hipertensi.value_or("-") << "\n";
            cout << "31. Riwayat Penyakit  : " << w.riwayat_penyakit.value_or("-") << "\n";
            cout << "32. Status Kesehatan  : " << w.status_lansia.value_or("-") << "\n";
            cout << "33. Rujukan           : " << w.rujukan_lansia.value_or("-") << "\n";
            cout << "34. Catatan           : " << w.catatan_lansia.value_or("-") << "\n";
        }
        
        cout << "\n 0. Selesai & Simpan ke Database\n";
        cout << "Pilih nomor field yang ingin diedit: ";
        
        int choice; cin >> choice;
        if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); continue; }
        
        if (choice == 0) {
            try {
                saveOrUpdate(conn, w);
                cout << "\nPerubahan berhasil disimpan ke Database!\n";
            } catch (const pqxx::sql_error& e) {
                cerr << "\nDatabase Error: " << e.what() << "\n";
            } //
            break;
        }
        
        cin.ignore();
        
        // === DATA UMUM (1-13) ===
        switch (choice) {
            case 1: w.no_kk = inputNIK("No KK Baru (16 digit): "); break;
            case 2: w.nama_lengkap = inputNama("Nama Lengkap Baru: "); break;
            case 3: w.tempat_lahir = inputNama("Tempat Lahir Baru: "); break;
            case 4: w.tgl_lahir = inputDate("Tgl Lahir Baru (YYYY-MM-DD): "); break;
            case 5: w.jenis_kelamin = inputGender("Jenis Kelamin Baru"); break;
            case 6: w.no_hp = inputNoHP("No HP Baru: "); break;
            case 7: w.provinsi = inputNama("Provinsi Baru: "); break;
            case 8: w.kota = inputNama("Kota/Kabupaten Baru: "); break;
            case 9: w.kecamatan = inputNama("Kecamatan Baru: "); break;
            case 10: w.desa = inputNama("Desa/Kelurahan Baru: "); break;
            case 11: w.rt = inputRTRW("RT Baru", "RT"); break;
            case 12: w.rw = inputRTRW("RW Baru", "RW"); break;
            case 13: {
                char dis = inputYN("Status disabilitas baru");
                if (dis == 'Y') {
                    w.disabilitas = inputNama("Jenis disabilitas: ");
                } else {
                    w.disabilitas = "N";
                }
                break;
            }
            
            // === KATEGORI 1: BAYI (21-39) ===
            case 21:
                if (w.kategori == 1) w.nama_ortu = inputNama("Nama Orang Tua Baru: ");
                else if (w.kategori == 2) w.hpht = inputDate("HPHT Baru (YYYY-MM-DD): ");
                else if (w.kategori == 3) w.nama_suami = inputNama("Nama Suami Baru: ");
                else if (w.kategori == 4) w.tekanan_darah_l = inputString("Tekanan Darah Baru (cth: 120/80): ");
                break;
            case 22:
                if (w.kategori == 1) w.usia_bulan = to_string(inputInt("Usia Baru (bulan, 0-60): ", 0, 60));
                else if (w.kategori == 2) w.kehamilan_ke = to_string(inputInt("Kehamilan ke- Baru (1-10): ", 1, 10));
                else if (w.kategori == 3) w.tgl_melahirkan = inputDate("Tgl Melahirkan Baru (YYYY-MM-DD): ");
                else if (w.kategori == 4) w.gula_darah = to_string(inputDouble("Gula Darah Baru (mg/dL, 50-500): ", 50.0, 500.0));
                break;
            case 23:
                if (w.kategori == 1) w.bb = to_string(inputDouble("BB Baru (kg, 1-30): ", 1.0, 30.0));
                else if (w.kategori == 2) w.bb_sebelum = to_string(inputDouble("BB Sebelum Hamil Baru (kg, 30-150): ", 30.0, 150.0));
                else if (w.kategori == 3) w.bb_menyusui = to_string(inputDouble("BB Baru (kg, 30-150): ", 30.0, 150.0));
                else if (w.kategori == 4) w.kolesterol = to_string(inputDouble("Kolesterol Baru (mg/dL, 100-400): ", 100.0, 400.0));
                break;
            case 24:
                if (w.kategori == 1) w.tb = to_string(inputDouble("TB Baru (cm, 30-100): ", 30.0, 100.0));
                else if (w.kategori == 2) w.tb_ibu = to_string(inputDouble("TB Ibu Baru (cm, 130-200): ", 130.0, 200.0));
                else if (w.kategori == 3) w.tb_menyusui = to_string(inputDouble("TB Ibu Baru (cm, 130-200): ", 130.0, 200.0));
                else if (w.kategori == 4) w.pengobatan = inputString("Pengobatan Baru: ", true);
                break;
            case 25:
                if (w.kategori == 1) w.lla = to_string(inputDouble("LLA Baru (cm, 5-25): ", 5.0, 25.0));
                else if (w.kategori == 2) w.bb_sekarang = to_string(inputDouble("BB Sekarang Baru (kg, 30-150): ", 30.0, 150.0));
                else if (w.kategori == 3) w.tekanan_darah_m = inputString("Tekanan Darah Baru (cth: 120/80): ");
                else if (w.kategori == 4) w.keluhan_lansia = inputString("Keluhan Baru: ", true);
                break;
            case 26:
                if (w.kategori == 1) w.lingkar_kepala = to_string(inputDouble("Lingkar Kepala Baru (cm, 25-55): ", 25.0, 55.0));
                else if (w.kategori == 2) w.lla_ibu = to_string(inputDouble("LLA Ibu Baru (cm, 15-40): ", 15.0, 40.0));
                else if (w.kategori == 3) w.kondisi_payudara = inputString("Kondisi Payudara Baru: ");
                else if (w.kategori == 4) w.nyeri_sendi = string(1, inputYN("Nyeri Sendi Baru"));
                break;
            case 27:
                if (w.kategori == 1) w.imun_bcg = string(1, inputYN("Imunisasi BCG Baru"));
                else if (w.kategori == 2) w.punya_kia = string(1, inputYN("Punya KIA Baru"));
                else if (w.kategori == 3) w.sedang_menyusui = string(1, inputYN("Sedang Menyusui Baru"));
                else if (w.kategori == 4) w.bisa_jalan = string(1, inputYN("Bisa Berjalan Baru"));
                break;
            case 28:
                if (w.kategori == 1) w.imun_dpt = inputImunisasi("Imunisasi DPT Baru", 3);
                else if (w.kategori == 2) w.dapat_pmt = string(1, inputYN("Dapat PMT Baru"));
                else if (w.kategori == 3) w.kons_asi = string(1, inputYN("Konseling ASI Baru"));
                else if (w.kategori == 4) w.gangguan_penglihatan = inputString("Gangguan Penglihatan Baru: ", true);
                break;
            case 29:
                if (w.kategori == 1) w.imun_polio = inputImunisasi("Imunisasi Polio Baru", 4);
                else if (w.kategori == 2) w.gejala_janin = string(1, inputYN("Gerakan Janin Berkurang Baru"));
                else if (w.kategori == 3) w.status_menyusui = inputStatus("Status Kesehatan Baru");
                else if (w.kategori == 4) w.malnutrisi = string(1, inputYN("Malnutrisi Baru"));
                break;
            case 30:
                if (w.kategori == 1) w.imun_campak = string(1, inputYN("Imunisasi Campak Baru"));
                else if (w.kategori == 2) w.pendarahan = string(1, inputYN("Pendarahan Baru"));
                else if (w.kategori == 3) w.rujukan_menyusui = inputNama("Rujukan Baru: ");
                else if (w.kategori == 4) w.hipertensi = string(1, inputYN("Hipertensi Baru"));
                break;
            case 31:
                if (w.kategori == 1) w.imun_hepb = inputImunisasi("Imunisasi Hepatitis B Baru", 4);
                else if (w.kategori == 2) w.bengkak = string(1, inputYN("Bengkak Baru"));
                else if (w.kategori == 3) w.catatan_menyusui = inputString("Catatan Baru: ", true);
                else if (w.kategori == 4) w.riwayat_penyakit = inputString("Riwayat Penyakit Baru: ", true);
                break;
            case 32:
                if (w.kategori == 1) w.vit_a = inputChoice("Vitamin A Baru", {"dapat", "belum"});
                else if (w.kategori == 2) w.kons_gizi = string(1, inputYN("Konseling Gizi Baru"));
                else if (w.kategori == 4) w.status_lansia = inputStatus("Status Kesehatan Baru");
                break;
            case 33:
                if (w.kategori == 1) w.obat_cacing = inputChoice("Obat Cacing Baru", {"dapat", "belum"});
                else if (w.kategori == 2) w.status_hamil = inputStatus("Status Kesehatan Baru");
                else if (w.kategori == 4) w.rujukan_lansia = inputNama("Rujukan Baru: ");
                break;
            case 34:
                if (w.kategori == 1) w.buku_kia = inputChoice("Buku KIA Baru", {"dapat", "belum"});
                else if (w.kategori == 2) w.rujukan_hamil = inputNama("Rujukan Baru: ");
                else if (w.kategori == 4) w.catatan_lansia = inputString("Catatan Baru: ", true);
                break;
            case 35:
                if (w.kategori == 1) w.buku_kms = inputChoice("Buku KMS Baru", {"dapat", "belum"});
                else if (w.kategori == 2) w.catatan_hamil = inputString("Catatan Baru: ", true);
                break;
            case 36:
                if (w.kategori == 1) w.pmt = inputChoice("PMT Baru", {"terjadwal", "tidak"});
                break;
            case 37:
                if (w.kategori == 1) w.status_bayi = inputStatus("Status Kesehatan Baru");
                break;
            case 38:
                if (w.kategori == 1) w.rujukan_bayi = inputNama("Rujukan Baru: ");
                break;
            case 39:
                if (w.kategori == 1) w.catatan = inputString("Catatan Baru: ", true);
                break;
            default:
                cout << "Sabar, Pilihan tidak valid.\n";
        } //
        
        cout << "Field berhasil diperbarui.\n"; //
    }
}

string optStr(const optional<string>& opt) {
    return (opt.has_value() && !opt.value().empty()) ? opt.value() : "-";
}

void printWarga(const Warga& w) {
    string status = "-", catatan = "-";
    
    switch (w.kategori) {
        case 1: status = w.status_bayi.value_or("-"); catatan = w.catatan.value_or("-"); break;
        case 2: status = w.status_hamil.value_or("-"); catatan = w.catatan_hamil.value_or("-"); break;
        case 3: status = w.status_menyusui.value_or("-"); catatan = w.catatan_menyusui.value_or("-"); break;
        case 4: status = w.status_lansia.value_or("-"); catatan = w.catatan_lansia.value_or("-"); break;
    }
    
    cout << "\n====================================\n"
              << "| NIK        : " << w.nik << "\n"
              << "| Nama       : " << w.nama_lengkap << "\n"
              << "| Kategori   : " << (w.kategori==1?"Bayi":w.kategori==2?"Ibu Hamil":w.kategori==3?"Ibu Menyusui":"Lansia") << "\n"
              << "| Tgl Lahir  : " << w.tgl_lahir << " | JK: " << w.jenis_kelamin << "\n"
              << "| Alamat     : " << w.desa.value_or("-") << ", Kec. " << w.kecamatan.value_or("-") << "\n"
              << "| Status     : " << status << "\n"
              << "| Catatan    : " << catatan << "\n"
              << "====================================\n";
}

void printWargaDetail(const Warga& w) {
    cout << "\n==================================================\n";
    cout << "                 DETAIL DATA WARGA                \n";
    cout << "==================================================\n";
    
    cout << "\n[ DATA UMUM ]\n";
    cout << "NIK           : " << w.nik << "\n";
    cout << "No KK         : " << w.no_kk << "\n";
    cout << "Nama Lengkap  : " << w.nama_lengkap << "\n";
    cout << "TTL           : " << w.tempat_lahir << ", " << w.tgl_lahir << "\n";
    cout << "Jenis Kelamin : " << w.jenis_kelamin << "\n";
    cout << "No HP         : " << optStr(w.no_hp) << "\n";
    cout << "Alamat        : Ds. " << optStr(w.desa) << ", RT/RW " << optStr(w.rt) << "/" << optStr(w.rw) 
              << ", Kec. " << optStr(w.kecamatan) << ", " << optStr(w.kota) << ", " << optStr(w.provinsi) << "\n";
    cout << "Disabilitas   : " << optStr(w.disabilitas) << "\n";
    cout << "Kategori      : " << (w.kategori==1?"Bayi":w.kategori==2?"Ibu Hamil":w.kategori==3?"Ibu Menyusui":"Lansia") << "\n";

    if (w.kategori == 1) {
        cout << "\n[ DATA KHUSUS BAYI ]\n";
        cout << "Nama Ortu     : " << optStr(w.nama_ortu) << "\n";
        cout << "Usia (bln)    : " << optStr(w.usia_bulan) << "\n";
        cout << "BB / TB       : " << optStr(w.bb) << " kg / " << optStr(w.tb) << " cm\n";
        cout << "LLA / LK      : " << optStr(w.lla) << " cm / " << optStr(w.lingkar_kepala) << " cm\n";
        cout << "Imunisasi     : BCG(" << optStr(w.imun_bcg) << ") DPT(" << optStr(w.imun_dpt) 
                  << ") Polio(" << optStr(w.imun_polio) << ") Campak(" << optStr(w.imun_campak) 
                  << ") HepB(" << optStr(w.imun_hepb) << ")\n";
        cout << "Status        : " << optStr(w.status_bayi) << "\n";
        cout << "Rujukan       : " << optStr(w.rujukan_bayi) << "\n";
        cout << "Catatan       : " << optStr(w.catatan) << "\n";
    } 
    else if (w.kategori == 2) {
        cout << "\n[ DATA KHUSUS IBU HAMIL ]\n";
        cout << "HPHT          : " << optStr(w.hpht) << "\n";
        cout << "Kehamilan Ke  : " << optStr(w.kehamilan_ke) << "\n";
        cout << "BB Sblm/Skrng : " << optStr(w.bb_sebelum) << " kg / " << optStr(w.bb_sekarang) << " kg\n";
        cout << "Status        : " << optStr(w.status_hamil) << "\n";
        cout << "Rujukan       : " << optStr(w.rujukan_hamil) << "\n";
        cout << "Catatan       : " << optStr(w.catatan_hamil) << "\n";
    } 
    else if (w.kategori == 3) {
        cout << "\n[ DATA KHUSUS IBU MENYUSUI ]\n";
        cout << "Nama Suami    : " << optStr(w.nama_suami) << "\n";
        cout << "Tgl Melahirkan: " << optStr(w.tgl_melahirkan) << "\n";
        cout << "BB / TB       : " << optStr(w.bb_menyusui) << " kg / " << optStr(w.tb_menyusui) << " cm\n";
        cout << "Tensi         : " << optStr(w.tekanan_darah_m) << "\n";
        cout << "Status        : " << optStr(w.status_menyusui) << "\n";
        cout << "Rujukan       : " << optStr(w.rujukan_menyusui) << "\n";
        cout << "Catatan       : " << optStr(w.catatan_menyusui) << "\n";
    } 
    else if (w.kategori == 4) {
        cout << "\n[ DATA KHUSUS LANSIA ]\n";
        cout << "Tensi         : " << optStr(w.tekanan_darah_l) << "\n";
        cout << "Gula Darah    : " << optStr(w.gula_darah) << " mg/dL\n";
        cout << "Kolesterol    : " << optStr(w.kolesterol) << " mg/dL\n";
        cout << "Status        : " << optStr(w.status_lansia) << "\n";
        cout << "Rujukan       : " << optStr(w.rujukan_lansia) << "\n";
        cout << "Catatan       : " << optStr(w.catatan_lansia) << "\n";
    }
    cout << "==================================================\n\n";
}


int main() {
    try {
        pqxx::connection conn("host=localhost dbname=posyandu user=postgres password=root");
        
        cout << "Terhubung ke PostgreSQL.\n";
        auto data = fetchAll(conn);
        cout << "Memuat " << data.size() << " data ke memori...\n";
        
        insertionSort(data); //
        cout << "Data diurutkan berdasarkan NIK dengan Insertion Sort..\n";

        while (true) {
            cout << "\n=== POSYANDU SYSTEM ===\n"
                      << "1. Cari Data\n"
                      << "2. Tambah Data Baru\n"
                      << "3. Edit Data\n"
                      << "4. Hapus Data\n"
                      << "5. Tampilkan Semua Data\n"
                      << "6. Keluar\n"
                      << "Pilih: ";
            int choice; cin >> choice;
            if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); continue; }

            if (choice == 1) {
                string nik; cout << "Masukkan NIK: "; cin >> nik;
                auto t0 = chrono::high_resolution_clock::now();
                int idx = binarySearch(data, nik);
                auto t1 = chrono::high_resolution_clock::now();
                auto dur = chrono::duration_cast<chrono::microseconds>(t1-t0).count();

                if (idx != -1) {
                    printWargaDetail(data[idx]);
                    cout << "Data ditemukan!\n";
                } else {
                    cout << "NIK tidak ditemukan.\n"; //
                }
            } else if (choice == 2) {
                Warga w;
                cout << "\n======================================\n";
                cout <<   "|      INPUT DATA WARGA BARU        |\n";
                cout <<   "======================================\n";
                cout << "\n--- Data Umum ---\n";
                
                w.nik = inputNIK("NIK (16 digit): ");
                w.no_kk = inputNIK("No KK (16 digit): ");
                w.nama_lengkap = inputNama("Nama Lengkap: ");
                w.tempat_lahir = inputNama("Tempat Lahir: ");
                w.tgl_lahir = inputDate("Tgl Lahir (YYYY-MM-DD): ");
                w.jenis_kelamin = inputGender("Jenis Kelamin");
                
                cout << "\n--- Alamat ---\n";
                w.no_hp = inputNoHP("No HP (boleh kosong jika tekan Enter): ");
                w.provinsi = inputNama("Provinsi: ");
                w.kota = inputNama("Kota/Kabupaten: ");
                w.kecamatan = inputNama("Kecamatan: ");
                w.desa = inputNama("Desa/Kelurahan: ");
                w.rt = inputRTRW("RT", "RT");
                w.rw = inputRTRW("RW", "RW");
                
                char dis = inputYN("Status disabilitas");
                if (dis == 'Y') {
                    w.disabilitas = inputNama("Jenis disabilitas: ");
                } else {
                    w.disabilitas = "N";
                }
                
                cout << "\n--- Kategori ---\n";
                cout << "1 = Bayi\n2 = Ibu Hamil\n3 = Ibu Menyusui\n4 = Lansia\n";
                w.kategori = inputKategori("Pilih Kategori (1-4): ");
                
                switch (w.kategori) {
                    case 1: inputBayi(w); break;
                    case 2: inputIbuHamil(w); break;
                    case 3: inputIbuMenyusui(w); break;
                    case 4: inputLansia(w); break;
                }
                
                data.push_back(w);
                insertionSort(data);
                try {
                    saveOrUpdate(conn, w);
                    cout << "\nData berhasil disimpan & diurutkan dengan Insertion Sort..\n"; //
                } catch (const pqxx::sql_error& e) {
                    cerr << "\nDatabase Error: " << e.what() << "\n"; //
                    data.pop_back(); //
                }
            } else if (choice == 3) {
                string nik; 
                cout << "Masukkan NIK yang ingin diedit: "; 
                cin >> nik;
                
                int idx = binarySearch(data, nik);
                
                if (idx != -1) {
                    editWarga(conn, data, idx);
                } else {
                    cout << "NIK tidak ditemukan dalam data.\n"; //
                }
            } else if (choice == 4) {
                string nik; cout << "NIK yang dihapus: "; cin >> nik;
                int idx = binarySearch(data, nik);
                if (idx != -1) {
                    data.erase(data.begin() + idx);
                    deleteByNIK(conn, nik);
                } else cout << "Tidak ditemukan.\n"; //
            } else if (choice == 5) {
                for (const auto& w : data) printWarga(w);
            } else if (choice == 6) {
                cout << "Program ditutup.\n";
                break;
            }
        }
    } catch (const pqxx::sql_error& e) { //
        cerr << "SQL Error: " << e.what() << "\nQuery: " << e.query() << "\n"; //
    } catch (const exception& e) { //
        cerr << "Error: " << e.what() << "\n"; //
    } //
    return 0; //
} //