/* DISCLAIMER - THIS VERSION IS A PROTOTYPE */

#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <Arduino.h>
#include "html_page.h"

DNSServer dnsServer;
AsyncWebServer server(80);

const char* accessPointName = "EA-236";
const char* accessPointPassword = "teoelectric";

#define FRONT_WHITE_LIGHTS 22 //
#define BACK_WHITE_LIGTHS 13 //
#define FRONT_RED_LIGHTS 23 //
#define BACK_RED_LIGHTS 2 //
#define FRONT_BEAMS 15 //
#define BACK_BEAMS 12 //

// Motor pins
#define FRONT_MOTOR_1_PIN_1 26
#define FRONT_MOTOR_1_PIN_2 25
#define FRONT_MOTOR_2_PIN_1 33
#define FRONT_MOTOR_2_PIN_2 32
#define BACK_MOTOR_1_PIN_1 5
#define BACK_MOTOR_1_PIN_2 18
#define BACK_MOTOR_2_PIN_1 19
#define BACK_MOTOR_2_PIN_2 21

// PWM channels
#define CH_FRONT_1A 0
#define CH_FRONT_1B 1
#define CH_FRONT_2A 2
#define CH_FRONT_2B 3
#define CH_BACK_1A  4
#define CH_BACK_1B  5
#define CH_BACK_2A  6
#define CH_BACK_2B  7

#define PWM_FREQ 20000
#define PWM_RES  10
#define RAMP_STEP 20
#define RAMP_DELAY 15

bool direction = true;
bool lights = false;
bool beams = false;
int throttle = 0;
int targetThrottle = 0;
unsigned long lastRampUpdate = 0;
bool changingDirection = false;
bool pendingDirection = true;

int appConfirmed = 0;
int apActive = 1;

void EEPROMSetup(unsigned size) {
  EEPROM.begin(size);
}

void SaveCredentials(String wifiName, String wifiPassword) {
  EEPROM.writeString(0, wifiName);
  EEPROM.writeString(20, wifiPassword);
  EEPROM.commit();
}

void CreateAP() {
  WiFi.softAP(accessPointName, accessPointPassword);
  dnsServer.start(53, "*", WiFi.softAPIP());
  MDNS.begin("EA-236");
  apActive = true;
}

void ConnectToWiFi(String ssid, String password) {
  WiFi.begin(ssid.c_str(), password.c_str());
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 50) {
    delay(200);
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("IP Address: " + WiFi.localIP().toString());
  } else {
    Serial.println("\nFailed to connect to Wi-Fi.");
  }
}

void EEPROMCheck() {
  String storedWiFiName = EEPROM.readString(0);
  String storedWiFiPassword = EEPROM.readString(20);
  if (storedWiFiName.length() > 0 && storedWiFiPassword.length() > 0) {
    ConnectToWiFi(storedWiFiName, storedWiFiPassword);
  }
}

void LightsSetup() {
  pinMode(FRONT_WHITE_LIGHTS, OUTPUT);
  pinMode(FRONT_RED_LIGHTS, OUTPUT);
  pinMode(BACK_WHITE_LIGTHS, OUTPUT);
  pinMode(BACK_RED_LIGHTS, OUTPUT);
}

void BeamsSetup() {
  pinMode(FRONT_BEAMS, OUTPUT);
  pinMode(BACK_BEAMS, OUTPUT);
  digitalWrite(FRONT_BEAMS, HIGH);
  digitalWrite(BACK_BEAMS, HIGH);
  delay(500);
  digitalWrite(FRONT_BEAMS, LOW);
  digitalWrite(BACK_BEAMS, LOW);
  delay(500);
  digitalWrite(FRONT_BEAMS, HIGH);
  digitalWrite(BACK_BEAMS, HIGH);
  delay(500);
  digitalWrite(FRONT_BEAMS, LOW);
  digitalWrite(BACK_BEAMS, LOW);
  delay(500);
  digitalWrite(FRONT_BEAMS, HIGH);
  digitalWrite(BACK_BEAMS, HIGH);
  delay(500);
  digitalWrite(FRONT_BEAMS, LOW);
  digitalWrite(BACK_BEAMS, LOW);
  delay(500);
}

void MotorsSetup() {
  ledcSetup(CH_FRONT_1A, PWM_FREQ, PWM_RES);
  ledcSetup(CH_FRONT_1B, PWM_FREQ, PWM_RES);
  ledcSetup(CH_FRONT_2A, PWM_FREQ, PWM_RES);
  ledcSetup(CH_FRONT_2B, PWM_FREQ, PWM_RES);
  ledcSetup(CH_BACK_1A, PWM_FREQ, PWM_RES);
  ledcSetup(CH_BACK_1B, PWM_FREQ, PWM_RES);
  ledcSetup(CH_BACK_2A, PWM_FREQ, PWM_RES);
  ledcSetup(CH_BACK_2B, PWM_FREQ, PWM_RES);

  ledcAttachPin(FRONT_MOTOR_1_PIN_1, CH_FRONT_1A);
  ledcAttachPin(FRONT_MOTOR_1_PIN_2, CH_FRONT_1B);
  ledcAttachPin(FRONT_MOTOR_2_PIN_1, CH_FRONT_2A);
  ledcAttachPin(FRONT_MOTOR_2_PIN_2, CH_FRONT_2B);
  ledcAttachPin(BACK_MOTOR_1_PIN_1, CH_BACK_1A);
  ledcAttachPin(BACK_MOTOR_1_PIN_2, CH_BACK_1B);
  ledcAttachPin(BACK_MOTOR_2_PIN_1, CH_BACK_2A);
  ledcAttachPin(BACK_MOTOR_2_PIN_2, CH_BACK_2B);
}

