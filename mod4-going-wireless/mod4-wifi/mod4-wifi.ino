#include "Arduino.h"
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#define GREEN_LED1      14
#define GREEN_LED2      13
#define BLUE_LED1       12
#define BLUE_LED2       9
#define YELLOW_LED1     19
#define YELLOW_LED2     4
#define RED_LED1        18
#define RED_LED2        15
#define PHOTORESISTOR   5
#define WEATHER_BUTTON  0

bool prevAPIStatus = false;
bool weatherAPIOn = false;
bool weatherIsPressed = false;
int analogValue;

String colorString = "";

// the IP of the machine to which you send msgs - this should be the correct IP in most cases (see note in python code)
#define CONSOLE_IP "192.168.1.2"
#define CONSOLE_PORT 4210
const char* ssid = "ESP32";
const char* password = "12345678";
WiFiUDP Udp;
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
WebServer server(80);

void clear_leds() {
  digitalWrite(BLUE_LED1, LOW);
  digitalWrite(BLUE_LED2, LOW);
  digitalWrite(GREEN_LED1, LOW);
  digitalWrite(GREEN_LED2, LOW);
  digitalWrite(YELLOW_LED1, LOW);
  digitalWrite(YELLOW_LED2, LOW);
  digitalWrite(RED_LED1, LOW);
  digitalWrite(RED_LED2, LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(WEATHER_BUTTON, INPUT);
  //pinMode(buttonPin, INPUT);
  
  pinMode(GREEN_LED1, OUTPUT);
  pinMode(GREEN_LED2, OUTPUT);
  pinMode(BLUE_LED1, OUTPUT);
  pinMode(BLUE_LED2, OUTPUT);
  pinMode(YELLOW_LED1, OUTPUT);
  pinMode(YELLOW_LED2, OUTPUT);
  pinMode(RED_LED1, OUTPUT);
  pinMode(RED_LED2, OUTPUT);
  
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  server.begin();
}

void loop() {
  analogValue = analogRead(PHOTORESISTOR) / 4;

  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  Udp.print(analogValue);
  Udp.endPacket();

  if (WEATHER_BUTTON == LOW && weatherIsPressed) {
    weatherIsPressed = !weatherIsPressed;
    delay(100);

    if (!weatherAPIOn) {
      prevAPIStatus = weatherAPIOn;
      Serial.println("Turn on API");
      weatherAPIOn = !weatherAPIOn;
    } else {
      prevAPIStatus = weatherAPIOn;
      weatherAPIOn = !weatherAPIOn;
    }
  } else if (WEATHER_BUTTON == HIGH && !weatherIsPressed) {
    weatherIsPressed = !weatherIsPressed;
  }

  while (Serial.available() > 0) {
    char c = Serial.read();
    colorString = c;
  }

  if (prevAPIStatus != weatherAPIOn) {
    clear_leds();
  }
  
  if (!weatherAPIOn) {
    if (analogValue < 127) {
      digitalWrite(YELLOW_LED1, HIGH);
      digitalWrite(YELLOW_LED2, HIGH);
    } else {
      digitalWrite(YELLOW_LED1, LOW);
      digitalWrite(YELLOW_LED2, LOW);
    }
  } else {
    if (analogValue < 127) {
      if (colorString == "B") {
        digitalWrite(BLUE_LED1, HIGH);
        digitalWrite(BLUE_LED2, HIGH);
      } else if (colorString == "G") {
        digitalWrite(GREEN_LED1, HIGH);
        digitalWrite(GREEN_LED2, HIGH);
      } else if (colorString == "Y") {
        digitalWrite(YELLOW_LED1, HIGH);
        digitalWrite(YELLOW_LED2, HIGH);
      } else if (colorString == "R") {
        digitalWrite(RED_LED1, HIGH);
        digitalWrite(RED_LED2, HIGH);
      }
    } else {
      if (colorString == "B") {
        digitalWrite(BLUE_LED1, LOW);
        digitalWrite(BLUE_LED2, LOW);
      } else if (colorString == "G") {
        digitalWrite(GREEN_LED1, LOW);
        digitalWrite(GREEN_LED2, LOW);
      } else if (colorString == "Y") {
        digitalWrite(YELLOW_LED1, LOW);
        digitalWrite(YELLOW_LED2, LOW);
      } else if (colorString == "R") {
        digitalWrite(RED_LED1, LOW);
        digitalWrite(RED_LED2, LOW);
      }
    }
  }
  
  delay(1000);
}
