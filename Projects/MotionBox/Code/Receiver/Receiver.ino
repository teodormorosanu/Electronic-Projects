#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7,8);

void LEDsSetup(int redPin, int yellowPin, int greenPin) {
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
}

void BuzzerSetup(int vccPin, int iPin) {
  pinMode(iPin, OUTPUT);
  pinMode(vccPin, OUTPUT);
}

void ReceiverSetup(byte address[]) {
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
}

void setup() {
  Serial.begin(9600);
  LEDsSetup(4, 5, 6);
  BuzzerSetup(3, 2);
  ReceiverSetup("00001");
  digitalWrite(5, HIGH);
}

void Receiver(int iPin) {
  char text[32] = "";
  if (radio.available()) {
    digitalWrite(5, LOW);
    radio.read(&text, sizeof(text));
    String transData = String(text);
    if(transData == "verify") {
      Serial.println("Te-am prins");
      digitalWrite(6, LOW);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      tone(iPin, 2000);
      delay(500);
      digitalWrite(4, LOW);
      tone(iPin, 1600);
      delay(500);
      digitalWrite(4, HIGH);
    }
    Serial.println("Liber");
    digitalWrite(6, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(3, LOW);
    noTone(iPin);  
  }
}

void loop() {
  Receiver(2);
}
