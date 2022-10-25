#ifndef readPM_h
#define readPM_h

#include <Arduino.h> // necessario includere la libreria se si vogliono utilizzare funzioni di arduino come digitalWrite()...

int dataPinPM = 0;

// Grove Dust Sensor PM10:
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 30000; // campionamento 30s ;
unsigned long lowpulseoccupancy = 0; // rappresenta la Low Pulse Occupancy (LPO) rilevata nei dati 30s
                                     // LPO: Il sensore, indipendentemente dall'intensità o dalla dimensione delle particelle, conta la quantità di tempo in cui una particella viene vista.
                                     // Questa quantità di tempo, chiamata Low Pulse Occupancy, può essere vista come "Percentuale di opacità" dell'aria che circola attraverso il sensore.
                                     // OSS: Per misurare l'LPO di particelle di diverse dimensioni (noi consideriamo le PM10, particolato formato da particelle con dimensioni minori di 10 micrometri), il sensore fornisce
                                     // un ingreso variabile che permette di regolare il filtro passa-banda.
float ratio = 0;
float concentration = 0;

// Grove Dust Sensor PM10:
float readPM(){
  pinMode(dataPinPM,INPUT);
  Serial.println(" Groove Dust Sensor => Detecting concentration of PM10");
  starttime = millis(); // ottieni il tempo/orario corrente, millis() ritorna il numero di millisecondi dall'avvio del programma;
  while((millis()-starttime) < sampletime_ms){
  duration = pulseIn(dataPinPM, LOW); // pulseIn() legge un impulso (sia di tipo HIGH oppure LOW) su un pin;
                                      // se valore = HIGH allora pulseIn() aspetta che il pin vada nello stato HIGH, comincia a contare, quindi aspetta cha vada nello stato LOW e smette di contare.
                                      // Restituisce la durata dell'impulso in microsecondi (0 se nessun impulso parte entro il timeout prestabilito).
  lowpulseoccupancy = lowpulseoccupancy+duration;
  }

  ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Percentuale intera 0-100
  concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // basandosi sulla curva del foglio dele specifiche

  lowpulseoccupancy = 0;
  starttime = millis();
  return concentration;
}

#endif
