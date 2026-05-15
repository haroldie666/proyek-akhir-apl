#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include <mysql.h>
#include <vector>
#include <algorithm>
#include "tabulate/table.hpp"
#include "helper.h" 

using namespace std;
using namespace tabulate;

extern bool isTerdaftar;
extern string user;
extern string userRole;
extern int currentUserId;

struct DataRequest {
    int id_request;
    int id_user;
    string nama_makanan_req;
    string status_request;
};

inline void readData(MYSQL* conn) {
    system("cls");
    cout << "\n====== DATA MAKANAN ======\n\n";
    DataMakanan arr[100];
    int n = fetchMakananToArray(conn, arr, 100);
    tabelMakanan(arr, n);
}

inline void createData(MYSQL* conn) {
    system("cls");
    cout << "\n====== TAMBAH DATA MAKANAN ======\n\n";
    string nama, kategori; 
    float kalori, protein, karbohidrat, lemak;

    while (true) {
        try {
            cout << "Nama Makanan : "; 
            getline(cin, nama);

            if (nama.empty()) throw invalid_argument("Nama makanan tidak boleh kosong");
            
            if (nama.find_first_of("0123456789") != string::npos) {
                throw invalid_argument("Nama makanan tidak boleh mengandung angka!");
            }
            break; 
        } catch (const invalid_argument& e) {
            cout << "[ERROR] " << e.what() << "\nSilakan isi kembali.\n\n";
        }
    }

    while (true) {
        try {
            cout << "\nPilih Kategori Makanan:\n";
            
            Table tabelKategori;
            tabelKategori.add_row({"No", "Kategori Makanan"});
            tabelKategori.add_row({"1", "Lauk pauk"});
            tabelKategori.add_row({"2", "Cemilan"});
            tabelKategori.add_row({"3", "Makanan utama"});
            tabelKategori.add_row({"4", "Buah"});
            tabelKategori.add_row({"5", "Appetizer"});
            tabelKategori[0].format().font_align(FontAlign::center);           
            for (size_t i = 1; i <= 5; ++i) {
                tabelKategori[i][0].format().font_align(FontAlign::center);
            }
            
            cout << tabelKategori << endl;
            cout << "Masukkan pilihan (1-5): "; 
            
            string pilKat;
            getline(cin, pilKat);
            
            if (pilKat == "1") { kategori = "Lauk pauk"; 
                break; }
            else if (pilKat == "2") { kategori = "Cemilan"; 
                break; }
            else if (pilKat == "3") { kategori = "Makanan utama"; 
                break; }
            else if (pilKat == "4") { kategori = "Buah"; 
                break; }
            else if (pilKat == "5") { kategori = "Appetizer"; 
                break; }
            else {
                throw invalid_argument("Pilihan kategori tidak valid. Pilih harus angka 1-5.");
            }
        } catch (const invalid_argument& e) {
            cout << "[ERROR] " << e.what() << "\nSilakan isi kembali.\n";
        }
    }

    auto inputFloat = [](const string& prompt) -> float {
        string inputStr;
        while(true) {
            try {
                cout << prompt;
                getline(cin, inputStr);
                if (inputStr.empty()) throw invalid_argument("Input tidak boleh kosong");
                
                size_t pos;
                float val = stof(inputStr, &pos); 
                
                if (pos != inputStr.length()) throw invalid_argument("Input harus berupa angka murni");
                return val; 
                
            } catch (const invalid_argument& e) {
                string msg = e.what();
                if (msg == "stof") cout << "[ERROR] Input tidak valid. Harus berupa angka.\n\n";
                else cout << "[ERROR] " << msg << "\n\n";
            } catch (...) {
                cout << "[ERROR] Input tidak valid atau angka terlalu besar\n\n";
            }
        }
    };

    cout << endl; 
    kalori = inputFloat("Jumlah Kalori (kcal) : ");
    protein = inputFloat("Jumlah Protein (g) : ");
    karbohidrat = inputFloat("Jumlah Karbohidrat (g) : ");
    lemak = inputFloat("Jumlah Lemak (g) : ");

    string query = "INSERT INTO makanan (nama_makanan, kategori, kalori, protein, karbohidrat, lemak) VALUES ('" + 
                nama + "', '" + kategori + "', " + formatFloat(kalori) + ", " + formatFloat(protein) + ", " + 
                formatFloat(karbohidrat) + ", " + formatFloat(lemak) + ")";

    if (mysql_query(conn, query.c_str())) cout << "Gagal menyimpan data: " << mysql_error(conn) << endl;
    else 
    {
        cout << "\n\033[33mData Makanan berhasil ditambahkan ke database\033[0m" << endl;
        catatLog(conn, currentUserId, "Menambahkan data makanan baru");
    }
}