void Train() {
  if(direction) {
    ledcWrite(CH_FRONT_1A, throttle);
    ledcWrite(CH_FRONT_2A, throttle);
    ledcWrite(CH_BACK_1A, throttle);
    ledcWrite(CH_BACK_2A, throttle);
    ledcWrite(CH_FRONT_1B, 0);
    ledcWrite(CH_FRONT_2B, 0);
    ledcWrite(CH_BACK_1B, 0);
    ledcWrite(CH_BACK_2B, 0);
  } else {
    ledcWrite(CH_FRONT_1B, throttle);
    ledcWrite(CH_FRONT_2B, throttle);
    ledcWrite(CH_BACK_1B, throttle);
    ledcWrite(CH_BACK_2B, throttle);
    ledcWrite(CH_FRONT_1A, 0);
    ledcWrite(CH_FRONT_2A, 0);
    ledcWrite(CH_BACK_1A, 0);
    ledcWrite(CH_BACK_2A, 0);
  }
}

void UpdateMotors() {
  unsigned long now = millis();
  if (now - lastRampUpdate < RAMP_DELAY) return;
  lastRampUpdate = now;

  if (changingDirection) {
    if (throttle > 0) {
      throttle -= RAMP_STEP;
      if (throttle < 0) throttle = 0;
      Train();
    } else {
      direction = pendingDirection;
      changingDirection = false;
    }
    return;
  }

  if (throttle < targetThrottle) {
    throttle += RAMP_STEP;
    if (throttle > targetThrottle) throttle = targetThrottle;
    Train();
  } else if (throttle > targetThrottle) {
    throttle -= RAMP_STEP;
    if (throttle < targetThrottle) throttle = targetThrottle;
    Train();
  }
}

void Server() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", captivePortalPage);
  });

server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", captivePortalPage);
  });

  server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", captivePortalPage);
  });

  server.on("/configure", HTTP_POST, [](AsyncWebServerRequest *request){
    String wifiName = request->arg("wifiName");
    String wifiPassword = request->arg("wifiPassword");
    SaveCredentials(wifiName, wifiPassword);
    ConnectToWiFi(wifiName, wifiPassword);
    request->send(200, "text/html", saveConfirmationPage);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
    if (WiFi.status() == WL_CONNECTED) {
      request->send(200, "text/plain", WiFi.localIP().toString());
    } else {
      request->send(200, "text/plain", "Not connected");
    }
  });

  server.on("/throttle", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("throttle")) {
      targetThrottle = request->getParam("throttle")->value().toInt();
      if (targetThrottle < 0) targetThrottle = 0;
      if (targetThrottle > 1023) targetThrottle = 1023;
    }
  });

  server.on("/changeDirection", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("directionName")) {
      String dir = request->getParam("directionName")->value();
      if ((dir == "forward" && !direction) || (dir == "backward" && direction)) {
        changingDirection = true;
        pendingDirection = (dir == "forward");
      }
    }
  });

  server.on("/lights", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("lightsMode")) {
      String lightsMode = request->getParam("lightsMode")->value();
      if (lightsMode == "on") {
        lights = true;
      } else {
        lights = false;
      }
    }
  });

  server.on("/beams", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("beamsMode")) {
      String beamsMode = request->getParam("beamsMode")->value();
      if (beamsMode == "on") {
        beams = true;
      } else {
        beams = false;
      }
    }
  });

  server.begin();
}

void setup() {
  Serial.begin(115200);
  MotorsSetup();
  LightsSetup();
  BeamsSetup();
  EEPROMSetup(512);
  EEPROMCheck();
  CreateAP();
  Server();
}

void loop() {
  UpdateMotors();
  if(lights) {
    if(direction) {
      digitalWrite(FRONT_WHITE_LIGHTS, HIGH);
      digitalWrite(FRONT_RED_LIGHTS, LOW);
      digitalWrite(BACK_RED_LIGHTS, HIGH);
      digitalWrite(BACK_WHITE_LIGTHS, LOW);
    } else {
      digitalWrite(FRONT_WHITE_LIGHTS, LOW);
      digitalWrite(FRONT_RED_LIGHTS, HIGH);
      digitalWrite(BACK_RED_LIGHTS, LOW);
      digitalWrite(BACK_WHITE_LIGTHS, HIGH);
    }
  } else {
    digitalWrite(FRONT_WHITE_LIGHTS, LOW);
    digitalWrite(FRONT_RED_LIGHTS, LOW);
    digitalWrite(BACK_WHITE_LIGTHS, LOW);
    digitalWrite(BACK_RED_LIGHTS, LOW);
  }
  if(beams) { 
    if(direction) {
      digitalWrite(FRONT_BEAMS, HIGH);
      digitalWrite(BACK_BEAMS, LOW);
    } else {
      digitalWrite(FRONT_BEAMS, LOW);
      digitalWrite(BACK_BEAMS, HIGH);
    }
  } else {
    digitalWrite(FRONT_BEAMS, LOW);
    digitalWrite(BACK_BEAMS, LOW);
  }
  dnsServer.processNextRequest();
}
