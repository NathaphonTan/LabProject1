#include <SoftwareSerial.h>

SoftwareSerial RFID(16, 17); 

void setup() {
  Serial.begin(115200);
  RFID.begin(9600); 

  Serial.println("Ready to scan RFID...");
}

void loop() {
  if (RFID.available()) {
    String rfidData = "";
    while (RFID.available()) {
      rfidData += (char)RFID.read(); 
    }
    Serial.println("RFID Tag Detected: " + rfidData); 
  }
}
