
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
using namespace std;


const float PI = 3.14159265358979f;          // Nilai PI
const float MAX_RAD_PER_SEC = 10.0f;         // kecepatan maksimum dalam radian per detik
const int MAX_PWM = 255;                     // nilai PWM maksimum


// Kelas untuk Robot OmniWheel
class OmniWheelRobot {
public:
    // Konstruktor untuk inisialisasi diameter roda
    OmniWheelRobot(float diameterRoda) : diameterRoda_(diameterRoda) {}


    // Fungsi untuk menghitung kecepatan roda dan arah gerak
    void hitungKecepatanRoda(float targetX, float targetY, float targetRotasiDerajat, float targetWaktu) {
        float targetRotasiRadian = targetRotasiDerajat * PI / 180.0f; // mengubah derajat ke radian


        // Menghitung kecepatan roda (persamaan kinematika omniwheel)
        kecepatanRoda_[0] = (targetY + targetX + targetRotasiRadian * diameterRoda_ / 2.0f) / targetWaktu;
        kecepatanRoda_[1] = (targetY - targetX + targetRotasiRadian * diameterRoda_ / 2.0f) / targetWaktu;
        kecepatanRoda_[2] = (targetY - targetX - targetRotasiRadian * diameterRoda_ / 2.0f) / targetWaktu;
        kecepatanRoda_[3] = (targetY + targetX - targetRotasiRadian * diameterRoda_ / 2.0f) / targetWaktu;


        // Mengubah kecepatan roda ke rad/s dan kemudian ke nilai PWM
        for (int i = 0; i < 4; i++) {
            kecepatanRodaRad_[i] = kecepatanRoda_[i] / (diameterRoda_ / 2.0f);  // Menghitung rad/s
            kecepatanRodaRPM_[i] = konversiKeRPM(kecepatanRodaRad_[i]);         // Menghitung RPM
   
            // Memperbarui nilai PWM berdasarkan arah gerakan (nilai negatif)
            pwm_[i] = (kecepatanRodaRPM_[i] >= 0) ? abs(kecepatanRodaRPM_[i]) : -abs(kecepatanRodaRPM_[i]);  // Menggunakan tanda dari RPM untuk mengatur PWM
        }


        // Menentukan arah gerakan robot
        tentukanArah(targetX, targetY);
        rotasiRobotDerajat_ = targetRotasiRadian * 180.0f / PI;  // Mengubah rotasi ke derajat


        // Menampilkan data kontrol motor
        tampilkanDataHistory();
    }


    // Menampilkan hasil perhitungan
    void tampilkanDataHistory() {
        cout << fixed << setprecision(2);  // Mengatur presisi output


        // Motor 1
        tampilkanMotor(0, "Motor 1");


        // Motor 2
        tampilkanMotor(1, "Motor 2");


        // Motor 3
        tampilkanMotor(2, "Motor 3");


        // Motor 4
        tampilkanMotor(3, "Motor 4");


        // Menampilkan arah gerak dan rotasi robot
        cout << "Arah gerak robot: " << arah_ << "\n";
        cout << "Perputaran robot: " << rotasiRobotDerajat_ << " derajat\n";
    }


