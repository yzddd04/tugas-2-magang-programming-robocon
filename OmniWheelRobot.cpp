#include <iostream> // Mengimpor pustaka untuk input/output standar
#include <cmath> // Mengimpor pustaka untuk fungsi matematika
#include <iomanip> // Mengimpor pustaka untuk manipulasi format output
#include <fstream> // Mengimpor pustaka untuk manipulasi file
#include <ctime> // Mengimpor pustaka untuk pengelolaan waktu

using namespace std; // Menggunakan ruang nama standar

const float PI = 3.14159265358979f; // Konstanta untuk nilai PI
const float MAX_RAD_PER_SEC = 10.0f; // Batas kecepatan rad/s maksimum untuk konversi ke PWM
const int MAX_PWM = 255; // Nilai PWM maksimum

// Kelas untuk robot omniwheel
class OmniWheelRobot {
public:
    // Konstruktor untuk menetapkan diameter roda
    OmniWheelRobot(float diameterRoda) : diameterRoda_(diameterRoda) {}

    // Hitung kecepatan roda dan arah gerak robot
    void hitungKecepatanRoda(float targetX, float targetY, float targetRotasiDerajat, float targetWaktu) {
        float targetRotasiRadian = targetRotasiDerajat * PI / 180.0f; // Konversi rotasi derajat ke radian
        // Hitung kecepatan untuk masing-masing roda
        kecepatanRoda_[0] = (targetY + targetX + targetRotasiRadian * diameterRoda_ / 2.0f) / targetWaktu; 
        kecepatanRoda_[1] = (targetY - targetX + targetRotasiRadian * diameterRoda_ / 2.0f) / targetWaktu;
        kecepatanRoda_[2] = (targetY - targetX - targetRotasiRadian * diameterRoda_ / 2.0f) / targetWaktu;
        kecepatanRoda_[3] = (targetY + targetX - targetRotasiRadian * diameterRoda_ / 2.0f) / targetWaktu;

        // Konversi kecepatan ke rad/s dan RPM
        for (int i = 0; i < 4; i++) {
            kecepatanRodaRad_[i] = kecepatanRoda_[i] / (diameterRoda_ / 2.0f); // Hitung kecepatan roda dalam rad/s
            kecepatanRodaRPM_[i] = kecepatanRodaRad_[i] * 60.0f / (2.0f * PI); // Hitung kecepatan roda dalam RPM
            // Konversi ke nilai PWM
            pwm_[i] = konversiKePWM(kecepatanRodaRad_[i]); // Hitung nilai PWM untuk setiap roda
        }

        // Tentukan arah gerak berdasarkan target X dan Y
        tentukanArah(targetX, targetY); // Panggil fungsi untuk menentukan arah gerak
        rotasiRobotDerajat_ = targetRotasiRadian * 180.0f / PI; // Konversi rotasi radian ke derajat
    }

    // Tampilkan hasil kecepatan dan arah gerak ke layar
    void tampilkanOutput() {
        cout << fixed << setprecision(2); // Atur format output angka dengan 2 desimal
        cout << "\nKecepatan roda dalam rad/s:\n"; // Tampilkan header
        for (int i = 0; i < 4; i++) {
            cout << "Roda " << i + 1 << ": " << kecepatanRodaRad_[i] << " rad/s, PWM: " << pwm_[i] << endl; // Tampilkan kecepatan dan PWM untuk setiap roda
        }
        cout << "\nKecepatan roda dalam RPM:\n"; // Tampilkan header
        for (int i = 0; i < 4; i++) {
            cout << "Roda " << i + 1 << ": " << kecepatanRodaRPM_[i] << " RPM\n"; // Tampilkan RPM untuk setiap roda
        }
        cout << "\nArah gerak robot: " << arah_ << "\n"; // Tampilkan arah gerak robot
        cout << "Perputaran robot: " << rotasiRobotDerajat_ << " derajat\n"; // Tampilkan rotasi robot
    }

    // Dapatkan nomor entri terakhir dari file
    int dapatkanNomorEntriTerakhir() {
        ifstream file("output.txt"); // Buka file output.txt
        string line; // Variabel untuk menyimpan baris yang dibaca
        int nomorEntriTerakhir = 0; // Inisialisasi nomor entri terakhir

        // Baca file baris per baris untuk menemukan nomor urutan terakhir
        while (getline(file, line)) { // Baca setiap baris dalam file
            if (line.find("==================================") != string::npos) { // Jika menemukan pemisah entri
                nomorEntriTerakhir++; // Tambah nomor entri terakhir
            }
        }

        file.close(); // Tutup file
        return nomorEntriTerakhir; // Kembalikan nomor entri terakhir
    }

