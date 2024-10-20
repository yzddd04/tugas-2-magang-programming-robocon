#include <fstream>      // untuk menangani input/output file
#include <iostream>     // untuk input dan output standar
#include <map>          // untuk menggunakan struktur data map
#include <sstream>     // untuk penanganan string stream
#include <vector>       // untuk menggunakan struktur data vector
#include <string>       // untuk menggunakan tipe data string
#include <algorithm>    // untuk fungsi algoritma standar


using namespace std;    // menggunakan namespace std untuk menghindari std:: prefix


// struktur untuk mendefinisikan pengguna
struct User {
    string nrp;         // untuk menyimpan NRP pengguna
    string password;     // untuk menyimpan kata sandi pengguna
    string question;   // untuk menyimpan pertanyaan keamanan
    string answer;      // untuk menyimpan jawaban untuk pertanyaan keamanan
};


// fungsi untuk memisahkan string berdasarkan delimiter
vector<string> pisahkanString(const string& str, char delimiter) {
    vector<string> tokens;    // vektor untuk menyimpan token hasil pemisahan
    string token;            // variabel sementara untuk setiap token
    stringstream ss(str);      // membuat stringstream dari string input
    while (getline(ss, token, delimiter)) {  // membaca string hingga delimiter
        tokens.push_back(token);            // menambahkan token ke vektor
    }
    return tokens;              // mengembalikan vektor token
}


// fungsi untuk menyimpan data pengguna ke file
void simpanKeFile(const User& user) {
    ofstream file("DataUser.txt", ios::app);  // membuka file untuk menambahkan data
    if (file.is_open()) {                     // memeriksa apakah file berhasil dibuka
        file << user.nrp << ","               // menulis NRP ke file
             << user.password << ","          // menulis password ke file
             << user.question << ","        // menulis pertanyaan ke file
             << user.answer << endl;           // menulis jawaban ke file
        file.close();                        // menutup file
    } else {
        cout << "Tidak dapat membuka file!" << endl;  // menampilkan pesan error jika gagal membuka file
    }
}


// fungsi untuk memuat pengguna dari file
map<string, User> muatDariFile() {
    map<string, User> users;        // map untuk menyimpan data pengguna
    ifstream file("DataUser.txt");    // membuka file untuk dibaca
    string line;                    // variabel untuk menyimpan setiap baris dari file
    while (getline(file, line)) {      // membaca file baris per baris
        vector<string> dataUser = pisahkanString(line, ',');  // memisahkan data pengguna berdasarkan koma
        if (dataUser.size() == 4) {    // memeriksa apakah data pengguna valid (memiliki 4 elemen)
            User user = {              // membuat objek User dari data yang telah dipisahkan
                dataUser[0],
                dataUser[1],
                dataUser[2],
                dataUser[3]
            };
            users[dataUser[0]] = user;  // menambahkan user ke map dengan NRP sebagai key
        }
    }
    file.close();                  // menutup file
    return users;                    // mengembalikan map berisi data pengguna
}




// fungsi untuk memeriksa apakah NRP sudah terdaftar di DataHistory.txt
bool cekNRPSudahTerdaftar(const string& nrp) {
    ifstream file("DataHistory.txt");    // membuka file DataHistory.txt
    string line;                        // variabel untuk menyimpan setiap baris dari file
   
    while (getline(file, line)) {        // membaca file baris per baris
        if (line.find(nrp) != string::npos) {  // jika NRP ditemukan di dalam baris
            return true;                // NRP sudah terdaftar
        }
    }


    return false;                       // NRP belum terdaftar
}


