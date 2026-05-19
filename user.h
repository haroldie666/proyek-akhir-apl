#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <limits>
#include <mysql.h>
#include <cmath>
#include "tabulate/table.hpp"
#include "helper.h"

using namespace std;
using namespace tabulate;

extern string user;

inline void readKatalogGizi(MYSQL* conn) {
    system("cls");
    cout << "\n====== KATALOG GIZI MAKANAN ======\n\n";
    DataMakanan arr[100];
    int n = fetchMakananToArray(conn, arr, 100);

    if (n == 0) {
        cout << "Data makanan kosong.\n";
        return;
    }

    Table tbl;
    tbl.add_row({"No", "Nama Makanan", "Kategori", "Kalori", "Protein", "Karbo", "Lemak"});
    
    for (size_t i = 0; i < 7; ++i) {
        tbl[0][i].format()
            .font_align(FontAlign::center)
            .font_style({FontStyle::bold})
            .font_color(Color::yellow);
    }

    for (int i = 0; i < n; i++) {
        tbl.add_row({
            to_string(i + 1),
            arr[i].nama,
            arr[i].kategori,
            formatFloat(arr[i].kalori),
            formatFloat(arr[i].protein),
            formatFloat(arr[i].karbohidrat),
            formatFloat(arr[i].lemak)
        });
        tbl[i + 1][0].format().font_align(FontAlign::center);
    }
    cout << tbl << endl;
}

inline void reqGiziMakanan(MYSQL* conn) {
    system("cls");
    cout << "\n====== REQUEST GIZI MAKANAN ======\n\n";
    
    string namaReq;
    cout << "Masukkan nama makanan yang ingin di-request: ";
    getline(cin, namaReq);

    if (namaReq.empty()) {
        cout << "Nama makanan tidak boleh kosong!\n";
        return;
    }

    extern int currentUserId;

    string query = "INSERT INTO request_user (id_user, nama_makanan_req, status_request) VALUES (" 
                   + to_string(currentUserId) + ", '" + namaReq + "', 'Pending')";

    if (mysql_query(conn, query.c_str())) {
        cout << "Gagal mengirim request: " << mysql_error(conn) << endl;
    } else {
        cout << "\n\033[1;32m[BERHASIL] Request makanan '" << namaReq << "' telah dikirim ke Admin.\033[0m\n";
        cout << "Silakan cek status konfirmasi secara berkala.\n";
        catatLog(conn, currentUserId, "Mengirim request makanan baru: " + namaReq);
    }
}