inline void editData(MYSQL* conn) {
    readData(conn);
    string targetId;
    while (true) {
        try {
            cout << "ps : Jika tidak ingin mengubah data makanan tertentu, cukup tekan enter";
            cout << "\nMasukkan ID Makanan yang akan diedit (ketik 0 untuk batal): "; 
            getline(cin, targetId);
            
            if (targetId == "0") 
            return; 
            if (targetId.empty()) throw invalid_argument("ID Makanan tidak boleh kosong!");

            if (targetId.find_first_not_of("0123456789") != string::npos) {
                throw invalid_argument("ID Makanan tidak valid. Harus berupa angka murni");
            }

            string checkQ = "SELECT * FROM makanan WHERE id_makanan = " + targetId;
            mysql_query(conn, checkQ.c_str());
            MYSQL_RES* res = mysql_store_result(conn);
            
            if (mysql_num_rows(res) == 0) { 
                mysql_free_result(res); 
                throw invalid_argument("Data dengan ID tersebut tidak ditemukan!");
            }
            
            mysql_free_result(res);
            break; 
            
        } catch (const invalid_argument& e) {
            cout << "[ERROR] " << e.what() << "\n";
        } catch (...) {
            cout << "[ERROR] Terjadi kesalahan saat membaca ID.\n";
        }
    }

    string namaBaru, kategoriBaru, kaloriBaru, proteinBaru, karboBaru, lemakBaru; 
    
    while (true) {
        try {
            cout << "\nNama makanan : "; 
            getline(cin, namaBaru);
            if (!namaBaru.empty() && namaBaru.find_first_of("0123456789") != string::npos) {
                throw invalid_argument("Nama makanan tidak boleh mengandung angka");
            }
            break;
        } catch (const invalid_argument& e) {
            cout << "[ERROR] " << e.what() << "\n";
        }
    }

    while (true) {
        try {
            cout << "\nUbah Kategori Makanan:\n";
            
            Table tabelKategori;
            tabelKategori.add_row({"No", "Kategori Makanan"});
            tabelKategori.add_row({"1", "Lauk pauk"});
            tabelKategori.add_row({"2", "Cemilan"});
            tabelKategori.add_row({"3", "Makanan utama"});
            tabelKategori.add_row({"4", "Buah"});
            tabelKategori.add_row({"5", "Appetizer"});

            tabelKategori[0].format().font_align(FontAlign::center);
            for (size_t i = 1; i <= 5; 
                ++i) {
                tabelKategori[i][0].format().font_align(FontAlign::center);
            }

            cout << tabelKategori << endl;
            cout << "Pilih menu diantara 1-5 : "; 
            
            string pilKat;
            getline(cin, pilKat);
            
            if (pilKat.empty()) { kategoriBaru = ""; 
                break; } 
            else if (pilKat == "1") { kategoriBaru = "Lauk pauk"; 
                break; }
            else if (pilKat == "2") { kategoriBaru = "Cemilan"; 
                break; }
            else if (pilKat == "3") { kategoriBaru = "Makanan utama"; 
                break; }
            else if (pilKat == "4") { kategoriBaru = "Buah"; 
                break; }
            else if (pilKat == "5") { kategoriBaru = "Appetizer"; 
                break; }
            else {
                throw invalid_argument("Pilihan kategori tidak valid! Masukkan angka 1-5 atau kosongkan.");
            }
        } catch (const invalid_argument& e) {
            cout << "\n[ERROR] " << e.what() << "\n";
        }
    }

    auto editFloatOpsional = [](const string& prompt, string& varRef) {
        while (true) {
            try {
                cout << prompt;
                getline(cin, varRef);
                
                if (varRef.empty()) break; 
                
                size_t pos;
                stof(varRef, &pos); 
                
                if (pos != varRef.length()) throw invalid_argument("Input harus angka murni!");
                break; 
                
            } catch (...) {
                cout << "[ERROR] Input harus berupa angka! Silakan isi kembali.\n";
            }
        }
    };

    cout << endl;
    editFloatOpsional("Kalori baru : ", kaloriBaru);
    editFloatOpsional("Protein baru (g) : ", proteinBaru);
    editFloatOpsional("Karbohidrat baru (g) : ", karboBaru);
    editFloatOpsional("Lemak baru (g) : ", lemakBaru);

    string updateQuery = "UPDATE makanan SET "; bool isUpdate = false;
    if (!namaBaru.empty()) { updateQuery += "nama_makanan = '" + namaBaru + "'"; isUpdate = true; }
    if (!kategoriBaru.empty()) { 
        if (isUpdate) updateQuery += ", "; updateQuery += "kategori = '" + kategoriBaru + "'"; isUpdate = true; }
    if (!kaloriBaru.empty()) { 
        if (isUpdate) updateQuery += ", "; updateQuery += "kalori = " + kaloriBaru; isUpdate = true; }
    if (!proteinBaru.empty()) { 
        if (isUpdate) updateQuery += ", "; updateQuery += "protein = " + proteinBaru; isUpdate = true; }
    if (!karboBaru.empty()) { 
        if (isUpdate) updateQuery += ", "; updateQuery += "karbohidrat = " + karboBaru; isUpdate = true; }
    if (!lemakBaru.empty()) { 
        if (isUpdate) updateQuery += ", "; updateQuery += "lemak = " + lemakBaru; isUpdate = true; }

    updateQuery += " WHERE id_makanan = " + targetId;
    if (isUpdate) {
        if (mysql_query(conn, updateQuery.c_str())) cout << "Gagal mengupdate: " << mysql_error(conn) << endl;
        else {
            cout << "\n\033[33mData berhasil diupdate\033[0m" << endl;
            catatLog(conn, currentUserId, "Mengubah data makanan dengan ID: " + targetId);
        }
    } else { 
        cout << "\nTidak ada data yang diubah." << endl; 
    }
}

