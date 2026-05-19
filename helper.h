#ifndef HELPER_H
#define HELPER_H

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <mysql.h>
#include "tabulate/table.hpp"
#include <sstream>

using namespace std;
using namespace tabulate;

struct DataMakanan {
    int id;
    string nama;
    string kategori;
    float kalori;
    float protein;
    float karbohidrat;
    float lemak;
};

inline string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
        return tolower(c);
    });
    return s;
}

inline void swapData(DataMakanan* a, DataMakanan* b) {
    DataMakanan temp = *a;
    *a = *b;
    *b = temp;
}

inline string formatFloat(float val) {
    stringstream ss;
    ss << val;
    return ss.str();
}

inline int fetchMakananToArray(MYSQL* conn, DataMakanan arr[], int max_size) {
    string query = "SELECT * FROM makanan";
    if (mysql_query(conn, query.c_str())) return 0;

    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;
    int count = 0;

    while ((row = mysql_fetch_row(res)) && count < max_size) {
        arr[count].id = (row[0] && string(row[0]) != "") ? stoi(row[0]) : 0;
        arr[count].nama = row[1] ? row[1] : "-";
        arr[count].kategori = row[2] ? row[2] : "-";
        arr[count].kalori = (row[3] && string(row[3]) != "") ? stof(row[3]) : 0.0f;
        arr[count].protein = (row[4] && string(row[4]) != "") ? stof(row[4]) : 0.0f;
        arr[count].karbohidrat = (row[5] && string(row[5]) != "") ? stof(row[5]) : 0.0f;
        arr[count].lemak = (row[6] && string(row[6]) != "") ? stof(row[6]) : 0.0f;
        count++;
    }
    mysql_free_result(res);
    return count;
}

inline void tabelMakanan(DataMakanan arr[], int n) {
    if (n == 0) {
        cout << "Tidak ada data makanan.\n";
        return;
    }
    Table tbl;
    tbl.add_row({"No", "ID", "Nama Makanan", "Kategori", "Kalori (kcal)", "Protein (g)", "Karbo (g)", "Lemak (g)"});
    tbl.row(0).format().font_align(FontAlign::center);

    int no = 1; 
    for (int i = 0; i < n; i++) {
        tbl.add_row({
            to_string(no++), to_string(arr[i].id), arr[i].nama, arr[i].kategori,
            formatFloat(arr[i].kalori), formatFloat(arr[i].protein),
            formatFloat(arr[i].karbohidrat), formatFloat(arr[i].lemak)
        });
    }

    for (size_t i = 1; i <= (size_t)n; ++i) {
        for(size_t j=0; j<8; ++j) {
            if(j != 2 && j != 3) tbl[i][j].format().font_align(FontAlign::center);
        }
    }
    cout << tbl << endl;
}

inline int partisiKategori(DataMakanan arr[], int low, int high) {
    string pivot = arr[high].kategori;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j].kategori <= pivot) {
            i++; swapData(&arr[i], &arr[j]);
        }
    }
    swapData(&arr[i + 1], &arr[high]);
    return i + 1;
}

inline void quickSortKategori(DataMakanan arr[], int low, int high) {
    if (low < high) {
        int pi = partisiKategori(arr, low, high);
        quickSortKategori(arr, low, pi - 1);
        quickSortKategori(arr, pi + 1, high);
    }
}

inline int partisiKalori(DataMakanan arr[], int low, int high) {
    float pivot = arr[high].kalori;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j].kalori >= pivot) { 
            i++; swapData(&arr[i], &arr[j]);
        }
    }
    swapData(&arr[i + 1], &arr[high]);
    return i + 1;
}

inline void quickSortKalori(DataMakanan arr[], int low, int high) {
    if (low < high) {
        int pi = partisiKalori(arr, low, high);
        quickSortKalori(arr, low, pi - 1);
        quickSortKalori(arr, pi + 1, high);
    }
}

inline int partisiMakro(DataMakanan arr[], int low, int high, int mode) {
    float pivot = (mode == 1) ? arr[high].protein : ((mode == 2) ? arr[high].karbohidrat : arr[high].lemak);
    int i = low - 1;
    for (int j = low; j < high; j++) {
        float comp = (mode == 1) ? arr[j].protein : ((mode == 2) ? arr[j].karbohidrat : arr[j].lemak);
        if (comp >= pivot) { 
            i++; swapData(&arr[i], &arr[j]);
        }
    }
    swapData(&arr[i + 1], &arr[high]);
    return i + 1;
}

inline void quickSortMakro(DataMakanan arr[], int low, int high, int mode) {
    if (low < high) {
        int pi = partisiMakro(arr, low, high, mode);
        quickSortMakro(arr, low, pi - 1, mode);
        quickSortMakro(arr, pi + 1, high, mode);
    }
}

inline int partisiNama(DataMakanan arr[], int low, int high) {
    string pivot = toLower(arr[high].nama); 
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (toLower(arr[j].nama) <= pivot) {
            i++;
            DataMakanan temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    DataMakanan temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    
    return (i + 1);
}

inline void quickSortNama(DataMakanan arr[], int low, int high) {
    if (low < high) {
        int pi = partisiNama(arr, low, high);
        quickSortNama(arr, low, pi - 1);
        quickSortNama(arr, pi + 1, high);
    }
}

inline int binarySearchNama(DataMakanan arr[], int low, int high, string key) {
    string lowerKey = toLower(key); 

    while (low <= high) {
        int mid = low + (high - low) / 2;

        string currentName = toLower(arr[mid].nama); 

        if (currentName == lowerKey) return mid;
        if (currentName < lowerKey) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

inline int binarySearchKalori(DataMakanan arr[], int low, int high, float key) {
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid].kalori == key) return mid;
        if (arr[mid].kalori < key) high = mid - 1; 
        else low = mid + 1;
    }
    return -1;
}

inline void catatLog(MYSQL* conn, int idUser, string aktivitas) {
    if (idUser == 0) 
    return;

    string query = "INSERT INTO log_user (id_user, aktivitas, waktu) VALUES (" + to_string(idUser) + ", '" + aktivitas + "', NOW())";
    if (mysql_query(conn, query.c_str())) {
        cout << "[WARNING] Gagal mencatat log: " << mysql_error(conn) << endl;
    }
}

#endif