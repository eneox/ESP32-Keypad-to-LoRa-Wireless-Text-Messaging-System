#include "LoRa_E32.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LoRa_E32 e32ttl(Serial2); 
struct Signal {
  char type[15];
  byte temp[4];
} data;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);  
  e32ttl.begin();
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  delay(500);
}

void loop() {
  while (e32ttl.available() > 1) {
    ResponseStructContainer rsc = e32ttl.receiveMessage(sizeof(Signal));
    struct Signal receivedData = *(Signal*) rsc.data;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Received Message:");
    lcd.setCursor(0, 1);
    lcd.print(receivedData.type);

    rsc.close();
  }
}