inline void deleteData(MYSQL* conn) {
    readData(conn);
    string targetId; cout << "\nMasukkan ID Makanan yang akan dihapus: "; getline(cin, targetId);
    string query = "DELETE FROM makanan WHERE id_makanan = " + targetId;
    if (mysql_query(conn, query.c_str())) cout << "Gagal menghapus: " << mysql_error(conn) << endl;
    else 
    {
        cout << "\n\033[33mData berhasil dihapus\033[0m.\n";
        catatLog(conn, currentUserId, "Menghapus data makanan dengan ID: " + targetId);
    }
    }

inline void konfirmasiRequest(MYSQL* conn) {
    system("cls");
    cout << "\n====== KONFIRMASI REQUEST USER ======\n\n";
    
    string query = "SELECT r.id_request, r.id_user, u.username, r.nama_makanan_req, r.status_request "
                   "FROM request_user r "
                   "JOIN users u ON r.id_user = u.id "
                   "WHERE r.status_request = 'Pending'";

    mysql_query(conn, query.c_str());
    MYSQL_RES* res = mysql_store_result(conn);

    if (mysql_num_rows(res) == 0) { 
        cout << "Tidak ada request pending.\n"; 
        mysql_free_result(res); 
        return; 
    }

    Table tbl; 
    tbl.add_row({"ID Request", "ID User", "Username", "Nama Makanan", "Status"});
    for (size_t i = 0; i < 5; ++i) tbl[0][i].format().font_align(FontAlign::center).font_style({FontStyle::bold});

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        tbl.add_row({row[0], row[1], row[2], row[3], row[4]});
    }
    
    for (size_t i = 1; i <= mysql_num_rows(res); ++i) {
        tbl[i][0].format().font_align(FontAlign::center);
        tbl[i][1].format().font_align(FontAlign::center);
        tbl[i][4].format().font_align(FontAlign::center);
    }
    cout << tbl << "\n"; 
    mysql_free_result(res);

    string idReq, namaMakananReq, aksi;

    while (true) {
        try {
            cout << "Masukkan ID Request (0 = batal): "; 
            getline(cin, idReq);
            
            if (idReq == "0") return; 
            if (idReq.empty()) throw invalid_argument("ID Request tidak boleh kosong!");

            if (idReq.find_first_not_of("0123456789") != string::npos) {
                throw invalid_argument("ID Request harus berupa angka murni!");
            }

            string qNama = "SELECT nama_makanan_req FROM request_user WHERE id_request = " + idReq + " AND status_request = 'Pending'";
            mysql_query(conn, qNama.c_str());
            MYSQL_RES* resNama = mysql_store_result(conn);
            
            if (mysql_num_rows(resNama) == 0) {
                mysql_free_result(resNama);
                throw invalid_argument("ID Request tidak ditemukan atau statusnya bukan Pending");
            }
            
            MYSQL_ROW rowNama = mysql_fetch_row(resNama);
            namaMakananReq = rowNama[0];
            mysql_free_result(resNama);
            break; 
            
        } catch (const invalid_argument& e) {
            cout << "\033[1;31m[ERROR] " << e.what() << "\033[0m\nSilakan coba lagi.\n\n";
        }
    }

    while (true) {
        try {
            cout << "\n1. Terima | 2. Tolak \nPilihan: "; 
            getline(cin, aksi);
            
            if (aksi == "1" || aksi == "2") {
                break; 
            } else {
                throw invalid_argument("Pilihan tidak valid, Masukkan angka 1 atau 2.");
            }
        } catch (const invalid_argument& e) {
            cout << "\033[1;31m[ERROR] " << e.what() << "\033[0m\n";
        }
    }

    if (aksi == "1") {
        cout << "\n--- MELENGKAPI DATA GIZI UNTUK '" << namaMakananReq << "' ---\n";
        string kategori;
        float kalori, protein, karbohidrat, lemak;

        while (true) {
            try {
                cout << "\nPilih Kategori Makanan:\n";
                Table tabelKategori;
                tabelKategori.add_row({"No", "Kategori Makanan"});
                tabelKategori.add_row({"1", "Lauk pauk"});
                tabelKategori.add_row({"2", "Cemilan"});
                tabelKategori.add_row({"3", "Makanan utama"});
                tabelKategori.add_row({"4", "Buah"});
                tabelKategori.add_row({"5", "Appetizer"});
                
                tabelKategori[0].format().font_align(FontAlign::center).font_style({FontStyle::bold});
                for (size_t i = 1; i <= 5; ++i) tabelKategori[i][0].format().font_align(FontAlign::center);
                
                cout << tabelKategori << endl;
                cout << "Masukkan pilihan (1-5): "; 
                string pilKat;
                getline(cin, pilKat);
                
                if (pilKat == "1") { kategori = "Lauk pauk"; break; }
                else if (pilKat == "2") { kategori = "Cemilan"; break; }
                else if (pilKat == "3") { kategori = "Makanan utama"; break; }
                else if (pilKat == "4") { kategori = "Buah"; break; }
                else if (pilKat == "5") { kategori = "Appetizer"; break; }
                else throw invalid_argument("Pilihan kategori tidak valid. Harus angka 1-5.");
            } catch (const invalid_argument& e) {
                cout << "\033[1;31m[ERROR] " << e.what() << "\033[0m\n";
            }
        }

        auto inputFloat = [](const string& prompt) -> float {
            string inputStr;
            while(true) {
                try {
                    cout << prompt; getline(cin, inputStr);
                    if (inputStr.empty()) throw invalid_argument("Input tidak boleh kosong!");
                    size_t pos; 
                    float val = stof(inputStr, &pos); 
                    if (pos != inputStr.length()) throw invalid_argument("Input harus berupa angka murni!");
                    return val; 
                } catch (...) {
                    cout << "\033[1;31m[ERROR] Input tidak valid! Harus berupa angka.\033[0m\n";
                }
            }
        };

        kalori = inputFloat("Jumlah Kalori : ");
        protein = inputFloat("Jumlah Protein (g) : ");
        karbohidrat = inputFloat("Jumlah Karbohidrat (g) : ");
        lemak = inputFloat("Jumlah Lemak (g) : ");

        string qInsert = "INSERT INTO makanan (nama_makanan, kategori, kalori, protein, karbohidrat, lemak) VALUES ('" + 
                         namaMakananReq + "', '" + kategori + "', " + formatFloat(kalori) + ", " + 
                         formatFloat(protein) + ", " + formatFloat(karbohidrat) + ", " + formatFloat(lemak) + ")";
        
        if (mysql_query(conn, qInsert.c_str())) {
            cout << "\033[1;31m\nGagal menyimpan ke katalog makanan: \033[0m" << mysql_error(conn) << "\n";
            return; 
        } else {
            cout << "\n\033[1;32m[BERHASIL] '" << namaMakananReq << "' telah ditambahkan ke katalog gizi.\033[0m\n";
        }
    }

    string stat = (aksi == "1") ? "Diterima" : "Ditolak";
    string qUpdate = "UPDATE request_user SET status_request = '" + stat + "' WHERE id_request = " + idReq;
    
    if (mysql_query(conn, qUpdate.c_str())) {
        cout << "\033[1;31mGagal memperbarui status request: \033[0m" << mysql_error(conn) << endl;
    } else {
        cout << "Status Request berhasil diubah menjadi: " << stat << ".\n";
        catatLog(conn, currentUserId, "Melakukan konfirmasi request ID " + idReq + " menjadi " + stat);
    }
}

