#include <Arduino.h>
#include <WiFi.h>
#include <Credentials.h>
#include <WebServer.h>

WebServer server(80);

// Variables for remembering current light states on on/off
int currentMotorSpeed = 0;
int currentBrightness = 0;
int rCurrent = 1;
int gCurrent = 1;
int bCurrent = 1;
int wCurrent = 0; // on by default
// Pins
int motorPin = 19;
// LED Pins
int brightnessPin = 32;
int rPin = 33;
int gPin = 25;
int bPin = 26;
int wPin = 27;

// Set RGBW, brightness and motor values
void setLights(int r, int g, int b, int w, int brightness, int motorSpeed,bool newCurrent)
{
  if (newCurrent){
    rCurrent = r;
    gCurrent = g;
    bCurrent = b;
    wCurrent = w;
    currentBrightness = brightness;
    currentMotorSpeed = motorSpeed; 
  }
  analogWrite(rPin, r);
  analogWrite(gPin, g);
  analogWrite(bPin, b);
  analogWrite(wPin, w);
  analogWrite(brightnessPin, brightness);
  analogWrite(motorPin, motorSpeed);
}

// Lights on
void lightsOn()
{
  server.send(200);
  setLights(rCurrent, gCurrent, bCurrent, wCurrent, currentBrightness, currentMotorSpeed,false);
}

// Lights off
void lightsOff()
{
  server.send(200);
  setLights(1, 1, 1, 1, 0, 0,false);
}

void setBrightness(int brightness)
{
  // Set LED brightness & save
  currentBrightness = brightness;
  analogWrite(brightnessPin, brightness);
}

void setMotorspeed(int motorspeed)
{
  // Set LED brightness & save
  currentMotorSpeed = motorspeed;
  analogWrite(motorPin, motorspeed);
}

// Color Setting
void setR(int r)
{
  rCurrent = r;
  analogWrite(rPin, r);
}
void setG(int g)
{
  gCurrent = g;
  analogWrite(gPin, g);
}
void setB(int b)
{
  bCurrent = b;
  analogWrite(bPin, b);
}
void setW(int w)
{
  wCurrent = w;
  analogWrite(wPin, w);
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
  // Connect
  Serial.print("Connecting to WiFi network ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  setLights(1023, 1023, 0, 1023, 1023, 1023,true);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting...");
    delay(100);
  }
  // Print local IP
  setLights(1023, 0, 1023, 1023, 1023, 1023,true);
  Serial.println("Successfully connected to WiFi");
  Serial.print("Local IP Address: ");
  Serial.println(WiFi.localIP().toString());
  server.begin();

  // Handle requests

  // Handle on/off requests
  server.on("/on", HTTP_POST, lightsOn);
  server.on("/off", HTTP_POST, lightsOff);
  // Handle set brightness & motorspeed
  server.on("/setbrightness", HTTP_POST, []()
            {
    //Check if has the arg
    if (server.hasArg("brightness")){
      int brightness = server.arg("brightness").toInt();
      //Check if arg is within range
      if(0 <= brightness && brightness <= 1023){
      //If brightness within range, set brightness and OK
      setBrightness(brightness);
      server.send(200);
      }else{
      //If not, Bad Request
      server.send(400);
    }  
     }else{
    //If not, Bad Request
    server.send(400);
  } });

  server.on("/setmotorspeed", HTTP_POST, []()
            {
    //Check if has the arg
    if (server.hasArg("motorspeed")){
      int motorspeed = server.arg("motorspeed").toInt();
      //Check if arg is within range
      if(0 <= motorspeed && motorspeed <= 1023){
      //If brightness within range, set brightness and OK
      server.send(200);
      setMotorspeed(motorspeed);
      }else{
      //If not, Bad Request
      server.send(400);
    }  
     }else{
    //If not, Bad Request
    server.send(400);
  } });

  server.on("/setR", HTTP_POST, []()
            {
    //Check if has the arg
    if (server.hasArg("red")){
      int red = server.arg("red").toInt();
      //Check if arg is within range
      if(0 <= red && red <= 1023){
      //If brightness within range, set brightness and OK
      server.send(200);
      setR(red);
      }else{
      //If not, Bad Request
      server.send(400);
    }  
     }else{
    //If not, Bad Request
    server.send(400);
  } });

  server.on("/setG", HTTP_POST, []()
            {
    //Check if has the arg
    if (server.hasArg("green")){
      int green = server.arg("green").toInt();
      //Check if arg is within range
      if(0 <= green && green <= 1023){
      //If brightness within range, set brightness and OK
      server.send(200);
      setG(green);
      }else{
      //If not, Bad Request
      server.send(400);
    }  
     }else{
    //If not, Bad Request
    server.send(400);
  } });

  server.on("/setB", HTTP_POST, []()
            {
    //Check if has the arg
    if (server.hasArg("blue")){
      int blue = server.arg("blue").toInt();
      //Check if arg is within range
      if(0 <= blue && blue <= 1023){
      //If brightness within range, set brightness and OK
      server.send(200);
      setB(blue);
      }else{
      //If not, Bad Request
      server.send(400);
    }  
     }else{
    //If not, Bad Request
    server.send(400);
  } });

  server.on("/setW", HTTP_POST, []()
            {
    //Check if has the arg
    if (server.hasArg("white")){
      int white = server.arg("white").toInt();
      //Check if arg is within range
      if(0 <= white && white <= 1023){
      //If brightness within range, set brightness and OK
      server.send(200);
      setW(white);
      }else{
      //If not, Bad Request
      server.send(400);
    }  
     }else{
    //If not, Bad Request
    server.send(400);
  } });

  //Full set lights request
  server.on("/setLights", HTTP_POST, []() {
    if(server.hasArg("motorspeed") && server.hasArg("brightness") && server.hasArg("red") && server.hasArg("green") && server.hasArg("blue") && server.hasArg("white")){
      int motorspeed = server.arg("motorspeed").toInt();
      int brightness = server.arg("brightness").toInt();
      int red = server.arg("red").toInt();
      int green = server.arg("green").toInt();
      int blue = server.arg("blue").toInt();
      int white = server.arg("white").toInt();

      if (0 <= motorspeed && motorspeed <= 1023 && 0 <= brightness && brightness <= 1023 && 0 <= red && red <= 1023 && 0 <= green && green <= 1023 && 0 <= blue && blue <= 1023 && 0 <= white && white <= 1023){
        setLights(red,green,blue,white,brightness,motorspeed,true);
        server.send(200);
      }else{
        server.send(400);
      }
    }else{
      server.send(400);
    }
  });
}

void loop()
{
  // Check if WiFi is still connected
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi not connected!");
    WiFi.disconnect();
    WiFi.reconnect();
    // Wait for reconnection
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Connecting...");
      delay(100);
    }
  }
  server.handleClient();
  delay(100); // tick
}
