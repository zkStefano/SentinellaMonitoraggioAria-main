/*
   Arduino - Stazione completa per il monitoraggio della qualità dell'aria
*/

#include <DHT.h>
#include <MQ131.h>
#include <MQ135.h>

#define dataPinTU 1 // pin collegato di Arduino (teperatura-umidità) 
#define DHTType DHT22 // definizione modello di sensore (temperatura-umidità)

#define PIN_MQ135 A2 // il pin di input analogico per la lettura del sensore

#define RL 47  // il valore sdel resistore RL è 47 kohm
#define m -0.263 // il valore del coefficiente angolare calcolato.
                 // Punti: (x1;y1) e (x2;y2) --> (40;1) e (100;0.8) 
#define b 0.42 // il valore dell'intercetta calcolato.
               // Punto: (x;y) --> (70;0.75)
#define Ro 30 // il valore calcolato (con il programma apposito) di Ro
#define MQ_sensor A1 // pin analogico a cui è connesso il sensore

// DHT22:
DHT dht = DHT(dataPinTU, DHTType); // creazione oggetto per la gestione del sensore

// Grove Dust Sensor PM10:
int dataPinPM = 0;
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

// MQ135:
MQ135 mq135_sensor(PIN_MQ135);

float temperature = 21.0; // rileva la temperatura corrente (questo è un valore casuale). E' consgliato misurarla con il sensore DHT22.
float humidity = 25.0; // rileva l'umidità corrente (questo è un valore casuale). E' consgliato misurarla con il sensore DHT22.

void setup() {
    Serial.begin(9600); //Serial.begin(115200); per MQ131, Ozono
    // DHT22:
    dht.begin();
    // Grove Dust Sensor PM10:
    pinMode(dataPinPM,INPUT);
    starttime = millis(); // ottieni il tempo/orario corrente, millis() ritorna il numero di millisecondi dall'avvio del programma;
    // MQ131:
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
    Serial.println("Ohms ");
    Serial.print("Time to heat = ");
    Serial.print(MQ131.getTimeToRead());
    Serial.println("s ");
    // MQ135:
    // Per un corretto utilizzo ed una accurata lettura della concentrazione è importante caibrare il sensore.
    // Per fare ciò è necessario posizionare il sensore all'esterno, in un luogo in cui vi sia abbastanza aria pulita/fresca (idealmente una temperatura di 20°C con un 33% di umidità, in accordo con il datasheet), lasciarlo alimentato
    // per 12-24 ore così da "bruciarlo" e stabilizzarlo.
    // A questo punto sarà possibile leggere il valore calibrato della resistenza RZero (sempre all'esterno), con il comando:
    //float rzero = gasSensor.getRZero();
    // OSS: il miglior modo per fare tutt ciò è fare una media di letture multiple per combattere il rumore dell'ADC.
    // Terminato il processo di calibrazione, dovremo solo passare il valore di rzero al costruttore in questo modo:
    //MQ135 mq135_sensor(PIN_MQ135, rzero);
}

