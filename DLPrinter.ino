#include "SoftwareSerial.h"
#include "Adafruit_Thermal.h"

#include "HEART.h"

#include <avr/pgmspace.h>
#include <Bounce.h>
#include <LED.h>

const int printer_RX_Pin = 5;  // This is the green wire
const int printer_TX_Pin = 6;  // This is the yellow wire

const int kButtonPin = 0;
const int kLEDPin = 0;

boolean ledValue = LOW;

LED led = LED(kLEDPin);
Bounce button = Bounce(kButtonPin, 100);

Adafruit_Thermal printer(printer_RX_Pin, printer_TX_Pin);

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
  printMessage();
  printHeart();
  printer.sleep();
  led.off();
}

void printMessage() {
  printer.println("Hello lovely!");
}

void printHeart() {
  printer.printBitmap(HEART_width, HEART_height, HEART_data);
}
  
