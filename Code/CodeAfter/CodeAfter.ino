#include <SoftwareSerial.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

SoftwareSerial RFID(16, 17); // RX, TX

// ตั้งค่าการเชื่อมต่อ Wi-Fi
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// ตั้งค่าการเชื่อมต่อ Firebase
#define FIREBASE_HOST "your_firebase_database_url"
#define FIREBASE_AUTH "your_firebase_database_auth"

FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);
  RFID.begin(9600);  // เริ่มต้นการสื่อสารกับ RDM6300

  pinMode(2, OUTPUT);  // กำหนดพินที่ใช้เปิด LED

  // เชื่อมต่อ Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // เชื่อมต่อ Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  Serial.println("Ready to scan RFID...");
}

void loop() {
  if (RFID.available()) {
    String rfidData = "";
    while (RFID.available()) {
      rfidData += (char)RFID.read();  // อ่านข้อมูลจาก RFID
    }
    Serial.println("RFID Tag Detected: " + rfidData);

    // เปิด LED เมื่อแท็ก RFID ถูกสแกน
    digitalWrite(2, HIGH);
    delay(2000);  // เปิด LED เป็นเวลา 2 วินาที
    digitalWrite(2, LOW);

    // ส่งข้อมูลไปยัง Firebase
    String path = "/rfidData/";
    Firebase.setString(firebaseData, path, rfidData);
    if (firebaseData.error()) {
      Serial.println("Error sending data to Firebase: " + firebaseData.errorReason());
    }
  }
}