inline void kalkulatorKalori() {
    system("cls");
    cout << "\n====== KALKULATOR KALORI HARIAN (BMR) ======\n\n";
    try {
        char gender; float berat, tinggi; int umur;
        cout << "Pilih Jenis Kelamin (L/P): "; cin >> gender;
        if (toupper(gender) != 'L' && toupper(gender) != 'P') throw runtime_error("Jenis kelamin tidak valid!");

        cout << "Masukkan Berat Badan (kg) : "; cin >> berat;
        cout << "Masukkan Tinggi Badan (cm): "; cin >> tinggi;
        cout << "Masukkan Umur (tahun)     : "; cin >> umur;

        if (cin.fail() || berat <= 0 || tinggi <= 0 || umur <= 0) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw runtime_error("Input harus berupa angka positif!");
        }

        float bmr = (toupper(gender) == 'L') 
            ? (88.362 + (13.397 * berat) + (4.799 * tinggi) - (5.677 * umur))
            : (447.593 + (9.247 * berat) + (3.098 * tinggi) - (4.330 * umur));

        cout << "\n============================================\n";
        cout << "Kalori Dasar (BMR) Anda: " << bmr << " Kkal/hari.\n";
    } catch (const exception& e) {
        cout << "\n\033[1;31m[ERROR] " << e.what() << "\033[0m\n";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

inline void kalkulatorBMI() {
    system("cls");
    cout << "\n====== KALKULATOR BMI (Body Mass Index) ======\n\n";
    try {
        float berat, tinggiCm;
        cout << "Masukkan Berat Badan (kg) : "; cin >> berat;
        cout << "Masukkan Tinggi Badan (cm): "; cin >> tinggiCm;

        if (cin.fail() || berat <= 0 || tinggiCm <= 0) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw runtime_error("Input harus berupa angka positif!");
        }

        float tinggiM = tinggiCm / 100.0;
        float bmi = berat / (tinggiM * tinggiM);

        cout << "\n============================================\n";
        cout << "Skor BMI Anda: " << round(bmi * 10.0) / 10.0 << "\nKategori     : ";
        if (bmi < 18.5) cout << "Kekurangan Berat Badan (Underweight)\n";
        else if (bmi >= 18.5 && bmi <= 24.9) cout << "Normal (Ideal)\n";
        else if (bmi >= 25.0 && bmi <= 29.9) cout << "Kelebihan Berat Badan (Overweight)\n";
        else cout << "Obesitas\n";

    } catch (const exception& e) {
        cout << "\n\033[1;31m[ERROR] " << e.what() << "\033[0m\n";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

inline void tabelMakananUser(DataMakanan arr[], int n) {
    if (n == 0) {
        cout << "Tidak ada data makanan\n";
        return;
    }
    
    Table tbl;
    tbl.add_row({"No", "Nama Makanan", "Kategori", "Kalori (kcal)", "Protein (g)", "Karbo (g)", "Lemak (g)"});
    tbl.row(0).format().font_align(FontAlign::center).font_style({FontStyle::bold}).font_color(Color::yellow);

    int no = 1; 
    for (int i = 0; i < n; i++) {
        tbl.add_row({
            to_string(no++), 
            arr[i].nama, 
            arr[i].kategori,
            formatFloat(arr[i].kalori), 
            formatFloat(arr[i].protein),
            formatFloat(arr[i].karbohidrat), 
            formatFloat(arr[i].lemak)
        });
    }

    for (size_t i = 1; i <= (size_t)n; ++i) {
        for(size_t j = 0; j < 7; ++j) {
            if(j != 1 && j != 2) tbl[i][j].format().font_align(FontAlign::center);
        }
    }
    cout << tbl << endl;
}

inline void menuSorting(MYSQL* conn) {
    string pilihan;
    while (true) {
        system("cls");
        cout << "\n====== MENU SORTING ======\n\n";
        
        DataMakanan arr[100];
        int n = fetchMakananToArray(conn, arr, 100);

        Table menuTbl;
        menuTbl.add_row({"No", "Menu Sorting"});
        menuTbl.add_row({"1", "Sorting Berdasarkan Kategori (A-Z)"});
        menuTbl.add_row({"2", "Sorting Berdasarkan Kalori Tertinggi-Terendah"});
        menuTbl.add_row({"3", "Sorting Berdasarkan Makronutrisi Tertinggi-Terendah"});
        menuTbl.add_row({"0", "Kembali ke Menu Sebelumnya"});

        menuTbl[0].format().font_align(FontAlign::center).font_style({FontStyle::bold});
        for (size_t i = 1; i <= 4; ++i) menuTbl[i][0].format().font_align(FontAlign::center);

        cout << menuTbl << endl;
        cout << "Pilih opsi: ";
        getline(cin, pilihan);

        if (pilihan == "0") break;

        try {
            if (n == 0) throw runtime_error("Tidak ada data untuk disortir.");

            if (pilihan == "1") {
                quickSortKategori(arr, 0, n - 1);
                system("cls");
                cout << "\nHasil Sorting Berdasarkan Kategori\n";
                tabelMakananUser(arr, n); 
            } 
            else if (pilihan == "2") {
                quickSortKalori(arr, 0, n - 1);
                system("cls");
                cout << "\nHasil Sorting Berdasarkan Kalori Tertinggi\n";
                tabelMakananUser(arr, n);
            } 
            else if (pilihan == "3") {
                system("cls");
                cout << "\n====== MAKRONUTRISI ======\n\n";

                Table tblMakro;
                tblMakro.add_row({"Opsi", "Makronutrisi"});
                tblMakro.add_row({"1", "Protein"});
                tblMakro.add_row({"2", "Karbohidrat"});
                tblMakro.add_row({"3", "Lemak"});

                tblMakro[0].format().font_align(FontAlign::center).font_style({FontStyle::bold});

                for (size_t i = 1; i <= 3; ++i) {
                    tblMakro[i][0].format().font_align(FontAlign::center);
                }
                
                cout << tblMakro << endl;
                cout << "Pilihan: ";
                string pilMakro; getline(cin, pilMakro);
                
                if (pilMakro == "1") {
                    quickSortMakro(arr, 0, n - 1, 1); 
                    system("cls");
                    cout << "\nHasil Sorting Protein Tertinggi\n";
                } else if (pilMakro == "2") {
                    quickSortMakro(arr, 0, n - 1, 2); 
                    system("cls");
                    cout << "\nHasil Sorting Karbohidrat Tertinggi\n";
                } else if (pilMakro == "3") {
                    quickSortMakro(arr, 0, n - 1, 3); 
                    system("cls");
                    cout << "\nHasil Sorting Lemak Tertinggi\n";
                } else {
                    throw invalid_argument("Pilihan makronutrisi tidak valid.");
                }
                tabelMakananUser(arr, n);
            } 
            else {
                throw invalid_argument("Pilihan tidak valid!");
            }

            cout << "\n\033[1;32mTekan enter untuk kembali ke menu sorting...\033[0m";
            cin.get();

        } 
        catch (const exception& e) {
            cout << "\033[1;31m[ERROR] " << e.what() << "\033[0m\n";
            cout << "\033[1;32mTekan enter untuk mencoba lagi...\033[0m";
            cin.get();
        }
    }
}

inline void menuSearching(MYSQL* conn) {
    string pilihan;
    while (true) {
        system("cls");
        cout << "\n====== MENU SEARCHING ======\n\n";
        
        DataMakanan arr[100];
        int n = fetchMakananToArray(conn, arr, 100);

        Table searchTbl;
        searchTbl.add_row({"No", "Menu Searching"});
        searchTbl.add_row({"1", "Cari Berdasarkan Nama Makanan"});
        searchTbl.add_row({"2", "Cari Berdasarkan Jumlah Kalori"});
        searchTbl.add_row({"0", "Kembali ke Menu Sebelumnya"});

        searchTbl[0].format().font_align(FontAlign::center);
        for (size_t i = 1; i <= 3; ++i) searchTbl[i][0].format().font_align(FontAlign::center);

        cout << searchTbl << endl;
        cout << "Pilih opsi pencarian: ";
        getline(cin, pilihan);

        if (pilihan == "0") 
        break;

        try {
            if (n == 0) throw runtime_error("Tidak ada data untuk dicari.");

            if (pilihan == "1") {
                cout << "\nMasukkan Nama Makanan : ";
                string key; getline(cin, key);
                quickSortNama(arr, 0, n - 1); 
                int index = binarySearchNama(arr, 0, n - 1, key);

                if (index != -1) {
                    cout << "\n\033[1;32m[DATA DITEMUKAN]\033[0m\n";
                    DataMakanan hasil[1] = {arr[index]}; 
                    tabelMakananUser(hasil, 1);
                } else { 
                    throw runtime_error("Makanan dengan nama '" + key + "' tidak ada di database."); 
                }
            } else if (pilihan == "2") {
                cout << "\nMasukkan Jumlah Kalori yang dicari (contoh: 130): ";
                string strKalori; getline(cin, strKalori);
                float keyKalori;
                
                try {
                    keyKalori = stof(strKalori);
                } catch (...) {
                    throw invalid_argument("Input harus berupa angka untuk kalori");
                }

                quickSortKalori(arr, 0, n - 1);
                int index = binarySearchKalori(arr, 0, n - 1, keyKalori);

                if (index != -1) {
                    cout << "\n\033[1;32m[DATA DITEMUKAN]\033[0m\n";
                    DataMakanan hasil[1] = {arr[index]}; 
                    tabelMakananUser(hasil, 1);
                } else { 
                    throw runtime_error("Makanan dengan kalori " + formatFloat(keyKalori) + " kcal tidak ada."); 
                }
            } else { 
                throw invalid_argument("Pilihan tidak valid"); 
            }

            cout << "\n\033[1;32mTekan enter untuk kembali ke menu searching...\033[0m";
            cin.get();

        } catch (const exception& e) {
            cout << "\033[1;31m[ERROR] " << e.what() << "\033[0m\n";
            cout << "\033[1;32mTekan enter untuk mencoba lagi...\033[0m";
            cin.get();
        }
    }
}

inline void menuUser(MYSQL* conn) {
    string pilihan;
    while (true) {
        system("cls");
        cout << "\n====== MENU USER ======\n\n";
        
        Table userMenu;
        userMenu.add_row({"No", "Menu User"});
        userMenu.add_row({"1", "Read Katalog Gizi"}); 
        userMenu.add_row({"2", "Req Gizi Makanan"});
        userMenu.add_row({"3", "Kalkulator Kalori"}); 
        userMenu.add_row({"4", "Kalkulator BMI"});
        userMenu.add_row({"5", "Sorting Makanan"}); 
        userMenu.add_row({"6", "Searching Makanan"});
        userMenu.add_row({"0", "Kembali ke menu sebelumnya"});
        userMenu[0].format().font_align(FontAlign::center);
        for (size_t i = 1; i <= 7; ++i) userMenu[i][0].format().font_align(FontAlign::center);
        
        cout << userMenu << "\nSilakan pilih menu yang tersedia : ";
        
        getline(cin, pilihan);

        // --- PERUBAHAN LOGIKA ERROR HANDLING DI SINI ---
        if (pilihan.empty()) {
            cout << "\n\033[1;31m[ERROR] Input tidak boleh kosong!\033[0m\n";
            cout << "Silakan tekan enter untuk memilih kembali...";
            cin.get();
            continue; 
        }
        // -----------------------------------------------

        if (pilihan == "1") { 
            readKatalogGizi(conn); 
            cout << "\n\033[1;32mTekan enter untuk kembali...\033[0m"; 
            cin.get(); 
        } 
        else if (pilihan == "2") { 
            reqGiziMakanan(conn); 
            cout << "\n\033[1;32mTekan enter untuk kembali...\033[0m"; 
            cin.get(); 
        } 
        else if (pilihan == "3") { 
            kalkulatorKalori(); 
            cout << "\n\033[1;32mTekan enter untuk kembali...\033[0m"; 
            cin.get(); 
        } 
        else if (pilihan == "4") { 
            kalkulatorBMI(); 
            cout << "\n\033[1;32mTekan enter untuk kembali...\033[0m"; 
            cin.get(); 
        } 
        else if (pilihan == "5") { 
            menuSorting(conn); 
        } 
        else if (pilihan == "6") { 
            menuSearching(conn); 
        } 
        else if (pilihan == "0") {
            extern bool isTerdaftar; 
            extern string userRole;
            isTerdaftar = false; user = ""; userRole = "";
            cout << "\n\033[1;32mTekan enter untuk melanjutkan...\033[0m"; 
            cin.get(); 
            break;
        } else {
            cout << "\n\033[1;33m[PERINGATAN] Pilihan '" << pilihan << "' tidak valid!\033[0m\n"; 
            cout << "Silakan tekan enter untuk mencoba lagi...";
            cin.get(); 
        }
    }
}
#endif