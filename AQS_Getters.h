#ifndef __AQS_Getters_H__
#define __AQS_Getters_H__

#include <Wire.h>
#include "CCS811.h"
#include <string.h>
#include <Adafruit_BMP280.h> // Sensor de presión y temperatura BMP 280
#include <Adafruit_AHT10.h> // Sensor de temperatura y humedad AHT 10

extern CCS811 sensor;
extern Adafruit_AHT10 aht;
extern Adafruit_BMP280 bmp;

String getECO2(void);
String getTVOC(void);
String getHR(void);  
String getTEMP(void);
String getECO2_Max(void);  
String getTVOC_Max(void);
String processor(const String& var);

#endif