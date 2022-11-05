#ifndef readGPS_h
#define readGPS_h

#include <TinyGPS++.h>
#include <Arduino.h>

#define gpsSerial Serial1
#define debugSerial Serial
#define loraSerial Serial2

TinyGPSPlus gps; //gps object

#define PMTK_SET_NMEA_UPDATE_05HZ  "$PMTK220,2000*1C"
#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

// TTN Mapper stuff
unsigned long last_update = 0;
uint8_t txBuffer[9];
uint32_t latitudeBinary, longitudeBinary;
uint16_t altitudeGps;
uint8_t hdopGps;

float lat = 0.00;
float lon = 0.00;
String combine = ""; //since we cannot pass an array as function result, we pass a string back to functions and then we split there the values.

//FUNCTUIN
String readGPS(){

  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      
      debugSerial.print(F("Location: ")); 
      if (gps.location.isValid()){
        Serial.print(gps.location.lat(), 6);
        Serial.print(F(","));
        Serial.print(gps.location.lng(), 6);
        lat = gps.location.lat();
        lon = gps.location.lng();
        combine = String(lat) + " | ";
        combine = combine + String(lon);
        return combine;
       }
      else{
          Serial.println(F("INVALID"));
      }

    }
  }

 
  

}

#endif

// https://forum.arduino.cc/t/tinygps-location-problem/422301/7