    // Menyimpan data ke file
    void simpanKeFile(float targetX, float targetY, float targetRotasiDerajat, float targetWaktu) {
        ofstream file("DataHistory.txt", ios::app); // Membuka file dalam mode append
        if (file.is_open()) {
            // Menyimpan data input
            file << "======================================\n";
            file << "Data Input:\n";
            file << "Target X: " << targetX << " meter\n";
            file << "Target Y: " << targetY << " meter\n";
            file << "Target Rotasi: " << targetRotasiDerajat << " derajat\n";
            file << "Target Waktu: " << targetWaktu << " detik\n\n";


            // Menyimpan hasil perhitungan kecepatan roda dan PWM
            file << "Hasil Perhitungan:\n";
            for (int i = 0; i < 4; i++) {
                file << "Motor " << i + 1 << endl;
                file << "- Kecepatan rad: " << kecepatanRodaRad_[i] << " rad/s\n";
                file << "- Kecepatan PWM: " << kecepatanRodaRPM_[i] << " RPM\n";
                file << "- Value PWM: " << pwm_[i] << "\n\n";
            }


            // Menyimpan arah gerak dan rotasi
            file << "Arah gerak robot: " << arah_ << "\n";
            file << "Perputaran robot: " << rotasiRobotDerajat_ << " derajat\n\n";


            file.close();  // Menutup file
            cout << "Data berhasil disimpan ke file DataHistory.txt\n";
        } else {
            cout << "Gagal membuka file!\n";
        }
    }


private:
    float diameterRoda_;                     // Diameter roda
    float kecepatanRoda_[4], kecepatanRodaRad_[4];  // Kecepatan roda dan rotasi dalam rad/s
    float kecepatanRodaRPM_[4];              // Kecepatan roda dalam RPM
    int pwm_[4];                             // Nilai PWM untuk setiap roda
    string arah_;                            // Arah gerak robot
    float rotasiRobotDerajat_;               // Rotasi robot dalam derajat


    // Fungsi untuk menentukan arah gerakan berdasarkan target X dan Y
    void tentukanArah(float targetX, float targetY) {
        if (targetX > 0 && targetY == 0) arah_ = "Kanan";
        else if (targetX < 0 && targetY == 0) arah_ = "Kiri";
        else if (targetY > 0 && targetX == 0) arah_ = "Maju";
        else if (targetY < 0 && targetX == 0) arah_ = "Mundur";
        else if (targetX > 0 && targetY > 0) arah_ = "Diagonal kanan atas";
        else if (targetX < 0 && targetY > 0) arah_ = "Diagonal kiri atas";
        else if (targetX < 0 && targetY < 0) arah_ = "Diagonal kiri bawah";
        else if (targetX > 0 && targetY < 0) arah_ = "Diagonal kanan bawah";
        else arah_ = "Berputar di tempat";
    }


    // Mengubah rad/s ke RPM (rotasi per menit)
    float konversiKeRPM(float kecepatanRadPerDetik) {
        float skalaRPM = kecepatanRadPerDetik * 60.0f / (2.0f * PI);  // Mengubah rad/s ke RPM
        return skalaRPM;
    }


    // Menampilkan kontrol motor berdasarkan nilai PWM
    void tampilkanMotor(int motorIndex, string motorName) {
        cout << motorName << "\n";
        if (pwm_[motorIndex] > 0) {  // jika PWM positif (bergerak maju)
            cout << "- Direksi Arduino: \n";
            cout << "  digitalWrite(DIR" << motorIndex * 2 << "_PIN, LOW);\n";
            cout << "  digitalWrite(DIR" << motorIndex * 2 + 1 << "_PIN, HIGH);\n";
            cout << "- Direksi STM32:\n";
            cout << "  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_" << motorIndex * 2 << ", GPIO_PIN_SET);\n";
            cout << "  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_" << motorIndex * 2 + 1 << ", GPIO_PIN_RESET);\n";
        } else if (pwm_[motorIndex] < 0) {  // jika PWM negatif (bergerak mundur)
            cout << "- Direksi Arduino: \n";
            cout << "  digitalWrite(DIR" << motorIndex * 2 << "_PIN, HIGH);\n";
            cout << "  digitalWrite(DIR" << motorIndex * 2 + 1 << "_PIN, LOW);\n";
            cout << "- Direksi STM32:\n";
            cout << "  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_" << motorIndex * 2 << ", GPIO_PIN_RESET);\n";
            cout << "  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_" << motorIndex * 2 + 1 << ", GPIO_PIN_SET);\n";
        }


        // Menampilkan kecepatan roda dan PWM
        cout << "- Kecepatan rad/s: " << kecepatanRodaRad_[motorIndex] << " rad/s\n";
        cout << "- Kecepatan PWM: " << kecepatanRodaRPM_[motorIndex] << " RPM\n";
        cout << "- Value PWM: " << pwm_[motorIndex] << "\n\n";
    }
};
