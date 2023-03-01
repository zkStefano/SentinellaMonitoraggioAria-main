
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



/* MODEM DECLARATION */
LoRaModem modem;

// Reading arduino_secrets.h values 
String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

/* TEMPORAL TUNING VARIABLES DECLARATION */
unsigned long currentMillis;
unsigned long previousMillisS;
unsigned long previousMillisR;

/* RICONFIGURATION VARIABLES DECLARATION*/
String conf_data; //Configuration string received in the downlink process [1|1|1|1|1|1|1|1|20000|30000]
String msg; //Used for uplink

/* PERIOD VARIABLES DECLARATION */
int timetosend;
int timetoreceive;

/* OTHER VARIABLES DECLARATION*/
int cycle = 1;  //Just for debug purposes.
int lostPacket = 0; //Counts how many packets have not been transmitted from the watchdog to the gateway
int failedConnection = 0; //We count every time we fail to connect to a gateway
int failedTriesConnection = 0;  // Only in setup * to implement* 
int connected = 0;
int notSentStreak =0;

/* SETUP FUNCTION - Initialization of Modem and Variables + Connection */
void setup() {
  
  Serial.begin(115200); 
  while (!Serial){
  if (!modem.begin(EU868)) {   // if initialization fails..
    modem_start_err(); 
    while (1) {}
  };
  info_modem();

  /* VARIABLES INITIALIZATION */
  timetosend = 20000; // ms
  timetoreceive = 40000; // ms
  previousMillisS = 0; // send 
  previousMillisR = 0; // data reception
  
  /* CONNECTION TO GATEWAY */ 
  /* DEACTIVATE THIS BLOCK OF CODE IF YOU WOULD LIKE TO TEST ONLY THE SENSORS */
  delay(10000);
  Serial.println("Pre to While");
  while (failedTriesConnection < 3 and !(connected)) { //We exit this while loop if we connect or we have tried at least 3 times.
    Serial.println("Arduino => Trying to connect with Gateway..");
    connected = connect_to_gateway(appEui, appKey); //we get 1 or 0 ... 1 means succesfully connected / 0 means we could not reach a gateway
    if (!connected) 
    {
      connect_to_gateway_message_error(); //print out error
      failedTriesConnection++;
      delay(60000); //we try to reconnect after 60 seconds, and we repeat this 'while' block of code.
    }
  }
  if (connected) {
  Serial.println("Arduino => Connected to gateway...."); //means we are online
  connected = 1;
  }
  modem.minPollInterval(60);  // Set polling interval to 60 s
  Serial.println();
  }
  
  
}



/* LOOP FUNCTION - */
void loop() { //read data from sensors --> msg --> conf-data

  currentMillis = millis(); // Assigning millis() value to variable 
  
  /* UPLINK DATA - PHASE 1 : READ FROM SENSORS */
  if( (unsigned long)(currentMillis-previousMillisS) >= timetosend )
  {
    Serial.println();
    Serial.println("----------------- CYCLE : " + String(cycle) + " -----------------" );
    String msg = read_data_from_sensor();
    Serial.println(msg); //String version 
    //See Cycle time
    //Serial.print("Total reading time at cycle " + String(cycle) + " : " );
    //Serial.println((currentMillis-previousMillisS));
    cycle++; 
    
   /* UPLINK DATA - PHASE 2 : SEND DATA TO GATEWAY */
    Serial.println(connected);
    if (!(connected)) {
      notSentStreak = 0;  //If we are not connected means that streak is 0.
      modem.restart(); //we need to restart the modem because we have disconnected
      gatewayConnection();
      Serial.println(connected);
      if(connected){
        modem.minPollInterval(60);
      }
      if (!(connected)) { //means that we are not connected again after retrying 1 time
        //msg is not sent.. update notSent.txt
      }
      if (failedConnection == 10){ //change to 1 for debug, 10 for execution -- after 10 failed 
        Serial.println("Terminating script..");
        exit(0); //Stop the script from working
      }
    }
    if(connected){
      failedConnection = 0;
      int err = send_data_to_gateway(msg); //change to msg then
      if (err > 0)  // Check error code
      {
        message_sent_ok(); //message sent succesfully print 
      } 
      else 
      {
        message_sent_error(); //return error message
        lostPacket++; //increase error counter (Sum)
        notSentStreak++;  
        delay(1500);
        float tmpCycle = float(cycle-1);
        float tmpLostPacket = float(lostPacket);
        uplink_error_status(tmpCycle,tmpLostPacket);
        if(notSentStreak == 3){
          connected = 0; //last one
        }
        //message hasn't been sent... update notSent.txt
      }
  
      //to handle millis() rollover
      previousMillisS = currentMillis;
    }
  }
  
  /* DOWNLINK DATA */
  if( (unsigned long)(currentMillis-previousMillisR) >= timetoreceive ) { 
   
    conf_data = exchange_data_with_gateway(); //See if there is a downlink msg available
    if(conf_data != "") // if conf_data is not empty
    {
      Serial.println(conf_data); //string to parse
      delay(1000);
      set_conf_data_SIM(conf_data);
    }

    //handle millis() rollover
    previousMillisR = currentMillis;
  }
     
}
