/*
  Lora send data
*/

// Include MKRWAN.h library
#include <MKRWAN.h>
// Inclusione arduino_secrets.h, functions.h and sensor modules
#include "arduino_secrets.h"
#include "functions.h"
#include "readTemperature.h"
#include "readHumidity.h"
#include "readPM.h"
#include "readOzone.h"
#include "readBenzene.h"
#include "readAmmonia.h"
#include "readAldehydes.h"
#include "readGPS.h"
// Modem declaration
LoRaModem modem;
// Reading arduino_secrets.h values
String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;


/* TEMPORAL TUNING VARIABLES DICHIARATION */
unsigned long currentMillis;
unsigned long previousMillisS;
unsigned long previousMillisR;


/* RICONFIGURATION VARIABLES */

String conf_data; //stringacomandi (DA GATEWAY) [(timetosend,30000),(..,..)]

//Period
int timetosend;
int timetoreceive;

//Sensor Flags
bool activeT; // Temperature 
bool activeH; // Humidity
bool activePM; // PM10
bool activeO; // Ozone
bool activeB; // Benzene
bool activeA; // Ammonia
bool activeAL; // Aldehydes
bool activeGPS; // GPS ((ADDED... TO TEST))


// OTHER VARIABLES
int cycle = 1; 


// SETUP:
void setup() {
  
  Serial.begin(115200); 
  while (!Serial);
  // initialize MRKWAN module
  if (!modem.begin(EU868)) {
    // if initialization fails..
    modem_start_err();
    while (1) {}
  };

  // get modem info (modem version, modem deviceEUI)
  info_modem();

  /* VARIABLES INITIALIZATION (All of these variables may change every time we read conf_data )*/
  
  timetosend = 20000; // ms
  timetoreceive = 40000; // ms
  previousMillisS = 0; // send - invio
  previousMillisR = 0; // data reception - ricezione (?)
  activeT = true;
  activeH = true;
  activePM = true;
  activeO = true;
  activeB = true;
  activeA = true;
  activeAL = true;
  activeGPS = true; //((ADDED))

/*  
  // Gateway connection
  int connected = connect_to_gateway(appEui, appKey);
  if (!connected) 
  {
    // Error
    connect_to_gateway_message_error();
    while (1) {}
  }
  // Set polling interval to 60 s
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

  // Assigning variable millis();
  currentMillis = millis();
  
  // UPLINK DATA
  if( (unsigned long)(currentMillis-previousMillisS) >= timetosend )
  {
    // Lettura dati dal sensore
    // Stringa con i dati da trasmettere al gateway
    Serial.println("----------------- CYCLE => " + String(cycle) );
    String msg = read_data_from_sensor();
    /*LETTURA STRINGA CONF_DATA input,
    se CONF_DATA
    */
    Serial.println(msg);
    cycle++;
    Serial.println();
    Serial.println();
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
}