void loop() {
    // DHT22:
    //delay(2000); // si può sondare il DHT22 per nuove informazioni ogni due secondi
   
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
    Serial.print("%, Temperature: ");
    Serial.print(t);
    Serial.print("°C ");

    //Grove Dust Sensor PM10:
    duration = pulseIn(dataPinPM, LOW); // pulseIn() legge un impulso (sia di tipo HIGH oppure LOW) su un pin;
                                  // se valore = HIGH allora pulseIn() aspetta che il pin vada nello stato HIGH, comincia a contare, quindi aspetta cha vada nello stato LOW e smette di contare.
                                  // Restituisce la durata dell'impulso in microsecondi (0 se nessun impulso parte entro il timeout prestabilito).
    lowpulseoccupancy = lowpulseoccupancy+duration;
 
    if ((millis()-starttime) > sampletime_ms) // se il tempo di campionamento è ==30s
    {
        ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Percentuale intera 0-100
        concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // basandosi sulla curva del foglio dele specifiche
        Serial.print("LPO: ");
        Serial.print(lowpulseoccupancy);
        Serial.print(",");
        Serial.print(" Rateo: ");
        Serial.print(ratio);
        Serial.print(",");
        Serial.print(" Concentrazione: ");
        Serial.print(concentration);
        Serial.print("pcs/283ml=0.01cf ");
        lowpulseoccupancy = 0;
        starttime = millis();
    }

    // MQ131:
    //Serial.println("Sampling...");
    MQ131.sample();

    // Il sensore è sensibile alle variazioni ambientali (Temperatura e Umidità). Se si desidera avere valori corretti, è necessario impostare la temperatura e l'umidità prima della chiamata alla funzione getO3()
    // con la funzione setEnv(). La temperatura è in °C e l'midità in %. I valori dovrebbero provenire da un altro sensore come il DHT22.
    //MQ131.setEnv(t,h);
  
    // Stampa concentrazione Ozono (O3) in diverse unità di misura
    Serial.print("Concentration O3 : ");
    Serial.print(MQ131.getO3(PPM));
    Serial.println("ppm ");
    //Serial.print("Concentration O3 : ");
    //Serial.print(MQ131.getO3(PPB));
    //Serial.println(" ppb");
    //Serial.print("Concentration O3 : ");
    //Serial.print(MQ131.getO3(MG_M3));
    //Serial.println(" mg/m3");
    //Serial.print("Concentration O3 : ");
    //Serial.print(MQ131.getO3(UG_M3));
    //Serial.println(" ug/m3");

    //delay(60000);

    // MQ135:
    float correctionFactor = mq135_sensor.getCorrectionFactor(temperature, humidity);
    float rload = mq135_sensor.getResistance();
    float correctedRLoad = mq135_sensor.getCorrectedResistance(temperature, humidity);
    float rzero = mq135_sensor.getRZero();
    float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
    float resistance = mq135_sensor.getResistance();
    float ppmB = mq135_sensor.getPPM();
    float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);
  
    // Stampa concentrazione Benzene (C6H6)
    //Serial.print("MQ135 Correction factor: ");
    //Serial.print(correctionFactor);
    //Serial.print("\t MQ135 RLoad: ");
    //Serial.print(rload);
    //Serial.print("\t MQ135 Corrected Rload: ");
    //Serial.print(correctedRLoad);
    //Serial.print("\t MQ135 RZero: ");
    //Serial.print(rzero);
    //Serial.print("\t Corrected RZero: ");
    //Serial.print(correctedRZero);
    //Serial.print("\t Resistance: ");
    //Serial.print(resistance);
    Serial.print("\t PPM: ");
    Serial.print(ppmB);
    Serial.print("\t Corrected PPM: ");
    Serial.print(correctedPPM);
    Serial.println("ppm");

    //delay(300);

    // MQ137:
    float analog_value; // ci servirà per ottenere la media delle letture analogiche del sensore, da convertire poi in tensione
    float VRL; // caduta di tensione su RL
    float Rs; // valore di resistenza del resistore ad una determinata concentrazione di gas 
    float ratio; // variabile per il rapporto Rs/Ro
  
    for(int i = 1 ; i <= 500 ; i++){ // lettura dell'uscita analogica del sensore per 500 volte
      analog_value = analog_value + analogRead(MQ_sensor); // somma dei valori anologici letti
    }
  
    analog_value = analog_value/500.0; // media valori analogici campionati
    VRL = analog_value*(5.0/1024.0); // conversione valore analogico in tensione (digitale)
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
    float ppmA = pow(10, ((log10(ratio)-b)/m));
    // Stampa concentrazione ammoniaca (NH3) in ppm
    Serial.print("Concentration NH3: ");
    Serial.print(ppmA);
    Serial.print("ppm ");
  
    delay(60000);
}
