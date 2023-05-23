#include <SPI.h>
#include <nRF24L01.h> // Libraries for nRF24L01 transmitter
#include <RF24.h>

RF24 radio(7,8); // Initialising RF24

void LEDsSetup(int redPin, int yellowPin, int greenPin) { // Method for LEDs setup
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT); // Setting the LED pins as output
  pinMode(greenPin, OUTPUT);
}

void BuzzerSetup(int vccPin, int iPin) { // Method for buzzer setup
  pinMode(iPin, OUTPUT); // Setting the buzzer pin as output
  pinMode(vccPin, OUTPUT); // Powering the buzzer from another arduino pin
}

void ReceiverSetup(byte address[]) { // Method for receiver setup
  radio.begin(); // Radio beginning
  radio.openReadingPipe(0, address); // Getting the adress
  radio.setPALevel(RF24_PA_MAX); // Setting the PA level
  radio.setDataRate(RF24_250KBPS); // Setting the data rate
  radio.startListening();// Starting the receiver listening
}

void setup() {
  LEDsSetup(A0, A1, A2); // Method for LEDs setup
  BuzzerSetup(3, 2); // Method for buzzer setup
  ReceiverSetup("00001"); // Method for receiver setup
  digitalWrite(A1, HIGH); // Powering the yellow LED
}

void Receiver(int iPin) { // Method for receiving and processing the data
  char text[32] = "";
  if (radio.available()) { // If a radio signal is available, a text is read
    digitalWrite(A1, LOW);
    radio.read(&text, sizeof(text));
    String transData = String(text);
    if(transData == "verify") { // If the readed text is "verify", the buzzer and red LED are turned on
      digitalWrite(A2, LOW);
      digitalWrite(3, HIGH);
      digitalWrite(A0, HIGH);
      tone(iPin, 2000);
      delay(500);
      digitalWrite(A0, LOW);
      tone(iPin, 1600);
      delay(500);
      digitalWrite(A0, HIGH);
    }
    digitalWrite(A2, HIGH);
    digitalWrite(A0, LOW); // Powering on the green LED and powering off the red LED and the buzzer if no text is received
    digitalWrite(3, LOW);
    noTone(iPin);  
  }
}

void loop() { // Method for receiving and processing the data
  Receiver(2);
}
