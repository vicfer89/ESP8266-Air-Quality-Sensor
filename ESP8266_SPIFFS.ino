/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>
#include "CCS811.h"
#include <string.h>

CCS811 sensor;


// Replace with your network credentials
const char* ssid = "VFF_ESP8266";
const char* password = "123456789";

// Set LED GPIO
const int ledPin = 2;
// Stores LED state
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String getECO2() {
  float eCO2 = (float) sensor.getCO2PPM();
  Serial.println(eCO2);
  return String(eCO2);
}
  
String getTVOC() {
  float TVOC = (float) sensor.getTVOCPPB();
  Serial.println(TVOC);
  return String(TVOC);
}

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if (var == "ECO2_LECTURE"){
    return getECO2();
  }
  else if (var == "TVOC_LECTURE"){
    return getTVOC();
  }
}
 
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Initialize the sensor
  /*if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }*/

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  delay(100);

  // Connect to Wi-Fi
  //WiFi.mode(WIFI_AP);
  //WiFi.begin(ssid, password);
  /*while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    delay(1000);
  }*/
  WiFi.softAP(ssid, password);

  // Print ESP32 Local IP Address
  Serial.println(WiFi.softAPIP());

   while(sensor.begin() != 0){
        Serial.println("failed to init chip, please check if the chip connection is fine");
        delay(1000);
    }

     sensor.setMeasCycle(sensor.eCycle_250ms);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/eco2_lecture", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getECO2().c_str());
  });
  
  server.on("/tvoc_lecture", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getTVOC().c_str());
  });

  // Start server
  server.begin();
}
 
void loop(){
  
}