inline void readLog(MYSQL* conn) {
    system("cls");
    cout << "\n====== LOG AKTIVITAS USER ======\n\n";
    mysql_query(conn, "SELECT id_log, id_user, aktivitas, waktu FROM log_user ORDER BY waktu DESC");
    MYSQL_RES* res = mysql_store_result(conn);
    if (mysql_num_rows(res) == 0) { 
        cout << "Belum ada riwayat.\n"; 
        mysql_free_result(res); 
        return; }

    Table tbl; tbl.add_row({"ID Log", "ID User", "Aktivitas", "Waktu"});
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) tbl.add_row({row[0], row[1], row[2], row[3]});
    cout << tbl << endl; mysql_free_result(res);
}

inline int fetchRequestToArray(MYSQL* conn, DataRequest arr[], int max_size) {
    string query = "SELECT id_request, id_user, nama_makanan_req, status_request FROM request_user";
    if (mysql_query(conn, query.c_str())) return 0;
    MYSQL_RES* res = mysql_store_result(conn); MYSQL_ROW row;
    int count = 0;
    while ((row = mysql_fetch_row(res)) && count < max_size) {
        arr[count].id_request = row[0] ? stoi(row[0]) : 0;
        arr[count].id_user = row[1] ? stoi(row[1]) : 0;
        arr[count].nama_makanan_req = row[2] ? row[2] : "";
        arr[count].status_request = row[3] ? row[3] : "";
        count++;
    }
    mysql_free_result(res); return count;
}

inline void insertionSortRequestFIFO(DataRequest arr[], int n) {
    for (int i = 1; i < n; i++) {
        DataRequest key = arr[i]; int j = i - 1;
        while (j >= 0 && arr[j].id_request > key.id_request) { 
            arr[j + 1] = arr[j]; j = j - 1;
        }
        arr[j + 1] = key;
    }
}

