#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include "SR04.h"

#define RST_PIN   5     
#define SS_PIN    53    

#define TRIG_PIN 12     
#define ECHO_PIN 11     

Servo myservo;         
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);  
long distance;         

MFRC522 mfrc522(SS_PIN, RST_PIN);   

bool isRadarActivated = false;      
bool rfidCardDetected = false;      

void setup() {
  Serial.begin(9600);
  SPI.begin();          
  mfrc522.PCD_Init();   
  myservo.attach(9);    

  Serial.println(F("Place your RFID card on the reader to activate the radar."));
}

void loop() {
 
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);  
    return;
  }

  
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  
  if (!rfidCardDetected) {
    rfidCardDetected = true;  
    isRadarActivated = true;  
    Serial.println(F("Radar activated!"));
  }

  
  if (isRadarActivated) {
    sweepServoAndMeasure();
  }

  delay(1000); 
}


void sweepServoAndMeasure() {
  for (int pos = 0; pos <= 180; pos += 2) {  
    myservo.write(pos);
    distance = sr04.Distance();  
    Serial.print("Angle: ");
    Serial.print(pos);
    Serial.print(" degrees, Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    
    Serial.print(pos);
    Serial.print(",");
    Serial.println(distance);

    delay(100);  
  }
  
  for (int pos = 180; pos >= 0; pos -= 2) {  
    myservo.write(pos);
    distance = sr04.Distance();  
    Serial.print("Angle: ");
    Serial.print(pos);
    Serial.print(" degrees, Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    
    Serial.print(pos);
    Serial.print(",");
    Serial.println(distance);

    delay(100);  
  }
}
