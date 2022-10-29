#ifndef readPM_h
#define readPM_h

#include <Arduino.h> 

int dataPinPM = 0;

// Grove Dust Sensor PM10:
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 30000; // sampletime is 30s ;
unsigned long lowpulseoccupancy = 0; // represents Low Pulse Occupancy (LPO) got during the 30s sampling;
                                     // LPO: Il sensore, indipendentemente dall'intensità o dalla dimensione delle particelle, conta la quantità di tempo in cui una particella viene vista.
                                     // Questa quantità di tempo, chiamata Low Pulse Occupancy, può essere vista come "Percentuale di opacità" dell'aria che circola attraverso il sensore.
float ratio = 0;
float concentration = 0;

// Grove Dust Sensor PM10:
float readPM(){

  pinMode(dataPinPM,INPUT);
  Serial.println("Groove Dust Sensor => Detecting PM10 concentration..");
  starttime = millis(); //millis() get current time. We associate this value to starttime() to check the delta time elapsed in the while structure.
  while((millis()-starttime) < sampletime_ms){
  duration = pulseIn(dataPinPM, LOW); // pulseIn() legge un impulso (sia di tipo HIGH oppure LOW) su un pin;
                                      // se valore = HIGH allora pulseIn() aspetta che il pin vada nello stato HIGH, comincia a contare, quindi aspetta cha vada nello stato LOW e smette di contare.
                                      // Restituisce la durata dell'impulso in microsecondi (0 se nessun impulso parte entro il timeout prestabilito).
  lowpulseoccupancy = lowpulseoccupancy+duration;
  }

  ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Percentage 0-100 (Integer value)
  concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // According to the graph on the instructions.

  lowpulseoccupancy = 0;
  starttime = millis();
  return concentration;
}

#endif
