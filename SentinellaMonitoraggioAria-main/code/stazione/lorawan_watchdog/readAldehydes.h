#ifndef readAldehydes_h
#define readAldehydes_h

#include <Arduino.h> 

#define dataPinA A3 // pin connected to the Arduino board

#define RL 20  // il valore sdel resistore RL è 47 kohm
#define M -0.447 // il valore del coefficiente angolare calcolato. (Dava un problema con 'm' minuscola, non so il perchè)
                 // Punti: (x1;y1) e (x2;y2) --> (397.27;1.50) e (1005.26;0.99) 
#define b 1.34 // il valore dell'intercetta calcolato.
               // Punto: (x;y) --> (700.0072;1.1679)
#define Ro 4.10 // il valore calcolato (con il programma apposito) di Ro

// MQ137:
float readAldeidi(){
  float analog_value; // ci servirà per ottenere la media delle letture analogiche del sensore, da convertire poi in tensione
  float VRL; // caduta di tensione su RL
  float Rs; // valore di resistenza del resistore ad una determinata concentrazione di gas 
  float ratio; // variabile per il rapporto Rs/Ro
  Serial.println(" MQ138 => Detecting aldehydes concentration.. ");

  for(int i = 1 ; i <= 500 ; i++){ // lettura dell'uscita analogica del sensore per 500 volte
    analog_value = analog_value + analogRead(dataPinA); // somma dei valori anologici letti
  }
  
  analog_value = analog_value/500.0; // media valori analogici campionati
  VRL = analog_value*(5.0/1023.0); // conversione valore analogico in tensione (digitale)
  // Se si volesse evitare il ciclo: VRL = analogRead(MQ_sensor)*(5.0/1024.0); (misura della caduta di tesnione e sua conversione tra 0V-5V)
  // Rs = ((Vc/VRL)-1)*RL o Rs = [(Vc*RL)/VRL]-RL è la formula che è stata ottenuta dall'analisi del circuito equivalente del sensore MQ137
  Rs = ((5.0*RL)/VRL)-RL;
  ratio = Rs/Ro;  // rapporto Rs/Ro, non in aria pulita
  // Calcolo della concentrazione (in ppm) di ammoniaca tramite la "formula inversa".
  // La formula da cui si deriva la seguente è ottenibile analizzando il grafico associato al sensore MQ137 ed è: y = m*x + b, ovvero, essendo la scala del grafico log-log, log(y) = m*log(x) + b
  // dove: y = rapporto Rs/Ro
  //       x = concentrazione in ppm
  //       m = coefficiente angolare
  //       b = intercetta
  float ppm = pow(10, ((log10(ratio)-b)/(M)));

  delay(2000);
  return ppm;  
}

#endif
