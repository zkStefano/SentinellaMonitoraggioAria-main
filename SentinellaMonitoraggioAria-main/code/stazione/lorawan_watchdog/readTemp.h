#ifndef readTemp_h
#define readTemp_h

#include <Arduino.h> // necessario includere la libreria se si vogliono utilizzare funzioni di arduino come digitalWrite()...
#include <DHT.h> // libreria necessaria per il funzionamento del sensore di temperatura (e umidità) DHT22

#define dataPinTU 1 // pin collegato di Arduino (teperatura-umidità) 
#define DHTType DHT22 // definizione modello di sensore (temperatura-umidità)

// DHT22:
DHT dht = DHT(dataPinTU, DHTType); // creazione oggetto per la gestione del sensore

// DHT22 (Temperatura):
float readTemp(){
  dht.begin();
  Serial.println(" DHT22 => Detecting temperature... ");
  
  delay(2000); // si può sondare il DHT22 per nuove informazioni ogni due secondi

  float t = dht.readTemperature(); // lettura della temperatura
    
  return t;
}

#endif