    // Simpan hasil ke file dengan timestamp dan pemisah
    void simpanKeFile(float targetX, float targetY, float targetRotasiDerajat, float targetWaktu) {
        int nomorEntriTerakhir = dapatkanNomorEntriTerakhir(); // Ambil urutan terakhir
        int nomorEntriSaatIni = nomorEntriTerakhir + 1; // Tambahkan 1 ke nomor urutan

        ofstream file("output.txt", ios::app); // Buka file untuk menambahkan data
        time_t sekarang = time(0); // Dapatkan waktu saat ini
        char* dt = ctime(&sekarang); // Konversi waktu ke format string

        // Tulis data ke file
        file << "==================================\n"; // Tulis pemisah entri
        file << "Data ke - " << nomorEntriSaatIni << "\n"; // Tulis nomor entri saat ini
        file << "Waktu simpan: " << dt; // Tulis waktu simpan
        file << "Input: X=" << targetX << "m, Y=" << targetY << "m, Rotasi=" << targetRotasiDerajat << "°, Waktu=" << targetWaktu << "s\n"; // Tulis input yang diterima
        for (int i = 0; i < 4; i++) {
            file << "Roda " << i + 1 << ": " << kecepatanRodaRad_[i] << " rad/s, " << kecepatanRodaRPM_[i] << " RPM, PWM: " << pwm_[i] << "\n"; // Tulis kecepatan dan PWM untuk setiap roda
        }
        file << "Arah gerak: " << arah_ << "\n"; // Tulis arah gerak
        file << "Perputaran: " << rotasiRobotDerajat_ << " derajat\n"; // Tulis rotasi robot
        file.close(); // Tutup file
        cout << "File berhasil disimpan!\n\n"; // Tampilkan pesan bahwa file telah disimpan
    }

private:
    float diameterRoda_; // Diameter roda
    float kecepatanRoda_[4], kecepatanRodaRad_[4], kecepatanRodaRPM_[4], rotasiRobotDerajat_; // Variabel untuk menyimpan kecepatan roda dan rotasi
    int pwm_[4]; // Variabel untuk menyimpan nilai PWM untuk setiap roda
    string arah_; // Arah gerak robot

    // Tentukan arah gerak berdasarkan target X dan Y
    void tentukanArah(float targetX, float targetY) {
        if (targetX > 0 && targetY == 0) arah_ = "Kanan"; // Jika hanya ke kanan
        else if (targetX < 0 && targetY == 0) arah_ = "Kiri"; // Jika hanya ke kiri
        else if (targetY > 0 && targetX == 0) arah_ = "Maju"; // Jika hanya maju
        else if (targetY < 0 && targetX == 0) arah_ = "Mundur"; // Jika hanya mundur
        else if (targetX > 0 && targetY > 0) arah_ = "Diagonal kanan atas"; // Jika bergerak diagonal kanan atas
        else if (targetX < 0 && targetY > 0) arah_ = "Diagonal kiri atas"; // Jika bergerak diagonal kiri atas
        else if (targetX < 0 && targetY < 0) arah_ = "Diagonal kiri bawah"; // Jika bergerak diagonal kiri bawah
        else if (targetX > 0 && targetY < 0) arah_ = "Diagonal kanan bawah"; // Jika bergerak diagonal kanan bawah
        else arah_ = "Berputar di tempat"; // Jika tidak bergerak
    }

    // Konversi kecepatan rad/s ke nilai PWM
    int konversiKePWM(float kecepatanRadPerDetik) {
        // Skala kecepatan dari rad/s ke nilai PWM dengan rentang -255 sampai 255
        float skalaPWM = kecepatanRadPerDetik / MAX_RAD_PER_SEC; // Skala kecepatan
        int nilaiPWM = static_cast<int>(skalaPWM * MAX_PWM); // Hitung nilai PWM

        // Pastikan nilai PWM dalam rentang yang valid
        if (nilaiPWM > MAX_PWM) nilaiPWM = MAX_PWM; // Jika lebih dari maksimum, set ke maksimum
        else if (nilaiPWM < -MAX_PWM) nilaiPWM = -MAX_PWM; // Jika kurang dari minimum, set ke minimum

        return nilaiPWM; // Kembalikan nilai PWM
    }
};
