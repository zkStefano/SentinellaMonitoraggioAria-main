/*
 Arduino - Sensore Ozono MQ131 (campionamento ogni 60s)
*/

#include <MQ131.h>

void setup() {
  Serial.begin(115200);

  // Inizializza/avvia il sensore (necessari i 4 parametri)
  // Controllo (della potenza del) riscaldatore (resistenza) sul pin 2
  // Lettura analogica del sensore sul pin A0 (output)
  // Modello LOW_CONCENTRATION // Il "materiale sensibile" del sensore MQ131 è il WO3 (Ossido di Tungsteno(VI), ovvero Triossido di Tungsteno o Anidride Tungstica), che ha una condttivtà inferiore nell'aria pulita
                               // Quando è presente dell'Ozono, la conduttività del materale diminuisce all'aumentare della concentrazione del gas. Il funzionamento si basa, dunque, sulla conversione
                               // della variazione di conduttività in concentrazione del gas tramite un segnale di uscita. (Per questo si utilizza il modello di sensore LOW_CONCENTRATION) 
  // Reistenza di carico RL di 1MOhms (1000000 Ohms)
  MQ131.begin(2,A0, LOW_CONCENTRATION, 1000000);  

  // Prima di usare il sensore è meglio calibrarlo; la calibrazione è "buona pratica" farla avvenire a 20°C e 65% di umidità in aria pulita.
  Serial.println("Calibration in progress...");
  
  MQ131.calibrate();
  
  Serial.println("Calibration done!");

  // La calibrazione "agiusta" 2 parametri successivi: il valore di R0 (resistenza di base) e il tempo necessario per riscaldare il sensore e ottenere lettre coerenti e affidabili
  Serial.print("R0 = ");
  Serial.print(MQ131.getR0());
  Serial.println(" Ohms");
  Serial.print("Time to heat = ");
  Serial.print(MQ131.getTimeToRead());
  Serial.println(" s");
}

void loop() {
  Serial.println("Sampling...");
  MQ131.sample();

  // Il sensore è sensibile alle variazioni ambientali (Temperatura e Umidità). Se si desidera avere valori corretti, è necessario impostare la temperatura e l'umidità prima della chiamata alla funzione getO3()
  // con la funzione setEnv(). La temperatura è in °C e l'midità in %. I valori dovrebbero provenire da un altro sensore come il DHT22.
  //MQ131.setEnv(t,h);
  
  // Stampa concentrazione Ozono (O3) in diverse unità di misura
  Serial.print("Concentration O3 : ");
  Serial.print(MQ131.getO3(PPM));
  Serial.println(" ppm");
  Serial.print("Concentration O3 : ");
  Serial.print(MQ131.getO3(PPB));
  Serial.println(" ppb");
  Serial.print("Concentration O3 : ");
  Serial.print(MQ131.getO3(MG_M3));
  Serial.println(" mg/m3");
  Serial.print("Concentration O3 : ");
  Serial.print(MQ131.getO3(UG_M3));
  Serial.println(" ug/m3");

  delay(60000);
}
