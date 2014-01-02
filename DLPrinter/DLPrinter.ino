#include <Arduino.h>

#include "SPI.h"
#include "SoftwareSerial.h"
#include "Adafruit_Thermal.h"

#include <avr/pgmspace.h>

#include "HEART.h"
#include "MESSAGES.h"

#include <Bounce.h>
#include <LED.h>

#include <EEPROM.h>

const int kPrinter_RX_Pin = 5;  // This is the green wire
const int kPrinter_TX_Pin = 6;  // This is the yellow wire

const int kButtonPin = 7;
const int kLEDPin = 13;

boolean ledValue = LOW;

LED led = LED(kLEDPin);
Bounce button = Bounce(kButtonPin, 100);

Adafruit_Thermal printer(kPrinter_RX_Pin, kPrinter_TX_Pin);

void setup() {
//  Serial.begin(9600);
//  Serial.println("Initialized.");
  pinMode(kLEDPin, OUTPUT);
  digitalWrite(kLEDPin, LOW);
  pinMode(kButtonPin, INPUT);
  digitalWrite(kButtonPin, HIGH);
  printer.begin();
  printer.sleep();
}

void loop() {
  buttonHandler();
//  button.update();
//  if (button.fallingEdge()) {
//    Serial.println("Button pressed.");
//    buttonPressed();
//  }
}

void buttonHandler() {
  button.update();
  if (button.fallingEdge()) {
//    Serial.println("Button pressed.");
    
//    printer.println("Happy Birthday Diana!");
    buttonPressed();
  }
}

void buttonPressed() {
  led.on();
  printer.wake();
  if (printer.hasPaper()) {
    printMessage();
    printSignature();
    printHeart();
    printer.feed(4);
  }
  printer.sleep();
  led.off();
}

void printMessage() {
  printer.boldOn();
  printer.println("Hello lovely!");
  printer.feed();
//  // we always store the index at location 0;
  int index = 0;
  index = EEPROM.read(0);
  index++;
  if (index >= MESSAGES_length) {
   index = 0; 
  }
  switch (index) {
    case 0:
      printer.println("I hope you have\na wonderful day :).");
      break;
    case 1:
      printer.println("You are the\nmost beautifulest!");
      break;
    case 2:
      printer.println("Today is going\nto be a great day!");
      break;
    case 3:
      printer.println("Be the\n#winningnest\nthat you can be.");
      break;
    case 4:
      printer.println("Don't forget to\n call 714-878-9551\n for a pick-me-up!");
      break;
  }
////  String message = String(MESSAGES_messages[index]);
////  printer.println(message);
  EEPROM.write(0, index);
//  char buf[MESSAGES_max_message_length];
////  strcpy_P(buf, (char*)pgm_read_word(&(MESSAGES_messages[0])));
////  Serial.println(buf);
//  char c;
//  prog_uchar *str = (prog_uchar *)&MESSAGES_messages[0];
//   while((c = pgm_read_byte(str++)))
//     Serial.write(c);
  printer.boldOff();
}

void printSignature() {
  printer.println(MESSAGES_signature); 
}

void printHeart() {
  printer.printBitmap(HEART_width, HEART_height, HEART_data);
}
  
