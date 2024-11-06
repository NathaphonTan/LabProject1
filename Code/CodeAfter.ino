#include <SoftwareSerial.h>

#define RX_PIN 5      
#define TX_PIN 4       
#define RELAY_PIN 2

SoftwareSerial rdmSerial(RX_PIN, TX_PIN); 

void setup() {
  Serial.begin(115200);    
  rdmSerial.begin(9600);   
  pinMode(RELAY_PIN, OUTPUT); 
}

void loop() {
  if (rdmSerial.available()) {  
    String rfidTag = "";
    
    while (rdmSerial.available()) {
      rfidTag += (char)rdmSerial.read();  
    }

    Serial.print("RFID Tag Detected: ");
    Serial.println(rfidTag);

   
    if (rfidTag == "0F00147EC0") { 
      Serial.println("Access Granted");  
      digitalWrite(RELAY_PIN, HIGH);   
      delay(5000);                      
      digitalWrite(RELAY_PIN, LOW);    
    } else {
      Serial.println("Access Denied");  
    }
  }
}
