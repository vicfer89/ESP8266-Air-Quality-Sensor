/*
  Victor Fernández Fernández
  Air Quality Sensor AP - ESP826 & CCS811

  Conexionado:
    I2C BUS:
      + SCL -> D1
      + DSA -> D2
    Alimentaciones:
      + AHT10: +3.3 V
      + BMP280: +3.3 V
      + CCS811: + 5 V (Vin)
    CCS811:
      + Wake -> GND
      + Int -> N/C
      + RST -> N/C
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>
#include "CCS811.h"
#include "AHT10.h"
#include <string.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_AHT10.h>



#define SENSOR_CICLE 300

CCS811 sensor;
Adafruit_AHT10 aht;
Adafruit_BMP280 bmp; //bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)

int eCO2_LastLecture = 0;
int TVOC_LastLecture = 0;
int eCO2_MAX = 0;
int TVOC_MAX = 0;
int temperature = 0;
int humedad = 0;

// IP Address configuration

// Replace with your network credentials
const char* ssid = "AirQualitySensor";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

int last_time = 0;

String getECO2() {  
  return String(eCO2_LastLecture);
}
  
String getTVOC() {
  return String(TVOC_LastLecture);
}

String getECO2_Max() {  
  return String(eCO2_MAX);
}
  
String getTVOC_Max() {
  return String(TVOC_MAX);
}

// Replaces placeholder with LED state value
String processor(const String& var){
  if (var == "ECO2_LECTURE"){
    return getECO2();
  }
  else if (var == "TVOC_LECTURE"){
    return getTVOC();
  }
  else  if (var == "ECO2_MAX"){
    return getECO2_Max();
  }
  else if (var == "TVOC_MAX"){
    return getTVOC_Max();
  }
}
 
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Initialize FileSystem
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  delay(100);

  WiFi.softAP(ssid);

  // Print ESP32 Local IP Address
  Serial.println(WiFi.softAPIP());

   while(sensor.begin() != 0){
        Serial.println("failed to init chip, please check if the chip connection is fine");
        delay(1000);
    }

  // Initialize Sensor CCS811
  sensor.setMeasCycle(sensor.eCycle_250ms);

   while(aht_sensor.begin() != 0)
   {
        Serial.println("failed to init AHT chip, please check if the chip connection is fine");
        delay(1000);
   }

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

  server.on("/eco2_max", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getECO2_Max().c_str());
  });
  
  server.on("/tvoc_max", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getTVOC_Max().c_str());
  });

  // Start server
  server.begin();
}
 
void loop(){

  if(abs(millis() - last_time) > SENSOR_CICLE)
  {
    eCO2_LastLecture = (int) sensor.getCO2PPM();
    TVOC_LastLecture = (int) sensor.getTVOCPPB();
    eCO2_MAX = max(eCO2_LastLecture, eCO2_MAX);
    TVOC_MAX = max(TVOC_LastLecture, TVOC_MAX);
  }
}
