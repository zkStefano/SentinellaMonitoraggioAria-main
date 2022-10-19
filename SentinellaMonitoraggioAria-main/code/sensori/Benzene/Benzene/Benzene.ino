/*
   Arduino - Sensore Benzene MQ135
*/

#include <MQ135.h>

#define PIN_MQ135 A2 // il pin di input analogico per la lettura del sensore

MQ135 mq135_sensor(PIN_MQ135); 

float temperature = 21.0; // rileva la temperatura corrente (questo è un valore casuale). E' consgliato misurarla con il sensore DHT22.
float humidity = 25.0; // rileva l'umidità corrente (questo è un valore casuale). E' consgliato misurarla con il sensore DHT22.

void setup() {
  // Per un corretto utilizzo ed una accurata lettura della concentrazione è importante caibrare il sensore.
  // Per fare ciò è necessario posizionare il sensore all'esterno, in un luogo in cui vi sia abbastanza aria pulita/fresca (idealmente una temperatura di 20°C con un 33% di umidità, in accordo con il datasheet), lasciarlo alimentato
  // per 12-24 ore così da "bruciarlo" e stabilizzarlo.
  // A questo punto sarà possibile leggere il valore calibrato della resistenza RZero (sempre all'esterno), con il comando:
  //float rzero = gasSensor.getRZero();
  // OSS: il miglior modo per fare tutt ciò è fare una media di letture multiple per combattere il rumore dell'ADC.
  // Terminato il processo di calibrazione, dovremo solo passare il valore di rzero al costruttore in questo modo:
  //MQ135 mq135_sensor(PIN_MQ135, rzero); 
  Serial.begin(9600);
}

void loop() {
  float correctionFactor = mq135_sensor.getCorrectionFactor(temperature, humidity);
  float rload = mq135_sensor.getResistance();
  float correctedRLoad = mq135_sensor.getCorrectedResistance(temperature, humidity);
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);

  // Stampa concentrazione Benzene (C6H6)
  Serial.print("MQ135 Correction factor: ");
  Serial.print(correctionFactor);
  Serial.print("\t MQ135 RLoad: ");
  Serial.print(rload);
  Serial.print("\t MQ135 Corrected Rload: ");
  Serial.print(correctedRLoad);
  Serial.print("\t MQ135 RZero: ");
  Serial.print(rzero);
  Serial.print("\t Corrected RZero: ");
  Serial.print(correctedRZero);
  Serial.print("\t Resistance: ");
  Serial.print(resistance);
  Serial.print("\t PPM: ");
  Serial.print(ppm);
  Serial.print("\t Corrected PPM: ");
  Serial.print(correctedPPM);
  Serial.println("ppm");

  delay(300);
}