inline void menuSortingAdmin(MYSQL* conn) {
    string pilihan;

    while (true) {
        system("cls");
        cout << "\n====== SORTING DATA ADMIN ======\n\n";

        Table sortMenu;
        sortMenu.add_row({"No", "Menu Sorting"});
        sortMenu.add_row({"1", "Sorting Data Makanan Berdasarkan Alfabet Secara Ascending (A-Z)"});
        sortMenu.add_row({"2", "Sorting Data Makanan Berdasarkan Kalori (Tertinggi-Terendah)"});
        sortMenu.add_row({"3", "Sorting Request User"});
        sortMenu.add_row({"0", "Kembali ke menu sebelumnya"});

        sortMenu[0].format().font_align(FontAlign::center);
        for (size_t i = 1; i <= 4; ++i) {
            sortMenu[i][0].format().font_align(FontAlign::center);
        }
        
        cout << sortMenu << endl;
        
        try {
            cout << "Pilih opsi: ";
            getline(cin, pilihan);

            if (pilihan.empty()) throw invalid_argument("Input tidak boleh kosong!");
            if (pilihan.find_first_not_of("0123456789") != string::npos) throw invalid_argument("Input harus berupa angka!");
            if (pilihan != "0" && pilihan != "1" && pilihan != "2" && pilihan != "3") {
                throw invalid_argument("Pilihan tidak valid! Silakan masukkan angka 0 - 3.");
            }

            if (pilihan == "0") {
                return;
            } 
            else if (pilihan == "1" || pilihan == "2") {
                DataMakanan arr[100];
                int n = fetchMakananToArray(conn, arr, 100);
                if (n == 0) { 
                    cout << "Tidak ada data makanan.\n"; 
                } else {
                    if (pilihan == "1") {
                        system("cls");
                        quickSortNama(arr, 0, n - 1);
                        cout << "\n======  Hasil Sorting Data Makanan (A-Z)  ======\n"; 
                        tabelMakanan(arr, n);
                    } else {
                        system("cls");
                        quickSortKalori(arr, 0, n - 1);
                        cout << "\n======  Hasil Sorting Kalori Tertinggi-Terendah  ======\n"; 
                        tabelMakanan(arr, n);
                    }
                }
                cout << "\n\033[1;32mTekan enter untuk kembali ke menu sorting...\033[0m";
                cin.get();
            } 
            else if (pilihan == "3") {
                DataRequest arrReq[100];
                int n = fetchRequestToArray(conn, arrReq, 100);
                if (n == 0) { 
                    cout << "Tidak ada request user.\n"; 
                } else {
                    insertionSortRequestFIFO(arrReq, n);
                    system("cls");
                    cout << "\n======  Hasil Sorting Request User (FIFO)  ======\n";
                    Table tbl; 
                    tbl.add_row({"ID Req", "ID User", "Nama Makanan", "Status"});
                    for (int i = 0; i < n; i++) {
                        tbl.add_row({to_string(arrReq[i].id_request), to_string(arrReq[i].id_user), arrReq[i].nama_makanan_req, arrReq[i].status_request});
                    }
                    cout << tbl << endl;
                }
                cout << "\n\033[1;32mTekan enter untuk kembali ke menu sorting...\033[0m";
                cin.get();
            }

        } catch (const invalid_argument& e) {
            cout << "\033[1;31m[ERROR] " << e.what() << "\nTekan enter untuk mengulang...\033[0m";
            cin.get();
        }
    }
}

