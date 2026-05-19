#include <winsock.h>
#include <iostream>
#include <mysql.h> // Untuk koneksi ke database MySQL
#include <conio.h> 
#include <string>
#include "tabulate/table.hpp" // Library untuk membuat tabel
#include "autentikasi.h" // Header untuk fungsi login dan register
#include "admin.h" // Header untuk menu admin
#include "user.h" // Header untuk menu user

using namespace std;
using namespace tabulate;

void enableANSI() {
    #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    #endif
}
void katalogDiet() {
    string pilihanDiet;

    while (true) {
        system("cls");

        cout << "\n========= KATALOG DIET =========\n\n";

        Table menuDiet;
        menuDiet.add_row({"No", "Kategori Diet"});
        menuDiet.add_row({"1", "Intermittent Fasting"});
        menuDiet.add_row({"2", "Cutting"});
        menuDiet.add_row({"3", "Bulking"});
        menuDiet.add_row({"4", "Ketogenik"});
        menuDiet.add_row({"0", "Kembali ke menu sebelumnya"});

        menuDiet[0].format().font_align(FontAlign::center);
        for (size_t i = 1; i <= 5; ++i) {
            menuDiet[i][0].format().font_align(FontAlign::center);
        }

        cout << menuDiet << endl;
        cout << "Silakan pilih menu yang tersedia : ";
        getline(cin, pilihanDiet);

        if (pilihanDiet == "1") {
            system("cls");
            cout << "\n======   INFORMASI TENTANG INTERMITTENT FASTING  ======\n\n";
            
            Table infoDiet;
            infoDiet.add_row({"Aspek", "Keterangan"});
            infoDiet.add_row({"Pengertian", "Metode pengaturan pola makan yang berfokus pada siklus\nkapan Anda makan dan kapan Anda berpuasa."});
            infoDiet.add_row({"Aturan Makan", "- Populer dengan metode 16:8 (16 jam puasa, 8 jam makan).\n- Saat puasa hanya boleh air putih, kopi/teh tanpa gula."});
            infoDiet.add_row({"Contoh Menu", "- Buka puasa: Dada ayam panggang, nasi merah, brokoli.\n- Cemilan: Buah apel segar atau segenggam almond.\n- Penutup: Telur rebus dan salad sayur."});
            infoDiet.add_row({"Manfaat", "- Memperbaiki sensitivitas insulin.\n- Membantu regenerasi sel (Autofagi).\n- Efektif menurunkan berat badan."});

            infoDiet[0].format().font_align(FontAlign::center);
            infoDiet[0][0].format().font_color(Color::yellow);
            infoDiet[0][1].format().font_color(Color::yellow);

            for(size_t i = 1; i <= 4; ++i) {
                infoDiet[i][0].format().font_align(FontAlign::center).font_color(Color::cyan);
            }
            
            cout << infoDiet << endl;
            cout << "\n\033[1;32mTekan enter untuk kembali...\033[0m";
            cin.get();
        } 
        else if (pilihanDiet == "2") {
            system("cls");
            cout << "\n======   INFORMASI TENTANG CUTIING  ======\n\n";
            
            Table infoDiet;
            infoDiet.add_row({"Aspek", "Keterangan"});
            infoDiet.add_row({"Pengertian", "Fase memangkas kadar lemak tubuh sebanyak mungkin\nsambil mempertahankan massa otot."});
            infoDiet.add_row({"Aturan Makan", "- Defisit Kalori (kurangi 300-500 kalori dari TDEE).\n- Protein tinggi untuk mencegah penyusutan otot.\n- Wajib diimbangi angkat beban (weightlifting)."});
            infoDiet.add_row({"Contoh Menu", "- Sarapan: Putih telur rebus dan secangkir oatmeal.\n- Makan siang: Dada ayam panggang dan ubi jalar.\n- Makan malam: Ikan nila bakar dan bayam rebus."});
            infoDiet.add_row({"Manfaat", "- Menurunkan risiko penyakit kardiovaskular.\n- Tubuh lebih ringan dan definisi otot lebih tajam."});

            infoDiet[0].format().font_align(FontAlign::center);
            infoDiet[0][0].format().font_color(Color::yellow);
            infoDiet[0][1].format().font_color(Color::yellow);
            
            for(size_t i = 1; i <= 4; ++i) {
                infoDiet[i][0].format().font_align(FontAlign::center).font_color(Color::cyan);
            }

            cout << infoDiet << endl;
            cout << "\n\033[1;32mTekan enter untuk kembali...\033[0m";
            cin.get();
        } 
        else if (pilihanDiet == "3") {
            system("cls");
            cout << "\n======   INFORMASI TENTANG BULKING  ======\n\n";
            
            Table infoDiet;
            infoDiet.add_row({"Aspek", "Keterangan"});
            infoDiet.add_row({"Pengertian", "Fase peningkatan massa otot dan berat badan secara\nsengaja melalui pola makan berlebih yang terstruktur."});
            infoDiet.add_row({"Aturan Makan", "- Surplus Kalori (tambah 300-500 kalori dari TDEE).\n- Mengutamakan karbohidrat kompleks & protein tinggi.\n- Fokus pada latihan beban progresif."});
            infoDiet.add_row({"Contoh Menu", "- Sarapan: Telur dadar, roti gandum, selai kacang.\n- Makan siang: Nasi porsi besar, daging sapi, tempe.\n- Minuman: Susu full cream atau protein shake."});
            infoDiet.add_row({"Manfaat", "- Meningkatkan kekuatan fisik dan daya tahan tulang.\n- Memperbaiki BMR (metabolisme pembakaran kalori)."});

            infoDiet[0].format().font_align(FontAlign::center);
            infoDiet[0][0].format().font_color(Color::yellow);
            infoDiet[0][1].format().font_color(Color::yellow);

            for(size_t i = 1; i <= 4; ++i) {
                infoDiet[i][0].format().font_align(FontAlign::center).font_color(Color::cyan);
            }

            cout << infoDiet << endl;
            cout << "\n\033[1;32mSilahkan tekan enter untuk kembali...\033[0m";
            cin.get();
        } 
        else if (pilihanDiet == "4") {
            system("cls");
            cout << "\n======   INFORMASI TENTANG DIET KATEGONIK  ======\n\n";
            
            Table infoDiet;
            infoDiet.add_row({"Aspek", "Keterangan"});
            infoDiet.add_row({"Pengertian", "Diet sangat rendah karbohidrat dan tinggi lemak\nuntuk mengubah sumber energi utama tubuh ke lemak."});
            infoDiet.add_row({"Aturan Makan", "Target: 70% Lemak, 20-25% Protein, 5% Karbohidrat.\nKarbohidrat dibatasi maksimal 20-50 gram/hari.\nMemancing fase 'Ketosis' dalam tubuh."});
            infoDiet.add_row({"Contoh Menu", "Sarapan: Telur orak-arik dimasak dengan mentega.\nMakan siang: Salmon panggang, alpukat, dan keju.\nMakan malam: Daging iga berlemak & sayur hijau."});
            infoDiet.add_row({"Manfaat", "Sangat cepat untuk menurunkan berat badan.\nMenstabilkan kadar gula darah dan insulin."});

            infoDiet[0].format().font_align(FontAlign::center);
            infoDiet[0][0].format().font_color(Color::yellow);
            infoDiet[0][1].format().font_color(Color::yellow);
            
            for(size_t i = 1; i <= 4; ++i) {
                infoDiet[i][0].format().font_align(FontAlign::center).font_color(Color::cyan);
            }

            cout << infoDiet << endl;
            cout << "\n\033[1;32mTekan enter untuk kembali...\033[0m";
            cin.get();
        }
        else if (pilihanDiet == "0") {
            break; 
        } 
        else {
            cout << "\n\033[1;31mPilihan tidak valid, silakan masukkan angka saja\033[0m" << endl;
            cout << "\033[1;32mTekan enter untuk kembali...\033[0m";
            cin.get();
        }
    }
}

