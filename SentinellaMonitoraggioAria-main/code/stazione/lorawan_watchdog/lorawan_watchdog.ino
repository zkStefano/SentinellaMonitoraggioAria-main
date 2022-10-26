/*
  Lora send data
*/

// Inclusione libreria MKRWAN.h
#include <MKRWAN.h>
// Inclusione moduli arduino_secrets.h, functions.h e dei moduli per la gestione dei sensori
#include "arduino_secrets.h"
#include "functions.h"
#include "readTemperature.h"
#include "readHumidity.h"
#include "readPM.h"
#include "readOzone.h"
#include "readBenzene.h"
#include "readAmmonia.h"
#include "readAldehydes.h"

// Dichiarazione di modem
LoRaModem modem;
// Ottenimento dei valori di appEui e appKey da arduino_secrets.h
String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

// VARIABILI PER IL TUNING TEMPORALE
// Dichiarazione di currentMillis
unsigned long currentMillis;
// Dichiarazione di previousMillisS (invio)
unsigned long previousMillisS;
// Dichiarazione di previousMillisR (controllo ricezione)
unsigned long previousMillisR;

// VARIABILI PER LA RICONFIGURAZIONE
// Dichiarazione di conf_data
String conf_data; //stringacomandi (DA GATEWAY) [(timetosend,30000),(..,..)]

//Period
// Dichiarazione di timetosend
int timetosend;
// Dichiarazione di timetoreceive
int timetoreceive;

//Flags
// Temperature Sensor
bool activeT;
// Sensore Umidità
bool activeH;
// Sensore PM10
bool activePM;
// Sensore Ozono
bool activeO;
// Sensore Benzene
bool activeB;
// Sensore Ammoniaca
bool activeA;
// Sensore Aldeidi
bool activeAL;

//Other Variables
int cycle = 1; 

// SETUP:
void setup() {
  
  Serial.begin(115200); 
  while (!Serial);
  // inizializzazione libreria e modulo MKRWAN
  if (!modem.begin(EU868)) {
    // se inizializzazione fallisce
    modem_start_err();
    while (1) {}
  };

  // Info modem (modem version, modem deviceEUI)
  info_modem();

  // Inizializzazione di timetosend (in ms)
  timetosend = 20000;
  // Inizializzazione di timetoreceive (in ms)
  timetoreceive = 40000;
  // Inizializzazione di previousMillisS (invio)
  previousMillisS = 0;
  // Inizializzazione di previousMillisR (controllo ricezione)
  previousMillisR = 0;
  // Inizializzazione di activeT (attivazione/disattivazione funzionalità sensore)
  activeT = true;
  // Inizializzazione di activeH (attivazione/disattivazione funzionalità sensore)
  activeH = true;
  // Inizializzazione di activePM (attivazione/disattivazione funzionalità sensore)
  activePM = true;
  // Inizializzazione di activeO (attivazione/disattivazione funzionalità sensore)
  activeO = true;
  // Inizializzazione di activeB (attivazione/disattivazione funzionalità sensore)
  activeB = true;
  // Inizializzazione di activeA (attivazione/disattivazione funzionalità sensore)
  activeA = true;
  // Inizializzazione di activeAL (attivazione/disattivazione funzionalità sensore)
  activeAL = true;
/*  
  // Connessione con il gateway
  int connected = connect_to_gateway(appEui, appKey);
  if (!connected) 
  {
    // messaggio di errore connessione con gateway
    connect_to_gateway_message_error();
    while (1) {}
  }
  // Imposta intervallo di polling a 60 secondi
  modem.minPollInterval(60);
  // NOTA: indipendentemente da questa impostazione il modem
  // non permetterà di inviare più di un messaggio ogni 2 minuti,
  // questo è impostato dal firmware e non può essere modificato.
  Serial.println();
 */  

Serial.println();
}

// LOOP:
void loop() {

  // Assegnazione variabile currentMillis
  currentMillis = millis();
  Serial.println("----------------- CYCLE " + cycle + " ----------------------------");
  // UPLINK DATA
  if( (unsigned long)(currentMillis-previousMillisS) >= timetosend )
  {
    // Lettura dati dal sensore
    // Stringa con i dati da trasmettere al gateway
    String msg = read_data_from_sensor();
    /*LETTURA STRINGA CONF_DATA input,
    se CONF_DATA
    */
    Serial.println(msg);
  }
/*
  // invio dati al gateway
    int err = send_data_to_gateway(msg);
    // Controllo codice errore
    if (err > 0) 
    {
      // Procedura che restituisce il messaggio di invio corretto
      message_sent_ok();
    } 
    else 
    {
      // Procedura che restituisce il messaggio di invio errato
      message_sent_error();
    }

    // Eguaglio previousMillisS (invio) con currentMillis
    // per gestire il rollover di millis()
    previousMillisS = currentMillis;
  }
  



  //DOWNLINK DATA Simulated (Using https://www.c-sharpcorner.com/article/reading-input-from-serial-monitor-in-arduino/)
  if( (unsigned long)(currentMillis-previousMillisR) >= timetoreceive )
  { 
    ///// GETTING conf_data VIA CONSOLE (SIMULATING IT)  
    Serial.println("Set the conf_data parametres");  
    while (Serial.available() == 0)   
    { //Wait for user input  }       //WANT TO HAVE [1,1,1,1,1,1,0,23400,53000]
    conf_data = Serial.readString(); 
    
    ///CHECK
    if(conf_data != "")
    {
      // Associazione dei parametri di riconfigurazione 
      // presenti in conf_data alle relative variabili globali
      set_conf_data(conf_data);
    }

    // Eguaglio previousMillisR (controllo ricezione) con currentMillis
    // per gestire il rollover di millis()
    previousMillisR = currentMillis;
  }



  //DOWNLINK DATA Original
  if( (unsigned long)(currentMillis-previousMillisR) >= timetoreceive )
  { 
    // Ottengo conf_data
    conf_data = exchange_data_with_gateway();
    // controllo: se conf_data non è nulla
    if(conf_data != "")
    {
      // Associazione dei parametri di riconfigurazione 
      // presenti in conf_data alle relative variabili globali
      set_conf_data(conf_data);
    }

    // Eguaglio previousMillisR (controllo ricezione) con currentMillis
    // per gestire il rollover di millis()
    previousMillisR = currentMillis;
  }
*/     

  cycle++;
  Serial.prinln();
  Serial.println();
}
