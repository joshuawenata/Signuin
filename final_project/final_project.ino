#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Firebase_ESP_Client.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Ganti dengan SSID dan password WiFi Anda
#define WIFI_SSID "!"
#define WIFI_PASSWORD "VALOGASPOL"
#define FIREBASE_HOST "your-firebase-project-id.firebaseio.com"  // Your Firebase database URL
#define FIREBASE_AUTH "your-firebase-api-key"  // Your Firebase API key

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Alamat I2C dan dimensi LCD

// Konfigurasi pin untuk modul GPS
const int GPS_TX_PIN = D8; // TX pin dari modul GPS
const int GPS_RX_PIN = D7; // RX pin dari modul GPS

SoftwareSerial gpsSerial(GPS_TX_PIN, GPS_RX_PIN); // Inisialisasi koneksi Serial GPS

TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }

  FirebaseAuth auth;
  FirebaseConfig config;

  config.api_key = FIREBASE_AUTH;
  config.database_url = FIREBASE_HOST;

  // Inisialisasi koneksi Firebase
  Firebase.begin(&config, &auth);
  
  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Firebase Data:");
  
  // Inisialisasi koneksi Serial GPS
  gpsSerial.begin(9600);
}

void loop() {
  FirebaseData data;
  
  // Membaca data dari Firebase pada path /call
  if (Firebase.RTDB.getString(&data, "/call")) {
    if (data.dataType() == "string") {
      Serial.print("Data dari Firebase: ");
      Serial.println(data.stringData());

      // Menampilkan data dari Firebase pada LCD
      lcd.setCursor(0, 1);
      lcd.print("Value: ");
      lcd.print(data.stringData());
    } else {
      Serial.println("Error: Data dari Firebase bukan string.");
    }
  } else {
    Serial.println("Error saat mengambil data dari Firebase");
    Serial.println(data.errorReason());
  }

  // Membaca data dari modul GPS
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
    if (gps.location.isValid()) {
      lcd.setCursor(0, 2);
      lcd.print("Lat: " + String(gps.location.lat(), 6));
      lcd.setCursor(0, 3);
      lcd.print("Lon: " + String(gps.location.lng(), 6));
    }
  }

  delay(10000);  // Tunggu 10 detik sebelum mengambil data lagi
}
