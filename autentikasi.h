#ifndef AUTENTIKASI_H
#define AUTENTIKASI_H

#include <iostream>
#include <mysql.h>
#include <conio.h>
#include <string>
#include "helper.h"

using namespace std;

string user = "";
string userRole = "";
int currentUserId = 0; 
bool isTerdaftar = false;

void header(string judul) { 
    cout << "\n====== " << judul << " ======\n" << endl;
}

string inputPassword(string pesan) {
    string pass = "";
    char ch;
    cout << pesan;
    while ((ch = _getch()) != '\r') { 
        if (ch == '\b') { 
            if (pass.length() > 0) { cout << "\b \b"; pass.pop_back(); }
        } else {
            pass.push_back(ch); cout << '*'; 
        }
    }
    cout << endl;
    return pass;
}

void registerUser(MYSQL* conn) {
    system("cls");
    header("Form Registrasi Sigizi");
    
    string usnBaru, passBaru;
    
    while (true) {
        cout << "Masukkan username : ";
        getline(cin, usnBaru);
        
        if (usnBaru.empty()) {
            cout << "Username tidak boleh kosong\n";
            continue;
        }

        string checkQuery = "SELECT * FROM users WHERE username = '" + usnBaru + "'";
        mysql_query(conn, checkQuery.c_str());
        MYSQL_RES* res = mysql_store_result(conn);
        
        if (mysql_num_rows(res) > 0) {
            cout << "Username sudah digunakan. Silakan coba nama lain.\n";
            mysql_free_result(res);
        } else {
            mysql_free_result(res);
            break; 
        }
    }

    while (true) {
        passBaru = inputPassword("Password : ");
        if (passBaru.empty()) {
            cout << "Password tidak boleh kosong\n";
        } else {
            break;
        }
    }

    string insertQuery = "INSERT INTO users (username, password, role) VALUES ('" + usnBaru + "', '" + passBaru + "', 'user')";
    
    if (mysql_query(conn, insertQuery.c_str())) {
        cout << "\nGagal registrasi " << mysql_error(conn) << endl;
    } else {
        cout << "\n\033[34mRegistrasi berhasil, akun Anda sudah terdaftar sebagai user. Silakan login untuk melanjutkan\033[0m" << endl;
    }
    cout << "\033[1;32mTekan enter untuk kembali...\033[0m";
    cin.get();
}

bool login(MYSQL* conn, int percobaan) { 
    system("cls");
    header("LOGIN SIGIZI");
    
    string usn, pass;
    cout << "Masukkan username : ";
    getline(cin, usn);
    pass = inputPassword("Masukkan password : ");

    string query = "SELECT * FROM users WHERE username = '" + usn + "' AND password = '" + pass + "'";
    
    mysql_query(conn, query.c_str());

    MYSQL_RES* res = mysql_store_result(conn);
    
    if (mysql_num_rows(res) > 0) {
        MYSQL_ROW row = mysql_fetch_row(res);
        
        currentUserId = atoi(row[0]);
        user = usn;
        isTerdaftar = true;
        userRole = row[3] ? row[3] : "user"; 

        cout << "\n\033[34mLogin berhasil, selamat datang di Sistem Gizi \033[0m" << user << endl;
        cout << "\033[1;32mTekan enter untuk melanjutkan...\033[0m";
        cin.get();
        return true;
    }

    mysql_free_result(res);

    if (percobaan - 1 == 0) {
        cout << "\n\033[31mMaaf, kesempatan Anda telah habis\033[0m" << endl;
        cout << "Anda akan otomatis dialihkan ke form Registrasi." << endl;
        cout << "\033[1;32mTekan enter untuk melanjutkan...\033[0m";
        cin.get();
        registerUser(conn);

        return false;
    } 
    cout << "\n\033[31mSayang sekali, login gagal\033[0m" << endl;
    cout << "Sisa percobaan Anda adalah: " << percobaan - 1;
    cout << "\n\033[1;32mTekan enter untuk mencoba lagi...\033[0m";
    cin.get();
    
    return login(conn, percobaan - 1);
}

#endif