inline void searchingMenu(MYSQL* conn) {
    string sub;
    
    while (true) {
        system("cls");
        cout << "\n====== MENU SEARCHING ======\n\n";
        
        Table searchMenu;
        searchMenu.add_row({"No", "Menu Searching"});
        searchMenu.add_row({"1", "Cari Nama Bahan Makanan"});
        searchMenu.add_row({"2", "Cari Nama Akun"});
        searchMenu.add_row({"3", "Cari Log Aktivitas User"});
        searchMenu.add_row({"0", "Kembali ke menu sebelumnya"});

        searchMenu[0].format().font_align(FontAlign::center);
        for (size_t i = 1; i <= 4; ++i) {
            searchMenu[i][0].format().font_align(FontAlign::center);
        }
        
        cout << searchMenu << endl;
        
        try {
            cout << "Pilih opsi: ";
            getline(cin, sub);

            // Validasi Input
            if (sub.empty()) throw invalid_argument("Input tidak boleh kosong!");
            if (sub.find_first_not_of("0123456789") != string::npos) throw invalid_argument("Input harus berupa angka!");
            if (sub != "0" && sub != "1" && sub != "2" && sub != "3") {
                throw invalid_argument("Pilihan tidak valid! Silakan masukkan angka 0 - 3.");
            }

            if (sub == "0") {
                return; 
            } 
            else if (sub == "1") {
                DataMakanan arr[100];
                int n = fetchMakananToArray(conn, arr, 100);
                if (n == 0) { 
                    cout << "Belum ada data makanan.\n"; 
                } else {
                    system("cls");
                    cout << "\nMasukkan nama bahan makanan: "; 
                    string cari; 
                    getline(cin, cari);

                    quickSortNama(arr, 0, n - 1); 

                    int index = binarySearchNama(arr, 0, n - 1, toLower(cari));

                    system("cls");
                    if (index != -1) {

                        DataMakanan hasil[1] = {arr[index]};
                        tabelMakanan(hasil, 1);
                    } else { 
                        cout << "\nBahan '" << cari << "' tidak ditemukan.\n"; 
                    }
                }
                cout << "\n\033[1;32mTekan enter untuk kembali ke menu searching...\033[0m";
                cin.get();
            } 
            else if (sub == "2") {
                cout << "Masukkan nama akun: "; 
                string cari; 
                getline(cin, cari);

                string q = "SELECT id, username, role FROM users WHERE LOWER(username) = LOWER('" + cari + "')";
                mysql_query(conn, q.c_str()); 
                MYSQL_RES* res = mysql_store_result(conn);
                
                if (mysql_num_rows(res) > 0) {

                    system("cls");
                    Table tblAkun;
                    tblAkun.add_row({"ID Akun", "Username", "Role"});

                    tblAkun[0].format().font_align(FontAlign::center);

                    MYSQL_ROW row;
                    while ((row = mysql_fetch_row(res))) {
                        tblAkun.add_row({row[0], row[1], row[2]});
                    }

                    for (size_t i = 1; i <= mysql_num_rows(res); ++i) {
                        tblAkun[i][0].format().font_align(FontAlign::center);
                    }

                    cout << tblAkun << endl;
                } else { 
                    cout << "\nAkun dengan nama '" << cari << "' tidak ditemukan.\n"; 
                }
                
                mysql_free_result(res);
                cout << "\n\033[1;32mTekan enter untuk kembali ke menu searching...\033[0m";
                cin.get();
            } 
            else if (sub == "3") {
                cout << "Masukkan kata kunci log: "; 
                string cari; 
                getline(cin, cari);
            
                string q = "SELECT id_user, username, aktivitas FROM log_user WHERE LOWER(aktivitas) LIKE LOWER('%" + cari + "%')";
                mysql_query(conn, q.c_str()); 
                MYSQL_RES* res = mysql_store_result(conn);
                
                if (mysql_num_rows(res) > 0) {
                    Table tbl; 
                    system("cls");
                    tbl.add_row({"ID User", "Username", "Aktivitas"}); 
                    MYSQL_ROW row;
                    while((row = mysql_fetch_row(res))) {
                        tbl.add_row({row[0], row[1], row[2]});
                    }
                    cout << "\n" << tbl << endl;
                } else { 
                    cout << "Log tidak ditemukan.\n"; 
                }
                
                mysql_free_result(res);
                cout << "\n\033[1;32mTekan enter untuk kembali ke menu searching...\033[0m";
                cin.get();
            }

        } catch (const invalid_argument& e) {
            cout << "\033[1;31m[ERROR] " << e.what() << "\nTekan enter untuk mengulang...\033[0m";
            cin.get();
        }
    }
}

