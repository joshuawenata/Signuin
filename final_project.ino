#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Firebase_ESP_Client.h>

#define WIFI_SSID "oppo"
#define WIFI_PASSWORD "23maret2003"
#define FIREBASE_HOST "your-firebase-project-id.firebaseio.com"  // Your Firebase database URL
#define FIREBASE_AUTH "your-firebase-api-key"  // Your Firebase API key

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address and LCD dimensions

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  FirebaseAuth auth;
  FirebaseConfig config;

  config.api_key = FIREBASE_AUTH;
  config.database_url = FIREBASE_HOST;

  Firebase.begin(&config, &auth);
  
  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Firebase Data:");
}

void loop() {
  FirebaseData data;
  if (Firebase.RTDB.getString(&data, "/call")) {
    if (data.dataType() == "string") {
      Serial.print("Data from Firebase: ");
      Serial.println(data.stringData());

      // Display data on the LCD
      lcd.setCursor(0, 1);
      lcd.print("Value: ");
      lcd.print(data.stringData());
    } else {
      Serial.println("Error: Data from Firebase is not a string.");
    }
  } else {
    Serial.println("Error getting data from Firebase");
    Serial.println(data.errorReason());
  }

  delay(10000);  // Wait for 10 seconds before requesting data again
}
