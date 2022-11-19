/*
  Lora send data
*/

// libraries
#include <MKRWAN.h>
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
#include "sensorsStates.h"

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
bool activePM; // PM10
bool activeT; // Temperature 
bool activeH; // Humidity
bool activeO; // Ozone
bool activeB; // Benzene
bool activeAM; // Ammonia
bool activeAL; // Aldehydes
bool activeGPS; // GPS ((ADDED... TO TEST))
bool lowBattery; //low Battery state


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
  activeO = false; //riattivarlo quando funziona
  activeB = true;
  activeAM = true;
  activeAL = true;
  activeGPS = false; //((ADDED)) --> change to true when you active GPS
  //sensorsStates[8] = {activePM,activeT,activeH,activeO,activeB,activeAM,activeAL,activeGPS}; //THIS GETS DECLARED AND INITIALIZED IN THE SAME PLACE;
  lowBattery = false; //((ADDED)) <20%

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
void loop() { //read data from sensors --> msg --> conf-data



  // Assigning variable millis();
  currentMillis = millis();
  


  // UPLINK DATA
  if( (unsigned long)(currentMillis-previousMillisS) >= timetosend )
  {
    Serial.println();
    Serial.println("----------------- CYCLE => " + String(cycle) );
    String msg = read_data_from_sensor();
    Serial.println(msg);
    /*LETTURA STRINGA CONF_DATA input,
    se CONF_DATA
    */
    cycle++;
  }



  /*
  // SEND DATA TO THE GATEWAY
    int err = send_data_to_gateway(msg);
    // Check error code
    if (err > 0) 
    {
      //Return correct value
      message_sent_ok();
    } 
    else 
    {
      //Return error message
      message_sent_error();
    }

    //to handle millis() rollover
    previousMillisS = currentMillis;
  }
  */



  //DOWNLINK DATA Simulated ... getting conf_data via console (simulation... max 64 bytes per read)
  if( (unsigned long)(currentMillis-previousMillisR) >= timetoreceive )
  { 

    Serial.println("Looking for configuration data..");  
    //Serial.print(" Buffer characters => "); active this during debugging
    //Serial.println(Serial.available()); active this during debugging

    while (Serial.available() != 0)   { // Wait for user input       
    conf_data = Serial.readString(); //we expect a string like this [1,1,1,1,1,1,1,1,30000,50000]
    if(conf_data != "") //we check if there is something to read
    {
      Serial.println(conf_data);
      set_conf_data_SIM(conf_data); //we call the new simulated function here to parse the data
      Serial.println("Configuration finished..");
    }

    //handle millis() rollover
    previousMillisR = currentMillis;
    }
  }



  /**
  //DOWNLINK DATA Original
  if( (unsigned long)(currentMillis-previousMillisR) >= timetoreceive )
  { 
    // Ottengo conf_data
    conf_data = exchange_data_with_gateway();
    // controllo: se conf_data non è nulla
    if(conf_data != "")
    {
      set_conf_data(conf_data);
    }

    //handle millis() rollover
    previousMillisR = currentMillis;
  }
*/     
}