// fungsi untuk mendaftar
void daftar(map<string, User>& users) {
    User user;                          // membuat objek user baru
    cout << "Masukkan NRP (hanya angka): ";  // meminta input NRP dari pengguna
    cin >> user.nrp;                    // menyimpan input NRP


    // memeriksa apakah NRP hanya terdiri dari angka
    if (!all_of(user.nrp.begin(), user.nrp.end(), ::isdigit)) {
        cout << "NRP harus hanya terdiri dari angka!" << endl;  // menampilkan pesan error
        return;                         // keluar dari fungsi
    }


    // memeriksa apakah NRP sudah terdaftar di file DataHistory.txt
    if (cekNRPSudahTerdaftar(user.nrp)) {
        cout << "NRP sudah terdaftar sebelumnya!" << endl;  // menampilkan pesan error
        return;                         // keluar dari fungsi
    }


    // memeriksa apakah NRP sudah terdaftar di users (data yang sudah di-load)
    if (users.find(user.nrp) != users.end()) {
        cout << "NRP sudah terdaftar!" << endl;    // menampilkan pesan error
        return;                         // keluar dari fungsi
    }


    cout << "Masukkan Password: ";      // meminta input password
    cin >> user.password;              // menyimpan input password
    cout << "Masukkan pertanyaan untuk verifikasi (ketika lupa password): ";  // meminta input pertanyaan keamanan
    cin.ignore();                      // mengabaikan newline yang tersisa di buffer
    getline(cin, user.question);       // menyimpan input pertanyaan keamanan
    cout << "Masukkan jawaban untuk pertanyaan verifikasi: ";  // meminta input jawaban keamanan
    getline(cin, user.answer);         // menyimpan input jawaban keamanan


    users[user.nrp] = user;            // menambahkan user ke map
    simpanKeFile(user);               // menyimpan user ke file
    cout << "Pendaftaran berhasil!" << endl;  // menampilkan pesan sukses
}


// fungsi untuk login
bool login(const map<string, User>& users, string& nrpTerlogin) {
    string nrp, password;             // variabel untuk menyimpan input NRP dan password
    cout << "Masukkan NRP: ";          // meminta input NRP
    cin >> nrp;                       // menyimpan input NRP
    cout << "Masukkan Password: ";      // meminta input password
    cin >> password;                   // menyimpan input password


    if (users.find(nrp) != users.end() && users.at(nrp).password == password) {  // memeriksa apakah NRP ada dan password cocok
        nrpTerlogin = nrp;            // menyimpan NRP yang berhasil login
        cout << "Login berhasil!" << endl;  // menampilkan pesan sukses
        return true;                  // mengembalikan nilai true (login berhasil)
    } else {
        cout << "NRP atau Password salah!" << endl;  // menampilkan pesan error
        return false;                 // mengembalikan nilai false (login gagal)
    }
}


// fungsi untuk mereset password
void lupaPassword(map<string, User>& users) {
    string nrp;                        // variabel untuk menyimpan input NRP
    cout << "Masukkan NRP: ";          // meminta input NRP
    cin >> nrp;                       // menyimpan input NRP


    if (users.find(nrp) != users.end()) {  // memeriksa apakah NRP terdaftar
        User& user = users[nrp];        // mengambil referensi user dari map
        string jawaban;                // variabel untuk menyimpan input jawaban keamanan
        cout << "Pertanyaan verifikasi: " << user.question << endl;  // menampilkan pertanyaan keamanan
        cout << "Masukkan jawaban verifikasi: ";  // meminta input jawaban keamanan
        cin.ignore();                  // mengabaikan newline yang tersisa di buffer
        getline(cin, jawaban);         // menyimpan input jawaban keamanan


        if (jawaban == user.answer) {    // memeriksa apakah jawaban cocok
            cout << "Jawaban benar. Silakan masukkan password baru: ";  // meminta input password baru
            cin >> user.password;      // menyimpan input password baru
            cout << "Password berhasil diubah!" << endl;  // menampilkan pesan sukses


            ofstream file("DataUser.txt");  // membuka file untuk menulis data yang diperbarui
            if (file.is_open()) {         // memeriksa apakah file berhasil dibuka
                for (const auto& pair : users) {  // iterasi melalui semua pengguna di map
                    file << pair.second.nrp << ","  // menulis NRP ke file
                         << pair.second.password << ","  // menulis password ke file
                         << pair.second.question << ","  // menulis pertanyaan ke file
                         << pair.second.answer << endl;  // menulis jawaban ke file
                }
                file.close();            // menutup file
            }
        } else {
            cout << "Jawaban salah!" << endl;  // menampilkan pesan error
        }
    } else {
        cout << "NRP tidak ditemukan!" << endl;  // menampilkan pesan error
    }
}
