#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7,8);

int sensorValue;

void PirSetup(int sensorPin) {
  pinMode(sensorPin, INPUT);
}

void TransmitterSetup(byte address[]) {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
}

void setup() {
  Serial.begin(9600);
  PirSetup(3);
  TransmitterSetup("00001");
}

void Pir() {
  sensorValue = digitalRead(3);
  Serial.println(sensorValue);
}

void Transmitter() {
  if(sensorValue == HIGH) {
    const char text[] = "verify";
    radio.write(&text, sizeof(text));
  }
}

void loop() {
  Pir();
  Transmitter();
}

