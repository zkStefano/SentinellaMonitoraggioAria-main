/*
   Arduino - Sensore di temperatura ed umidità con DHT22
*/

#include <DHT.h>

#define dataPin 1 // pin collegato di Arduino 
#define DHTType DHT22 // definizione modello di sensore

DHT dht = DHT(dataPin, DHTType); // creazione oggetto per la gestione del sensore

void setup() {
    Serial.begin(9600);
    dht.begin();
}

void loop() {
    delay(2000); // si può sondare il DHT22 per nuove informazioni ogni due secondi
   
    float h = dht.readHumidity();

    float t = dht.readTemperature();

    // Controllo di integrità dei valori raccolti, se una variabile non è un numero si torna all'inizio del ciclo e si stampa un messaggio
    if (isnan(h) || isnan(t)) {
        Serial.println("Impossibile leggere i dati del sensore DHT.");
        Serial.println("Controllare il cablaggio.");
        return;
    }

    // Stampa l'umidità
    Serial.print("Humidity: ");
    Serial.print(h);
    // Stampa la temperatura
    Serial.print(" %, Temp: ");
    Serial.print(t);
    Serial.print(" Celsius ");
}   

    
