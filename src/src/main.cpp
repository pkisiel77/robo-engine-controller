#include <Arduino.h>
#include <WiFiNINA.h>
#include "register.h"
#include "mecanum.h"

byte* registerState;
WiFiServer server(2137);
Mecanum drivetrain;
bool running;
float x, y, z;

void setup() {
  Serial.begin(9600);
  Serial.println(WiFi.beginAP("Srakofski Janurz", "RobotJanusz420"));
  server.begin();
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  registerState = new byte[2];
  registerState[0] = 0;
  registerState[1] = 0;
  running = false;
  x = 0;
  y = 0;
  z = 0;
}

float protoToFloat(float x) {
    return (x - 0.0) * (1.0 - (-1.0)) / (255.0 - 0) + (-1.0);
}

void loop() {
  WiFiClient client = server.available();
  if (!client || client.connected()) goto robotcode;
  if (client.available() > 3) {
    if (client.read() != 0x11) goto robotcode;
    switch (client.read()) {
      case 0x20:
        running = client.read() == 0x03;
        client.read();
        break;
      case 0x01:
        break;
      case 0x02: {
        switch (client.read()) {
          case 0x00:
            z = protoToFloat(client.read());
            break;
          case 0x02:
            x = protoToFloat(client.read());
            break;
          case 0x03:
            y = protoToFloat(client.read());
            break;
        }
        break;
      }
    }
  }

  robotcode:
  if (running) {
    drivetrain.drive(y, x, z, 0);
  }
}
