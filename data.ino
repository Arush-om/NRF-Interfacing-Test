#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h> 
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial s_serial_to_esp(3, 2);
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
#define SS_PIN 10
#define RST_PIN 9
#define LED_G 5 
#define LED_R 4

MFRC522 mfrc522(SS_PIN, RST_PIN);  
float Data;

void setup() 

{
  lcd.begin();
  Serial.begin(9600);
  while (!Serial) continue;
  s_serial_to_esp.begin(4800);
  Serial.flush();
  
  lcd.begin();
  lcd.backlight();
  lcd.print("     WELCOME"); 
  delay(3000);
  lcd.clear();
  
  SPI.begin();   
  mfrc522.PCD_Init();  
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
 

}
void loop() 
{
lcd.clear();
lcd.print("   PLACE YOUR");
lcd.setCursor(0, 1);
lcd.print("   RFID TAG");
delay(1000);
lcd.clear();

DynamicJsonBuffer jBuffer;
JsonObject& root = jBuffer.createObject();
root["Data"] = Data;

  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  if (content.substring(1) == "D3 16 EC 11") 
  {
    
   lcd.clear();
   lcd.print(" ACCESS GRANDED"); 
   for (int Data = 1; Data <= 100; Data++){
   root.prettyPrintTo(s_serial_to_esp);
   Serial.print(Data);
   }
   
    digitalWrite(LED_G, HIGH);
    delay(6000);
    lcd.clear();
    digitalWrite(LED_G, LOW);
  }
 
 else   {

   lcd.print("  ACCESS DENIED"); 
   lcd.setCursor(0, 0);
       digitalWrite(LED_R, HIGH);
    delay(2000);
           digitalWrite(LED_R, LOW);
              lcd.clear();
    delay(2000);

  }
}
