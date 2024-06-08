#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#define SERVO_PIN 6
#define RST_PIN 9
#define SS_PIN 10
int IRsensor =7;
int flamepin =A0;
int buzpin=3;
int threshold=80;
int flamesenvalue=0;
byte readCard[4];
String MasterTag = "E368471C";  // REPLACE this Tag ID with your Tag ID!!! E3 68 47 1C


String tagID = "";

Servo myServo;
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() 
{
 
  SPI.begin(); // SPI bus
  mfrc522.PCD_Init(); // MFRC522
  myServo.attach(SERVO_PIN);
  myServo.write(0);
  pinMode(flamepin, INPUT);
  pinMode(IRsensor,INPUT);
  pinMode(buzpin, OUTPUT);


}
 
void loop() 
{

   while (getID()) 
  { 
    if (tagID == MasterTag) 
    {
      
        myServo.write(90);
        delay(4000);
        myServo.write(0);


      // You can write any code here like opening doors, switching on a relay, lighting up an LED, or anything else you can think of.
    }
    else
    {  
             myServo.write(0);

    }

  }
  flamesenvalue=analogRead(flamepin);
Serial.println(flamesenvalue);
if(flamesenvalue<=threshold){
  tone(buzpin,300,330);
}



if(digitalRead(IRsensor)==HIGH){
  delay(1000);
  myServo.write(0);
}
else{
  myServo.write(90);
}
  //Wait until new tag is available

 
}
 
//Read new tag if available
boolean getID() 
{
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
  return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
  return false;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) { // The MIFARE PICCs that we use have 4 byte UID
  //readCard[i] = mfrc522.uid.uidByte[i];
  tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}
