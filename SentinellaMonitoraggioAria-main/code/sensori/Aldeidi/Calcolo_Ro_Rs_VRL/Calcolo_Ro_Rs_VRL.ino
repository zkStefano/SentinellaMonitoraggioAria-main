/*
   Arduino - Programma che misura il valore di Ro e Rs (e VRL), per una data RL = 20 kohm.
   OSS: Il programma funziona meglio in una stanza con aria fresca, temperatura 20°C, umidità 65%, concentrazione O2 21% (e RL 20 kohm). 
*/

#define RL 20  // la resistenza del resistore vale 20 kohm
void setup()
{
  Serial.begin(9600); 
}

void loop() {
  float analog_value; // ci servirà per ottenere la media delle letture analogiche del sensore, da convertire poi in tensione
  float VRL;
  float Rs;
  float Ro;
  for(int test_cycle = 1 ; test_cycle <= 500 ; test_cycle++) // lettura dell'uscita analogica del sensore per 500 volte
  {
    analog_value = analog_value + analogRead(A3); // somma dei valori anologici letti
  }
  analog_value = analog_value/500.0; // media valori analogici campionati
  VRL = analog_value*(5.0/1023.0); // conversione valore analogico in tensione (digitale)
  //Rs = ((Vc/VRL)-1)*RL o Rs = [(Vc*RL)/VRL]-RL è la formula che è stata ottenuta dall'analisi del circuito equivalente del sensore MQ137
  Rs = ((5.0/VRL)-1) * RL;
  Ro = Rs/9.7; // Rs/Ro vale 9.7 in aria pulita, come si può osservare dal grafico (datasheet MQ138)
  //Stampa del valore di Ro
  Serial.print("Ro at fresh air = ");
  Serial.println(Ro);
  delay(1000); // delay di 1s
}
