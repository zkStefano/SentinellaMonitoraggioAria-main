#ifndef readBenzene_h
#define readBenzene_h

#include <Arduino.h> // necessario includere la libreria se si vogliono utilizzare funzioni di arduino come digitalWrite()...
#include <MQ135.h>

#define dataPinB A2 // il pin di input analogico per la lettura del sensore

// MQ135:
MQ135 mq135_sensor(dataPinB);

// MQ135:
float readBenzene(){
  float temperature = readTemp(); // rileva la temperatura corrente (questo è un valore casuale). E' consgliato misurarla con il sensore DHT22.
  float humidity = readHum(); // rileva l'umidità corrente (questo è un valore casuale). E' consgliato misurarla con il sensore DHT22.
  // Per un corretto utilizzo ed una accurata lettura della concentrazione è importante caibrare il sensore.
  // Per fare ciò è necessario posizionare il sensore all'esterno, in un luogo in cui vi sia abbastanza aria pulita/fresca (idealmente una temperatura di 20°C con un 33% di umidità, in accordo con il datasheet), lasciarlo alimentato
  // per 12-24 ore così da "bruciarlo" e stabilizzarlo.
  // A questo punto sarà possibile leggere il valore calibrato della resistenza RZero (sempre all'esterno), con il comando:
  //float rzero = gasSensor.getRZero();
  // OSS: il miglior modo per fare tutt ciò è fare una media di letture multiple per combattere il rumore dell'ADC.
  // Terminato il processo di calibrazione, dovremo solo passare il valore di rzero al costruttore in questo modo:
  //MQ135 mq135_sensor(PIN_MQ135, rzero);
  Serial.println(" MQ135 => Detecting benzene concentration.. ");


  float correctionFactor = mq135_sensor.getCorrectionFactor(temperature, humidity);
  float rload = mq135_sensor.getResistance();
  float correctedRLoad = mq135_sensor.getCorrectedResistance(temperature, humidity);
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);

  delay(2000);
  return correctedPPM;
}

#endif
