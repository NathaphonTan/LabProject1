#include <SoftwareSerial.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

SoftwareSerial RFID(16, 17); 


const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

#define FIREBASE_HOST "your_firebase_database_url"
#define FIREBASE_AUTH "your_firebase_database_auth"

FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);
  RFID.begin(9600);  

  pinMode(2, OUTPUT); 


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  Serial.println("Ready to scan RFID...");
}

void loop() {
  if (RFID.available()) {
    String rfidData = "";
    while (RFID.available()) {
      rfidData += (char)RFID.read();  
    }
    Serial.println("RFID Tag Detected: " + rfidData);

 
    digitalWrite(2, HIGH);
    delay(2000);  
    digitalWrite(2, LOW);

    String path = "/rfidData/";
    Firebase.setString(firebaseData, path, rfidData);
    if (firebaseData.error()) {
      Serial.println("Error sending data to Firebase: " + firebaseData.errorReason());
    }
  }
}
