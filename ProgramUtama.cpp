#include <fstream>                         // Mengimpor library untuk menangani file input/output
#include <iostream>                        // Mengimpor library untuk input dan output standar
#include <map>                             // Mengimpor library untuk menggunakan struktur data map (peta)
#include "OmniWheelRobot.cpp"              // Mengimpor file handling OmniWheelRobot.cpp
#include "ProgramAkun.cpp"                 // Mengimpor file handling ProgramAkun.cpp untuk digunakan dalam program

using namespace std;    // Menggunakan namespace std untuk mempersingkat std::

// 1. Fungsi untuk menjalankan program akun
void runProgram() {
    map<string, User> users = muatDariFile(); // Memuat data pengguna dari file ke dalam map users
    string nrpTerlogin;     // Variabel untuk menyimpan NRP pengguna
    bool sudahLogin = false;        // Status login pengguna, awalnya false

    // Proses login
    while (!sudahLogin) { // Selama pengguna belum login, maka looping
        int pilihan; 
        // Variabel pilihan menu
        cout << "\nLogin dulu ya kakak\n1. Daftar akun baru\n2. Login\n3. Lupa Password\n4. Keluar\nPilih: "; // Menampilkan pilihan menu
        cin >> pilihan;                     // Menerima input dari pengguna

        switch (pilihan) {      // Menggunakan statement switch-case untuk menentukan program selanjutnya berdasarkan pilihan user
            case 1:     // Jika user memilih pilihan 1
                daftar(users);      // Memanggil fungsi daftar di file handling Program Akun untuk mendaftar pengguna baru
                break;      // Keluar dari switch
            case 2:     // Jika memilih pilihan 2
                if (login(users, nrpTerlogin)) { // Memaanggil fungsi login di file handling Program AKun dan jika berhasil
                    sudahLogin = true;      // Menandakan login user berhasil
                    cout << "Selamat datang, " << nrpTerlogin << endl; // Menampilkan pesan selamat datang
                }
                break;      // keluar dari switch
            case 3:     // jika memilih pilihan 3
                lupaPassword(users);        // memanggil fungsi lupaPassword di file handling Program Akun untuk mengatur ulang password
                break;      // Keluar dari switch
            case 4:     // Jika pilihan 4
                cout << "Keluar dari program..." << endl; // Menampilkan pesan keluar program
                return;     // Keluar dari fungsi runProgram
            default:        // Jika pilihan tidak valid
                cout << "Pilihan tidak valid!" << endl; // Menampilkan pesan tidak valid jika user menginput selain angka 1-4
        }
    }

    // 2. Jika login berhasil, lanjutkan ke program OmniWheelRobot
    float diameterRoda, targetX, targetY, targetRotasiDerajat, targetWaktu, jarakSumbuDepanBelakang, jarakSumbuKananKiri;         // Variabel untuk menyimpan data yang diperlukan
    char opsi;      // Variabel untuk menyimpan opsi setelah menampilkan hasil
    
    cout << endl ;      // Mencetak baris kosong
    cout << "=============================================" << endl ; 
    cout << "  WELCOME TO THE INVERSE KINEMATIC PROGRAM  " << endl ;  // Menampilkan pesan selamat datang agar terkesan indah
    cout << "=============================================" << endl;  

    cout << "\nMasukkan diameter roda (m)(constant): ";      // Meminta input diameter roda dari pengguna dan angkanya inputnya constant
    cin >> diameterRoda;        // User menginput diameter roda
    // Struktur diameter roda
    OmniWheelRobot robot(diameterRoda);     // Membuat objek robot dari kelas OmniWheelRobot dengan diameter roda yang dimasukkan user

    cout << "Masukkan target X (meter): ";      // Meminta input target X
    cin >> targetX;     // Menerima input target X
    cout << "Masukkan target Y (meter): ";      // Meminta input target Y
    cin >> targetY;     // Menerima input target Y
    cout << "Masukkan target perputaran (derajat) robot:" ;        // Meminta input rotasi robot
    cin >> targetRotasiDerajat;     // Menerima input rotasi robot
    cout << "Masukkan target waktu (detik): ";      // Meminta input waktu
    cin >> targetWaktu;     // Menerima input waktu

    // Struktur hitung kecepatan roda dan tampilkan hasil
    robot.hitungKecepatanRoda(targetX, targetY, targetRotasiDerajat, targetWaktu);      // Menghitung kecepatan roda berdasarkan input
 
    while (true){
    // 3. Menampilkan opsi setelah menampilkan DataHistory
    cout << "\nPilihan opsi:\n";        // Menampilkan pilihan opsi
    cout << "1. Simpan input dan hasil ke file\n";      // Opsi untuk menyimpan ke file
    cout << "2. Tampilkan file\n";      // Opsi untuk menampilkan isi file
    cout << "3. Lanjut perhitungan\n";      // Opsi untuk melanjutkan perhitungan
    cout << "4. Selesai\n\n";       // Opsi untuk menyelesaikan program
    cout << "Pilih Opsi : ";        // Meminta pengguna untuk memilih opsi
    cin >> opsi;        // Menerima input opsi dari pengguna

    switch (opsi) {     // Menggunakan statement switch-case untuk memproses pilihan pengguna
        case '1':       // Jika user memilih opsi 1
            robot.simpanKeFile(targetX, targetY, targetRotasiDerajat, targetWaktu);       // Simpan input dan hasil perhitungan ke file
            continue;;      // Keluar dari switch
        case '2': {     // Jika user memilih opsi 2
            ifstream file("DataHistory.txt");       // Membuka file DataHistory.txt
            if (file.is_open()) {       // Jika file berhasil dibuka
                cout << "\nIsi file:\n";        // Menampilkan teks isi file
                string line;        // Variabel untuk menyimpan setiap baris
                while (getline(file, line)) {       // Membaca file baris per baris di DataHistory.txt
                    cout << line << endl;       // Menampilkan baris yang dibaca
                }
                file.close();       // Menutup file setelah dibaca
                cout << "\nFile berhasil ditampilkan!\n";       // Menampilkan teks pesan berhasil
            } else {        // Jika file tidak ditemukan
                cout << "File tidak ditemukan!\n";      // Menampilkan pesan kesalahan
            }
            // Kembali ke opsi 3 tanpa melanjutkan perhitungan lagi
            continue;      // Loop kembali ke opsi berikutnya (tidak melanjutkan perhitungan)
        }
        case '3':       // Jika user memilih opsi 3
                cout << "Masukkan target X (meter): ";      // Meminta input target X
                cin >> targetX;     // Menerima input target X
                cout << "Masukkan target Y (meter): ";      // Meminta input target Y
                cin >> targetY;     // Menerima input target Y
                cout << "Masukkan target perputaran (derajat) robot:" ;        // Meminta input rotasi robot
                cin >> targetRotasiDerajat;     // Menerima input rotasi robot
                cout << "Masukkan target waktu (detik): ";      // Meminta input waktu
                cin >> targetWaktu;     // Menerima input waktu

                // Struktur hitung kecepatan roda dan tampilkan hasil
                robot.hitungKecepatanRoda(targetX, targetY, targetRotasiDerajat, targetWaktu);      // Menghitung kecepatan roda berdasarkan input
                robot.tampilkanDataHistory();       // Menampilkan riwayat data dari objek robot
            
            continue;       // Kembali ke awal loop untuk input baru & Lanjut ke perhitungan berikutnya
        case '4':       // Jika user memilih opsi 4
            cout << "Program selesai!\n";       // Mengakhiri program dan menampilkan pesan selesai
            return;     // Keluar dari fungsi runProgram
        default:        // Jika input opsi tidak sesuai dengan angka 1-4
            cout << "Opsi tidak valid! Silakan coba lagi.\n";       // Menampilkan pesan kesalahan
            break;      // Keluar dari switch (opsi)
        }
    }
}
