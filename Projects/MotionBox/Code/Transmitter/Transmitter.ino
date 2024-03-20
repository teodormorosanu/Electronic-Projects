#include <SPI.h>
#include <nRF24L01.h> // Libraries for nRF24L01 transmitter
#include <RF24.h>

#define NRF_CE 7 // NRF24L01 CE pin
#define NRF_CSN 8 // NRF24L01 CSN pin
#define PIR 3 // PIR OUT pin

RF24 radio(NRF_CE, NRF_CSN); // Initialising RF24

int sensorValue; // Variable for storing the PIR sensor value

void PirSetup() { // Method for PIR setup
  pinMode(PIR, INPUT); // Setting the PIR pin as input
}

void TransmitterSetup(byte address[]) { // Method for transmitter setup
  radio.begin(); // Radio beginning
  radio.openWritingPipe(address); // Setting the adress
  radio.setPALevel(RF24_PA_MAX); // Setting the PA level
  radio.setDataRate(RF24_250KBPS); // Setting the data rate
  radio.stopListening(); // Stopping the transmitter listening
}

void setup() {
  PirSetup(); // Method for PIR setup
  TransmitterSetup("00001"); // Method for transmitter setup
}

void Pir() { // Method for getting the PIR sensor value
  sensorValue = digitalRead(PIR); // Getting the sensor voltage (HIGH / LOW)
}

void Transmitter() { // Method for transmitting the data
  if(sensorValue == HIGH) { // If PIR detects movement a "verify" text is transmitted
    const char text[] = "verify";
    radio.write(&text, sizeof(text));
  }
}

void loop() {
  Pir(); // Method for getting the PIR sensor value
  Transmitter(); // Method for transmitting the data
}