inline void manajemen_rekomendasi(MYSQL* conn) {
    string pil;
    while (true) {
        system("cls");
        cout << "\n====== MANAJEMEN REKOMENDASI DIET ======\n\n";

        Table menuTbl;
        menuTbl.add_row({"No", "Fitur Manajemen Rekomendasi"});
        menuTbl.add_row({"1", "Lihat Semua Rekomendasi (Read)"});
        menuTbl.add_row({"2", "Tambah Rekomendasi Baru (Create)"});
        menuTbl.add_row({"3", "Update Rekomendasi (Update)"});
        menuTbl.add_row({"4", "Hapus Rekomendasi (Delete)"});
        menuTbl.add_row({"0", "Kembali ke Menu Admin"});

        menuTbl[0].format().font_align(FontAlign::center);
        for (size_t i = 1; i <= 5; ++i) menuTbl[i][0].format().font_align(FontAlign::center);

        cout << menuTbl << endl;

        try {
            cout << "Pilih menu: ";
            getline(cin, pil);

            if (pil.empty()) throw invalid_argument("Input tidak boleh kosong!");
            if (pil.find_first_not_of("0123456789") != string::npos) throw invalid_argument("Input harus berupa angka!");

            if (pil == "0") break;

            if (pil == "1") {
                system("cls");
                cout << "\n====== DAFTAR REKOMENDASI DIET BERDASARKAN BMI ======\n\n";
                mysql_query(conn, "SELECT id_rekomendasi, kategori_bmi, saran_diet FROM manajemen_rekomendasi");
                MYSQL_RES* res = mysql_store_result(conn);

                if (mysql_num_rows(res) == 0) {
                    cout << "Belum ada data rekomendasi.\n";
                } else {
                    Table dataTbl;
                    dataTbl.add_row({"ID", "Kategori BMI", "Saran Diet"});
                    dataTbl[0].format().font_align(FontAlign::center);

                    MYSQL_ROW row;
                    while ((row = mysql_fetch_row(res))) {
                        dataTbl.add_row({row[0], row[1], row[2]});
                    }
                    dataTbl.column(2).format().width(60); 
                    cout << dataTbl << endl;
                }
                mysql_free_result(res);
                cout << "\n\033[1;32mTekan enter untuk kembali...\033[0m";
                cin.get();

            } else if (pil == "2") {
                string kategori, saran;
                
                while (true) {
                    try {
                        cout << "\nKategori BMI (Contoh: Underweight, Obesitas): ";
                        getline(cin, kategori);
                        
                        if (kategori.empty()) throw invalid_argument("Kategori tidak boleh kosong!");
                        if (kategori.find_first_of("0123456789") != string::npos) {
                            throw invalid_argument("Kategori tidak boleh mengandung angka!");
                        }
                        break; 
                    } catch (const invalid_argument& e) {
                        cout << "[ERROR] " << e.what() << "\n";
                    }
                }

                while (true) {
                    try {
                        cout << "Masukkan Saran Diet: ";
                        getline(cin, saran);
                        
                        if (saran.empty()) throw invalid_argument("Saran diet tidak boleh kosong!");
                        break;
                    } catch (const invalid_argument& e) {
                        cout << "[ERROR] " << e.what() << "\n";
                    }
                }

                string qInsert = "INSERT INTO manajemen_rekomendasi (kategori_bmi, saran_diet) VALUES ('" + 
                                 kategori + "', '" + saran + "')";
                
                if (mysql_query(conn, qInsert.c_str())) cout << "Gagal: " << mysql_error(conn) << endl;
                else cout << "\n[BERHASIL] Rekomendasi baru ditambahkan.\n";
                
                cout << "\033[1;32mTekan enter untuk kembali...\033[0m";
                cin.get();

            } else if (pil == "3") {
                system("cls");
                mysql_query(conn, "SELECT id_rekomendasi, kategori_bmi, saran_diet FROM manajemen_rekomendasi");
                MYSQL_RES* res = mysql_store_result(conn);
                Table tblUpd;
                tblUpd.add_row({"ID", "Kategori BMI", "Saran Diet"});
                tblUpd[0].format().font_align(FontAlign::center);
                MYSQL_ROW row;
                while ((row = mysql_fetch_row(res))) tblUpd.add_row({row[0], row[1], row[2]});
                mysql_free_result(res);
                tblUpd.column(2).format().width(60);
                cout << tblUpd << endl;

                string id;
                while (true) {
                    try {
                        cout << "Masukkan ID Rekomendasi yang ingin diupdate (0 = batal): ";
                        getline(cin, id);
                        
                        if (id == "0") break;
                        if (id.empty()) throw invalid_argument("ID tidak boleh kosong!");
                        if (id.find_first_not_of("0123456789") != string::npos) throw invalid_argument("ID harus berupa angka!");
                        
                        string cekQ = "SELECT id_rekomendasi FROM manajemen_rekomendasi WHERE id_rekomendasi = " + id;
                        mysql_query(conn, cekQ.c_str());
                        MYSQL_RES* cekRes = mysql_store_result(conn);
                        if (mysql_num_rows(cekRes) == 0) {
                            mysql_free_result(cekRes);
                            throw invalid_argument("ID tidak ditemukan!");
                        }
                        mysql_free_result(cekRes);
                        break;
                    } catch (const invalid_argument& e) {
                        cout << "[ERROR] " << e.what() << "\n";
                    }
                }

                if (id == "0") 
                continue;

                string kategoriBaru, saranBaru;

                while (true) {
                    try {
                        cout << "\nKategori BMI Baru (Kosongkan/Enter jika tak diubah): ";
                        getline(cin, kategoriBaru);

                        if (!kategoriBaru.empty() && kategoriBaru.find_first_of("0123456789") != string::npos) {
                            throw invalid_argument("Kategori tidak boleh mengandung angka!");
                        }
                        break;
                    } catch (const invalid_argument& e) {
                        cout << "[ERROR] " << e.what() << "\n";
                    }
                }

                while (true) {
                    try {
                        cout << "Masukkan Saran Diet Baru (Kosongkan/Enter jika tak diubah): ";
                        getline(cin, saranBaru);
                        break; 
                    } catch (const invalid_argument& e) {
                        cout << "[ERROR] " << e.what() << "\n";
                    }
                }

                string qUpdate = "UPDATE manajemen_rekomendasi SET ";
                bool isUpdate = false;
                if (!kategoriBaru.empty()) {
                    qUpdate += "kategori_bmi = '" + kategoriBaru + "'";
                    isUpdate = true;
                }
                if (!saranBaru.empty()) {
                    if (isUpdate) qUpdate += ", ";
                    qUpdate += "saran_diet = '" + saranBaru + "'";
                    isUpdate = true;
                }

                if (isUpdate) {
                    qUpdate += " WHERE id_rekomendasi = " + id;
                    if (mysql_query(conn, qUpdate.c_str())) cout << "Gagal: " << mysql_error(conn) << endl;
                    else cout << "\n[BERHASIL] Data telah diupdate.\n";
                } else {
                    cout << "\nTidak ada data yang diubah.\n";
                }
                
                cout << "\033[1;32mTekan enter untuk kembali...\033[0m";
                cin.get();

            } else if (pil == "4") {
                system("cls");
                mysql_query(conn, "SELECT id_rekomendasi, kategori_bmi FROM manajemen_rekomendasi");
                MYSQL_RES* res = mysql_store_result(conn);
                Table tblDel;
                tblDel.add_row({"ID", "Kategori BMI"});
                tblDel[0].format().font_align(FontAlign::center);
                MYSQL_ROW row;
                while ((row = mysql_fetch_row(res))) tblDel.add_row({row[0], row[1]});
                mysql_free_result(res);
                cout << tblDel << endl;

                string id;
                while (true) {
                    try {
                        cout << "Masukkan ID Rekomendasi yang akan dihapus (0 = batal): ";
                        getline(cin, id);
                        
                        if (id == "0") break;
                        if (id.empty()) throw invalid_argument("ID tidak boleh kosong!");
                        if (id.find_first_not_of("0123456789") != string::npos) throw invalid_argument("ID harus berupa angka!");
                        
                        string cekQ = "SELECT id_rekomendasi FROM manajemen_rekomendasi WHERE id_rekomendasi = " + id;
                        mysql_query(conn, cekQ.c_str());
                        MYSQL_RES* cekRes = mysql_store_result(conn);
                        if (mysql_num_rows(cekRes) == 0) {
                            mysql_free_result(cekRes);
                            throw invalid_argument("ID tidak ditemukan!");
                        }
                        mysql_free_result(cekRes);
                        break;
                    } catch (const invalid_argument& e) {
                        cout << "[ERROR] " << e.what() << "\n";
                    }
                }

                if (id == "0") continue;

                string qDelete = "DELETE FROM manajemen_rekomendasi WHERE id_rekomendasi = " + id;
                if (mysql_query(conn, qDelete.c_str())) cout << "Gagal: " << mysql_error(conn) << endl;
                else cout << "\n[BERHASIL] Data dihapus.\n";
                
                cout << "\033[1;32mTekan enter untuk kembali...\033[0m";
                cin.get();

            } else {
                throw invalid_argument("Pilihan tidak ada dalam menu");
            }

        } catch (const invalid_argument& e) {
            cout << "\033[1;31m\n[ERROR] " << e.what() << "\nTekan enter untuk mengulang...\033[0m";
            cin.get();
        }
    }
}

