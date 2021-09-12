#include "AQS_Getters.h"

CCS811 sensor;
Adafruit_AHT10 aht;
Adafruit_BMP280 bmp;

/* Variables privadas para el fichero */
sensors_event_t humidity, temp;
int eCO2_LastLecture = 0;
int TVOC_LastLecture = 0;
int HR_LastLecture = 0;
int TEMP_LastLecture = 0;
int eCO2_MAX = 0;
int TVOC_MAX = 0;
int temperature = 0;
int humedad = 0;

String getECO2() {  
  eCO2_LastLecture = (int) sensor.getCO2PPM();
  return String(eCO2_LastLecture);
}
  
String getTVOC() {
  TVOC_LastLecture = (int) sensor.getTVOCPPB();
  return String(TVOC_LastLecture);
}

String getHR() {
   aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data    
   HR_LastLecture = humidity.relative_humidity;   
  return String(HR_LastLecture);
}
  
String getTEMP() {
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data    
  TEMP_LastLecture = temp.temperature;
  return String(TEMP_LastLecture);
}

String getECO2_Max() { 
  eCO2_MAX = max(eCO2_LastLecture, eCO2_MAX);
  return String(eCO2_MAX);
}
  
String getTVOC_Max() {
  TVOC_MAX = max(TVOC_LastLecture, TVOC_MAX);
  return String(TVOC_MAX);
}

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
  else if(var == "HR_LECTURE"){
    return getHR();
  }
  else if (var == "TEMP_LECTURE"){
    return getTEMP();
  }
}