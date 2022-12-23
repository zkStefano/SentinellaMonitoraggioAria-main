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

String conf_data; //Configuration string received in the downlink process [1|1|1|1|1|1|1|1|20000|30000]
String msg; //Used for uplink

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
bool activeGPS; // GPS 
bool lowBattery; // Low Battery.. we may not need this.


// Other Variables
int cycle = 1;  //Just for debug purposes.


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
  

 
  // Gateway connection 
  Serial.println("Arduino => Trying to connect with Gateway..");
  int connected = connect_to_gateway(appEui, appKey);
  if (!connected) 
  {
    // Error
    connect_to_gateway_message_error();
    while (1) {}
  }
  Serial.println("Arduino => Connected to gateway....");
  modem.minPollInterval(60);  // Set polling interval to 60 s
  // NOTA: indipendentemente da questa impostazione il modem
  // non permetterà di inviare più di un messaggio ogni 2 minuti,
  // questo è impostato dal firmware e non può essere modificato.
  Serial.println();
 

Serial.println();
}


// LOOP:
void loop() { //read data from sensors --> msg --> conf-data



  // Assigning variable millis();
  currentMillis = millis();
  


  // UPLINK DATA
  // UPLINK PHASE 1
  if( (unsigned long)(currentMillis-previousMillisS) >= timetosend )
  {
    Serial.println();
    Serial.println("----------------- CYCLE => " + String(cycle) + " -----------------" );
    String msg = read_data_from_sensor();
    Serial.println(msg); //String version 
    /*msg = base64::encode(msg); //base64
    Serial.println(msg); //base64 version of MSG*/
    String prova = "HELLO"; //debug ... it works sometimes.. find it in DEVICE DATA under DATA... 
    /*LETTURA STRINGA CONF_DATA input,
    se CONF_DATA
    */
    cycle++; // if you want to separate the phases just uncomment next line
    //}
  
  // UPLINK PHASE 2
  // SEND DATA TO THE GATEWAY
    int err = send_data_to_gateway(msg); //change to msg then
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
  



  /* DOWNLINK DATA Simulated ... getting conf_data via console 
  if( (unsigned long)(currentMillis-previousMillisR) >= timetoreceive )
  { 

    Serial.println("Looking for configuration data..");  
    //Serial.print(" Buffer characters => "); active this during debugging
    //Serial.println(Serial.available()); active this during debugging

    while (Serial.available() != 0)   { // Wait for user input       
    conf_data = Serial.readString(); //we expect a string like this [1,1,1,1,1,1,1,1,30000,50000]
    Serial.println(conf_data); //Check if payload reads more than 64 bytes.

    // ---> DOWNLINK ARRAY <--- 1 COMPACT ARRAY [1,1,1,1,1,1,1,0,30000,50000]
    if(conf_data != "") //we check if there is something to read
    {
      Serial.println(conf_data);
      set_conf_data_SIM(conf_data); //we call the new simulated function here to parse the data
      Serial.println("Configuration finished..");
    }
    


    // 2 EXTENDED ARRAY [Pm10:0|Temperature:1|Humidity:1|Ozone:0|Benzene:1|Ammonia:1|Aldehydes:0|Gps:1|tts:30000|ttr:50000]  
    if(conf_data != "")
    {
      Serial.println(conf_data);
      set_conf_data_SIM2(conf_data);
      Serial.println("Configuration finished..");
    }
   


    //handle millis() rollover
    
    previousMillisR = currentMillis;
    }
  }
  */



  
  //DOWNLINK DATA Original
  if( (unsigned long)(currentMillis-previousMillisR) >= timetoreceive )
  { 
    // Look for configuration data from gateway
    conf_data = exchange_data_with_gateway();
    if(conf_data != "") // ccheck if conf_data is not NULL
    {
      Serial.println(conf_data); //string to parse
      delay(1000);
      Serial.println(conf_data[0]);
      delay(2000);
      set_conf_data_SIM(conf_data);
    }

    //handle millis() rollover
    previousMillisR = currentMillis;
  }
     
}
