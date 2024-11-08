## LabProject1
## แนวทางการทำงาน ESP32 project cook book
## 1. ระบุตัวอย่างที่ใช้ ว่าเอามาจากตัวอย่างไหน
 ตัวอย่างโปรเจค RFID Security System (ระบบรักษาความปลอดภัยด้วย RFID) ที่ใช้โมดูล RDM6300 สำหรับการอ่านแท็ก RFID ร่วมกับ ESP32 เพื่อสร้างระบบควบคุมการเข้าถึง
 
 https://esphome.io/components/binary_sensor/rdm6300.html
 
## 2. ระบุว่า จะแก้ไขตรงไหนบ้าง เพื่ออะไร
**เพิ่มฟังก์ชันการตรวจสอบ RFID tag**

*ในโค้ดเดิม*    ไม่มีการตรวจสอบว่าค่า RFID tag ตรงกับ tag ที่อนุญาตให้ปลดล็อคหรือไม่

*ในโค้ดใหม่*    เพิ่มการตรวจสอบโดยใช้ if (rfidTag == "0F00147EC0") เพื่อเช็คว่า tag ที่อ่านได้ตรงกับ tag ที่อนุญาตให้ปลดล็อคหรือไม่

**การเปิด/ปิดรีเลย์:**

*ในโค้ดเดิม*    ไม่มีการควบคุมรีเลย์

*ในโค้ดใหม่*    มีการควบคุมรีเลย์ด้วยคำสั่ง digitalWrite(RELAY_PIN, HIGH) เพื่อปลดล็อค และ digitalWrite(RELAY_PIN, LOW) เพื่อล็อคกลับหลังจาก 5 วินาที

**แสดงผลใน Serial Monitor:**

*ในโค้ดเดิม*    แสดงแค่ข้อมูล RFID tag ที่อ่านได้

*ในโค้ดใหม่*    แสดงผลทั้งข้อมูล RFID tag และข้อความเกี่ยวกับสถานะการปลดล็อค (Access Granted / Access Denied)

## 3. แสดงขั้นตอนการทำ project
**ขั้นตอนที่ 1: การเตรียมการเชื่อมต่อ**
เชื่อมต่อโมดูล RDM6300 กับ ESP32
* RDM6300 TX → ESP32 RX (GPIO 5)
* RDM6300 RX → ESP32 TX (GPIO 4)
* เชื่อมต่อพอร์ต GND และ VCC ของโมดูล RDM6300 กับ ESP32

![4](https://github.com/user-attachments/assets/30431a16-7c02-499f-a6ec-6419ce3ee0fd)

**ขั้นตอนที่ 2: การติดตั้งไลบรารี**
ไลบรารีที่ใช้สำหรับการทำงานกับ RDM6300:
* SoftwareSerial หรือ HardwareSerial (เพื่อสื่อสารกับโมดูล RDM6300)
* Wire สำหรับการสื่อสาร I2C (ถ้าจำเป็น) หรือการควบคุมอุปกรณ์อื่นๆ

**ขั้นตอนที่ 3: การเขียนโค้ด**
  ```cpp
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
```
**ขั้นตอนที่ 4: การอัพโหลดโค้ดและทดสอบ**
* อัพโหลดโค้ดลงใน ESP32 ผ่าน Arduino IDE
* ตรวจสอบผลลัพธ์ใน Serial Monitor
* ทดสอบการอ่าน RFID tag และตรวจสอบการปลดล็อคหรือไม่

## 4. แสดงผลการทำ project
* เมื่อแสดงผลใน Serial Monitor, คุณจะเห็นข้อมูล RFID tag ที่อ่านจากโมดูล RDM6300
* หาก RFID tag ที่อ่านได้ตรงกับ tag ที่อนุญาต ("0F00147EC0"), ระบบจะแสดงข้อความ Access Granted และปลดล็อคโดยการเปิดรีเลย์
* หากไม่ตรงกับ tag ที่อนุญาต, ระบบจะแสดงข้อความ Access Denied และไม่ทำการปลดล็อค

![3](https://github.com/user-attachments/assets/25cc4486-d226-44d7-b481-1c4af9a3d396)

![2](https://github.com/user-attachments/assets/5d40147d-34ce-43a2-86d2-1bf991891c4a)

## 5. สรุปผลการทำ project
โปรเจคนี้มีวัตถุประสงค์เพื่อสร้างระบบการปลดล็อคโดยใช้เทคโนโลยี RFID ผ่านโมดูล RDM6300 และ ESP32 ซึ่งจะสามารถอ่านข้อมูลจาก RFID tag และทำการตรวจสอบว่าเป็น tag ที่ได้รับอนุญาตหรือไม่ หากใช่จะทำการปลดล็อคด้วยรีเลย์ (หรืออุปกรณ์ที่ควบคุมการปลดล็อค) และแสดงผลสถานะผ่าน Serial Monitor หรือหน้าจอการแจ้งเตือน

**ผลลัพธ์ที่ได้:**
* การอ่านข้อมูลจาก RFID tag: ระบบสามารถอ่านข้อมูลจาก RFID tag ที่นำมาใช้ได้อย่างถูกต้อง เมื่อ tag ถูกนำมาใกล้กับโมดูล RDM6300, ข้อมูลที่อ่านได้จะถูกส่งไปยัง ESP32 และแสดงผลบน Serial Monitor
* การตรวจสอบ RFID tag: ระบบสามารถเปรียบเทียบข้อมูลที่ได้จาก RFID tag กับข้อมูลที่ตั้งไว้ในโค้ด (ในกรณีนี้คือ "0F00147EC0") และตัดสินใจว่าจะอนุญาตให้ปลดล็อคหรือไม่
* การปลดล็อค: หาก RFID tag ที่อ่านได้ตรงกับ tag ที่อนุญาต ระบบจะส่งสัญญาณไปยังรีเลย์ (หรืออุปกรณ์ที่ใช้งาน) เพื่อปลดล็อค โดยรีเลย์จะเปิดเป็นเวลา 5 วินาที จากนั้นจะปิดเพื่อกลับสู่สถานะล็อค
* การแสดงผลสถานะ: ผลการตรวจสอบจะถูกแสดงบน Serial Monitor ว่า Access Granted หรือ Access Denied ตามผลการตรวจสอบ RFID tag
