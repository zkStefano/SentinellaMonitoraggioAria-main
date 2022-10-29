#ifndef readOzone_h
#define readOzone_h

#include <Arduino.h> // necessario includere la libreria se si vogliono utilizzare funzioni di arduino come digitalWrite()...
#include <MQ131.h>

#define dataPinO A0

// MQ131:
float readOzono(bool calibrate){
  // MQ131:
  // Inizializza/avvia il sensore (necessari i 4 parametri)
  // Controllo (della potenza del) riscaldatore (resistenza) sul pin 2
  // Lettura analogica del sensore sul pin A0 (output)
  // Modello LOW_CONCENTRATION // Il "materiale sensibile" del sensore MQ131 è il WO3 (Ossido di Tungsteno(VI), ovvero Triossido di Tungsteno o Anidride Tungstica), che ha una condttivtà inferiore nell'aria pulita
                               // Quando è presente dell'Ozono, la conduttività del materale diminuisce all'aumentare della concentrazione del gas. Il funzionamento si basa, dunque, sulla conversione
                               // della variazione di conduttività in concentrazione del gas tramite un segnale di uscita. (Per questo si utilizza il modello di sensore LOW_CONCENTRATION) 
  // Resistenza di carico RL di 1MOhms (1000000 Ohms)
  
  MQ131.begin(2, dataPinO, LOW_CONCENTRATION, 1000000); //MODIFICA: FORSE QUESTA è DA METTERE ALL'INTERNO DELLA GUARDIA...

  // Prima di usare il sensore è meglio calibrarlo; la calibrazione è "buona pratica" farla avvenire a 20°C e 65% di umidità in aria pulita.
  if (calibrate){
  Serial.println("Calibration MQ131 in progress...");
  MQ131.calibrate();
  Serial.println("Calibration done!");
  }

  Serial.println("MQ131 => Detecting ozone concentration.. ");

  // La calibrazione "aggiusta" 2 parametri successivi: il valore di R0 (resistenza di base) e il tempo necessario per riscaldare il sensore e ottenere lettre coerenti e affidabili
  /*Serial.print("R0 = ");
  Serial.print(MQ131.getR0());
  Serial.println("Ohms ");
  Serial.print("Time to heat = ");
  Serial.print(MQ131.getTimeToRead());
  Serial.println("s ");*/
  
  //Serial.println("Sampling MQ131... ");
  MQ131.sample();

  // Il sensore è sensibile alle variazioni ambientali (Temperatura e Umidità). Se si desidera avere valori corretti, è necessario impostare la temperatura e l'umidità prima della chiamata alla funzione getO3()
  // con la funzione setEnv(). La temperatura è in °C e l'midità in %. I valori dovrebbero provenire da un altro sensore come il DHT22.
  //MQ131.setEnv(t,h);

  return MQ131.getO3(PPM);
}

#endif