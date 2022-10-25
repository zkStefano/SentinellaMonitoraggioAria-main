#ifndef readHum_h
#define readHum_h

#include <Arduino.h> // necessario includere la libreria se si vogliono utilizzare funzioni di arduino come digitalWrite()...
#include <DHT.h> // libreria necessaria per il funzionamento del sensore di umidità (e temperatura) DHT22

#define dataPinTU 1 // pin collegato di Arduino (teperatura-umidità) 
#define DHTType DHT22 // definizione modello di sensore (temperatura-umidità)

// DHT22:

// DHT22 (Umidità):
float readHum(){
  dht.begin();
  Serial.println(" DH22 => Detecting Humidity.. ");

  
  delay(2000); // si può sondare il DHT22 per nuove informazioni ogni due secondi

  float h = dht.readHumidity(); // lettura della percentuale di umidità
  
  return h;
}

#endif