int main() {
    enableANSI();

    MYSQL* conn = mysql_init(0);

    unsigned int ssl_mode = SSL_MODE_DISABLED;
    mysql_options(conn, MYSQL_OPT_SSL_MODE, &ssl_mode);
    mysql_options(conn, MYSQL_PLUGIN_DIR, "./plugin");

    if (!mysql_real_connect(conn, "localhost", "root", "", "db_gizi", 3306, NULL, 0)) {
        cout << "Koneksi Database Gagal: " << mysql_error(conn) << endl;
        return 1;
    }

    string pilihan;

    while (true) {
        system("cls");

        if (!isTerdaftar) {
            cout << "\n========= SELAMAT DATANG DI SIGIZI =========\n\n";

            Table mainMenu;
            mainMenu.add_row({"No", "Menu Utama"});
            mainMenu.add_row({"1", "Login"});
            mainMenu.add_row({"2", "Register"});
            mainMenu.add_row({"3", "Katalog Diet"});
            mainMenu.add_row({"0", "Logout"});

            mainMenu[0].format().font_align(FontAlign::center);
            for (size_t i = 1; i <= 4; 
                ++i) {
                mainMenu[i][0].format().font_align(FontAlign::center);
            }

            cout << mainMenu << endl;
            cout << "Silakan pilih menu yang tersedia : ";
            getline(cin, pilihan);

            if (pilihan == "1") {
                login(conn, 3);
            } else if (pilihan == "2") {
                registerUser(conn);
            } else if (pilihan == "3") {
                katalogDiet();
            } else if (pilihan == "0") {
                cout << "Terima kasih telah menggunakan program kami" << endl;
                break;
            } else {
                cout << "\n\033[1;31mPilihan tidak valid, silakan masukkan angka saja\033[0m" << endl;
                cout << "\033[1;32mSilakan tekan enter untuk memilih kembali...\033[0m" << endl;
                cin.get();
            }
        } 

        else {
            if (userRole == "admin") {
                menuAdmin(conn); 
            } 
            else if (userRole == "user") {
                menuUser(conn);
            }
        }
    }

    mysql_close(conn);
    return 0;
}