inline void menuAdmin(MYSQL* conn) {
    string pilihan;
    while (isTerdaftar && userRole == "admin") {
        system("cls");
        cout << "\n====== Menu Admin ======\n\n";
        Table adminMenu;
        adminMenu.add_row({"No", "Menu"});
        adminMenu.add_row({"1", "Read (Lihat data makanan)"}); 
        adminMenu.add_row({"2", "Create (Tambah data makanan)"});
        adminMenu.add_row({"3", "Update (Update data makanan)"}); 
        adminMenu.add_row({"4", "Delete (Hapus data makanan)"});
        adminMenu.add_row({"5", "Konfirmasi request user"}); 
        adminMenu.add_row({"6", "Log user"});
        adminMenu.add_row({"7", "Menu sorting"}); 
        adminMenu.add_row({"8", "Menu searching"});
        adminMenu.add_row({"9", "Manajemen rekomendasi diet"}); 
        adminMenu.add_row({"0", "Kembali ke menu sebelumnya"});
        adminMenu[0].format().font_align(FontAlign::center);

        for (size_t i = 1; i <= 10; ++i) adminMenu[i][0].format().font_align(FontAlign::center);
        cout << adminMenu << endl;
        
        cout << "Silakan pilih menu admin: ";
        getline(cin, pilihan);
        if(pilihan.empty()) 
        continue; 

        if (pilihan == "1") { 
            readData(conn); 
            cout << "\n\033[1;32mTekan enter untuk melanjutkan...\033[0m"; 
            cin.get(); } 
        else if (pilihan == "2") { 
            createData(conn); 
            cout << "\n\033[1;32mTekan enter untuk melanjutkan...\033[0m"; 
            cin.get(); } 
        else if (pilihan == "3") { 
            editData(conn); 
            cout << "\n\033[1;32mTekan enter untuk melanjutkan...\033[0m"; 
            cin.get(); } 
        else if (pilihan == "4") { 
            deleteData(conn); 
            cout << "\n\033[1;32mTekan enter untuk melanjutkan...\033[0m"; 
            cin.get(); } 
        else if (pilihan == "5") { 
            konfirmasiRequest(conn); 
            cout << "\n\033[1;32mTekan enter untuk melanjutkan...\033[0m"; 
            cin.get(); } 
        else if (pilihan == "6") { 
            readLog(conn); 
            cout << "\n\033[1;32mTekan enter untuk melanjutkan...\033[0m"; 
            cin.get(); } 
        else if (pilihan == "7") { 
            menuSortingAdmin(conn); 
            cout << "\n\033[1;32mTekan enter untuk melanjutkan...\033[0m"; 
            cin.get(); } 
        else if (pilihan == "8") { 
            searchingMenu(conn); 
            cout << "\n\033[1;32mTekan enter untuk melanjutkan...\033[0m"; 
            cin.get(); } 
        else if (pilihan == "9") { 
            manajemen_rekomendasi(conn); 
            cout << "\n\033[1;32mTekan enter untuk melanjutkan...\033[0m"; 
            cin.get(); }
        else if (pilihan == "0") {
            isTerdaftar = false; 
            user = ""; 
            userRole = "";
            cout << "\n\033[1;32mTekan enter untuk melanjutkan...\033[0m"; 
            cin.get(); 
            break;} 
        else { cout << "\033[1;33\nPilihan tidak valid. Silakan tekan enter...\033[0m"; 
            cin.get(); }
    }
}
#endif