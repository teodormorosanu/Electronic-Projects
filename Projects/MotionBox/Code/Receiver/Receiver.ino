#include <SPI.h>
#include <nRF24L01.h> // Libraries for nRF24L01 transmitter
#include <RF24.h>

#define NRF_CE 7 // NRF24L01 CE pin
#define NRF_CSN 8 // NRF24L01 CSN pin
#define RED_LED A0 // Red led pin
#define YELLOW_LED A1 // Yellow led pin
#define GREEN_LED A2 // Green led pin
#define BUZZ_VCC 3 // Buzzer VCC pin
#define BUZZ_IN 2 // Buzzer IN pin

RF24 radio(NRF_CE, NRF_CSN); // Initialising RF24

void LEDsSetup() { // Method for LEDs setup
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT); // Setting the LED pins as output
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(YELLOW_LED, HIGH); // Powering the yellow LED
}

void BuzzerSetup() { // Method for buzzer setup
  pinMode(BUZZ_IN, OUTPUT); // Setting the buzzer pin as output
  pinMode(BUZZ_VCC, OUTPUT); // Powering the buzzer from another arduino pin
}

void ReceiverSetup(byte address[]) { // Method for receiver setup
  radio.begin(); // Radio beginning
  radio.openReadingPipe(0, address); // Getting the adress
  radio.setPALevel(RF24_PA_MAX); // Setting the PA level
  radio.setDataRate(RF24_250KBPS); // Setting the data rate
  radio.startListening();// Starting the receiver listening
}

void setup() {
  LEDsSetup(); // Method for LEDs setup
  BuzzerSetup(); // Method for buzzer setup
  ReceiverSetup("00001"); // Method for receiver setup
}

void Receiver() { // Method for receiving and processing the data
  char text[32] = "";
  if (radio.available()) { // If a radio signal is available, a text is read
    digitalWrite(A1, LOW);
    radio.read(&text, sizeof(text));
    String transData = String(text);
    if(transData == "verify") { // If the readed text is "verify", the buzzer and red LED are turned on
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BUZZ_VCC, HIGH);
      digitalWrite(RED_LED, HIGH);
      tone(BUZZ_IN, 2000);
      delay(500);
      digitalWrite(RED_LED, LOW);
      tone(BUZZ_IN, 1600);
      delay(500);
      digitalWrite(RED_LED, HIGH);
    }
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW); // Powering on the green LED and powering off the red LED and the buzzer if no text is received
    digitalWrite(BUZZ_VCC, LOW);
    noTone(BUZZ_IN);  
  }
}

void loop() {
  Receiver(); // Method for receiving and processing the data
}
