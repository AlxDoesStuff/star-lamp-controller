#include <Arduino.h>
#include <WiFi.h>
#include <Credentials.h>
#include <WebServer.h>

WebServer server(80);

// Pins
int motorPin = 19;
// LED Pins
int brightnessPin = 32;
int rPin = 33;
int gPin = 25;
int bPin = 26;
int wPin = 27;

// Set RGBW and brightness values
void setRGBW(int r, int g, int b, int w, int brightness)
{
  digitalWrite(rPin, r);
  digitalWrite(gPin, g);
  digitalWrite(bPin, b);
  digitalWrite(wPin, w);
  analogWrite(brightnessPin, brightness);
}

void handleRequest()
{
}

void setup()
{
  Serial.begin(115200);
  // Register pins
  pinMode(motorPin, OUTPUT);
  pinMode(brightnessPin, OUTPUT);
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  pinMode(wPin, OUTPUT);
  // Begin WiFi
  WiFi.setHostname("ESP32-Starlamp");
  WiFi.mode(WIFI_STA);
  setRGBW(0,1,1,1,1023);
  // Connect
  Serial.print("Connecting to WiFi network ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  setRGBW(1,1,0,1,1023);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting...");
    delay(100);
  }
  // Print local IP
  setRGBW(1,0,1,1,1023);
  Serial.println("Successfully connected to WiFi");
  Serial.print("Local IP Address: ");
  Serial.println(WiFi.localIP().toString());
  // requests
  server.on("/", handleRequest);
  server.begin();
}

void loop()
{
  setRGBW(1,0,1,1,1023);
  digitalWrite(motorPin, HIGH);
  // Check if WiFi is still connected
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi not connected!");
    setRGBW(0,1,1,1,1023);
    WiFi.disconnect();
    WiFi.reconnect();
    setRGBW(1,1,0,1,1023);
    // Wait for reconnection
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Connecting...");
      delay(100);
    }
  }
  server.handleClient();
  delay(100); //tick
}
