#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "LoRa_E32.h"
#include <Keypad.h>

SoftwareSerial mySerial(10, 11);
LoRa_E32 e32ttl(&mySerial);

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROW_NUM = 4;
const byte COLUMN_NUM = 4;
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte pin_rows[ROW_NUM] = {9, 8, 7, 6};
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

String messageToSend = "";
bool sendingMessage = false;

void setup() {
  Serial.begin(9600);  
  mySerial.begin(9600);
  e32ttl.begin();
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  pinMode(12, INPUT_PULLUP);
}

void loop() {
  char key = keypad.getKey();

  if (key && !sendingMessage) {
    lcd.setCursor(messageToSend.length(), 0);
    lcd.print(key);
    messageToSend += key;
  }

  if (digitalRead(12) == LOW && !sendingMessage && messageToSend.length() > 0) {
    lcd.clear();
    lcd.print("Sending Message...");
    sendLoRaMessage(messageToSend);
    sendingMessage = true;
  }

  while (e32ttl.available() > 1) {
    ResponseStructContainer rsc = e32ttl.receiveMessage(32);
    String receivedMessage = rsc.data;
    lcd.clear();
    lcd.print("Received Message:");
    lcd.setCursor(0, 1);
    lcd.print(receivedMessage);
    rsc.close();
  }
}

void sendLoRaMessage(String message) {
  ResponseStatus rs = e32ttl.sendStringMessage(0, 44, 23, message);
  delay(1000);
  lcd.clear();
  lcd.print("Message Sent!");
  delay(2000);
  lcd.clear();
  messageToSend = "";
  sendingMessage = false;
}
