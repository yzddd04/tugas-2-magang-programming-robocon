#include "OmniWheelRobot.cpp" // Memasukkan file "OmniWheelRobot.cpp"
#include <fstream> // Memasukkan pustaka fstream untuk operasi file

using namespace std; // Menggunakan namespace std agar tidak perlu menyebut std::

int main() {
    // Deklarasi variabel untuk diameter roda, target X, Y, rotasi, dan waktu
    float diameterRoda, targetX, targetY, targetRotasiDerajat, targetWaktu;
    char opsi; // Variabel untuk menyimpan pilihan opsi

    // Input data awal dari pengguna
    cout << "\nMasukkan diameter roda (cm): "; // Menampilkan pesan untuk input diameter roda
    cin >> diameterRoda; // Menerima input diameter roda dari pengguna
    OmniWheelRobot robot(diameterRoda); // Membuat objek robot dengan diameter roda

    // Perulangan utama program
    while (true) {
        // Input data target X, Y, rotasi, dan waktu dari pengguna
        cout << "Masukkan target X (meter): "; // Pesan input untuk target X
        cin >> targetX; // Menerima input target X
        cout << "Masukkan target Y (meter): "; // Pesan input untuk target Y
        cin >> targetY; // Menerima input target Y
        cout << "Masukkan target perputaran (derajat): "; // Pesan input untuk rotasi (dalam derajat)
        cin >> targetRotasiDerajat; // Menerima input target rotasi
        cout << "Masukkan target waktu (detik): "; // Pesan input untuk target waktu
        cin >> targetWaktu; // Menerima input target waktu

        // Hitung kecepatan roda dan tampilkan hasilnya
        robot.hitungKecepatanRoda(targetX, targetY, targetRotasiDerajat, targetWaktu); // Memanggil fungsi untuk menghitung kecepatan roda
        robot.tampilkanOutput(); // Memanggil fungsi untuk menampilkan hasil perhitungan

        // Menampilkan opsi setelah hasil ditampilkan
        cout << "\nPilihan opsi:\n";
        cout << "1. Simpan input dan hasil ke file\n"; // Opsi untuk menyimpan input dan hasil ke file
        cout << "2. Tampilkan file\n"; // Opsi untuk menampilkan isi file
        cout << "3. Lanjut perhitungan\n"; // Opsi untuk melanjutkan perhitungan
        cout << "4. Selesai\n\n"; // Opsi untuk menyelesaikan program

        cout << "Pilih Opsi : "; // Pesan untuk memilih opsi
        cin >> opsi; // Menerima input pilihan opsi dari pengguna

        // Pengolahan pilihan dari pengguna
        switch (opsi) {
            case '1':
                // Simpan input dan hasil perhitungan ke file
                robot.simpanKeFile(targetX, targetY, targetRotasiDerajat, targetWaktu); // Menyimpan hasil perhitungan ke file
                break;
            case '2': {
                // Buka dan tampilkan isi file
                ifstream file("output.txt"); // Membuka file "output.txt"
                if (file.is_open()) { // Mengecek apakah file berhasil dibuka
                    cout << "\nIsi file:\n"; // Pesan menampilkan isi file
                    string line; // Variabel untuk menyimpan baris dari file
                    while (getline(file, line)) { // Membaca setiap baris dalam file
                        cout << line << endl; // Menampilkan baris file
                    }
                    file.close(); // Menutup file
                    cout << "\nFile berhasil ditampilkan!\n"; // Pesan file berhasil ditampilkan
                } else {
                    // Jika file tidak ditemukan
                    cout << "File tidak ditemukan!\n"; // Pesan jika file tidak ada
                }
                break;
            }
            case '3':
                // Lanjut ke perhitungan berikutnya
                continue; // Melanjutkan perulangan untuk input berikutnya
            case '4':
                // Akhiri program
                cout << "Program selesai.\n"; // Pesan akhir program
                return 0; // Keluar dari program
            default:
                // Jika pilihan tidak valid
                cout << "Opsi tidak valid!\n"; // Pesan jika input opsi salah
        }
    }
    return 0; // Kembalikan nilai 0 sebagai indikasi program selesai dengan sukses
}
