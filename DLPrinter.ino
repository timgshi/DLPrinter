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

const int kButtonPin = 0;
const int kLEDPin = 0;

boolean ledValue = LOW;

LED led = LED(kLEDPin);
Bounce button = Bounce(kButtonPin, 100);

Adafruit_Thermal printer(kPrinter_RX_Pin, kPrinter_TX_Pin);

void setup() {
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  pinMode(kButtonPin, INPUT);
  digitalWrite(kButtonPin, HIGH);
  printer.begin();
  printer.sleep();
}

void loop() {
  buttonHandler();
}

void buttonHandler() {
  button.update();
  if (button.fallingEdge()) {
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
  }
  printer.sleep();
  led.off();
}

void printMessage() {
  printer.println("Hello lovely!");
  // we always store the index at location 0;
  int index = 0;
  index = EEPROM.read(0);
  index++;
  if (index > MESSAGES_length) {
   index = 0; 
  }
  String message = String(MESSAGES_messages[index]);
  printer.println(message);
  EEPROM.write(0, index);
}

void printSignature() {
  printer.println(MESSAGES_signature); 
}

void printHeart() {
  printer.printBitmap(HEART_width, HEART_height, HEART_data);
}
  
