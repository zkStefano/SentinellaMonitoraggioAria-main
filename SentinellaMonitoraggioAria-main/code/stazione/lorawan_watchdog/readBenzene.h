#ifndef readBenzene_h
#define readBenzene_h

#include <Arduino.h> 
#include <MQ135.h>

#define dataPinB A2 // pin connected to the Board

/* MODE 1

// MQ135:
MQ135 mq135_sensor(dataPinB);

// MQ135:
float readBenzene(float Temp, float Hum){
  // To get reliable data reads from the sensor its important to calibrate it correctly.
  // We can do this by placing it outdoor (ideal conditions: 20 degrees, 33% humidity), let it run for 12-24 hours to "burn it" and stabilize it.
  // Then, whilst remaining outdoor, we can read RZero value.
  //float rzero = gasSensor.getRZero();
  // OSS: best way to do this is by doing multiple reads to combat also the ADC noise.
  // After calibration is finished, we will pass RZero to our constructor like this:
  //MQ135 mq135_sensor(PIN_MQ135, rzero);
  Serial.println("MQ135 => Detecting Benzene..  ");

  //Change the values between parenthesis in the following section with the values passed as parametres.
  float correctionFactor = mq135_sensor.getCorrectionFactor(Temp, Hum);
  float rload = mq135_sensor.getResistance();
  float correctedRLoad = mq135_sensor.getCorrectedResistance(Temp, Hum);
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(Temp, Hum);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  //Serial.println(ppm);
  float correctedPPM = mq135_sensor.getCorrectedPPM(Temp, Hum);
  //Serial.println(correctedPPM);

  delay(2000);
  return correctedPPM;
}

*/

/* MODE 2 */
//COSTANTS
const float aBenzene = 23.4461175;
const float bBenzene = -3.98724605;
const float aCor = 0.00035;
const float bCor = 0.02718;
const float cCor = 1.39538;
const float dCor = 0.0018;

//OTHER VARIABLES
int ave = 30;
byte adcBits = 12;
float R0;
float R;
float voltage = 3.3; //check again
float RL = 1; 
float a = 0;
float adcAverage;
float cPPM = 0.001252; //4 ug/m3 come valore standard


// MQ135:
float readBenzene(float Temp, float Hum){
  Serial.println("MQ135 => Detecting Benzene..  ");
  //RESET VALUES
  a = 0; 
  adcAverage = 0; 
  float correctionFactor = aCor * Temp * Temp - bCor * Temp + cCor - (Hum-33.)*dCor; //correction
  for(int i=0;i<ave;i++)
  {
    a += analogRead(dataPinB);
    delay(2);
  }
  adcAverage = a/ave;
  //float rload = mq135_sensor.getResistance();
  //float correctedRLoad = mq135_sensor.getCorrectedResistance(Temp, Hum);
  float resistance = ((5.0/((adcAverage/((pow(2,adcBits)-1)))*voltage)-1.0)*RL); //MANCA RL da settare diverso da 1
  R0 = resistance*pow((cPPM/aBenzene),(1.0/-bBenzene)); //calibrazione
  float ppm = aBenzene * pow((resistance/R0), bBenzene); //MANCA RO
  Serial.println(ppm);
  float correctedPPM = aBenzene * pow(((resistance/correctionFactor)/R0), bBenzene); //manca Ro
  Serial.println(correctedPPM);

  //DEBUG PRINTS
  Serial.println("a value is: " + String(a));
  Serial.println("adcAverage is: " + String(adcAverage));
  Serial.println("resistance is: " + String(resistance));
  Serial.println("r0 is: " + String(R0));
  Serial.println("rL is: " + String(RL));      
  Serial.println("ppm is: " + String(ppm));  
  Serial.println("correctedPPM is: " + String(correctedPPM));
  
  delay(2000);
  return correctedPPM*1000; //use correctedPPM then
}


#endif
