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
    BMP280:
      +
    AHT10:
      +
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include "AQS_Server.h"
#include "AQS_Getters.h"

#define SENSOR_CICLE 500

void configure_server(void);

// Replace with your network credentials
const char* ssid = "AirQualitySensor";

// Create AsyncWebServer object on port 80

// Configuración de ESP8266 
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

  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);
  delay(100);
  digitalWrite(0, HIGH);
  delay(40);

   while(sensor.begin() != 0){
        Serial.println("failed to init CCS811 chip, please check if the chip connection is fine");
        delay(1000);
    }

  // Initialize Sensor CCS811
  sensor.setMeasCycle(sensor.eCycle_250ms);

   while(aht.begin() != 0)
   {
        Serial.println("failed to init AHT chip, please check if the chip connection is fine");
        delay(1000);
   }

  configure_server();
  // Start server
  server.begin();
}
 
void loop(){
}

void configure_server(void)
{
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

  server.on("/hr_lecture", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getHR().c_str());
  });
  
  server.on("/temp_lecture", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getTEMP().c_str());
  });

  server.on("/eco2_max", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getECO2_Max().c_str());
  });
  
  server.on("/tvoc_max", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getTVOC_Max().c_str());
  });
}
