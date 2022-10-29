#ifndef functions_h
#define functions_h

#include "readTemperature.h"
#include "readHumidity.h"
#include "readPM.h"
#include "readOzone.h"
#include "readBenzene.h"
#include "readAmmonia.h"
#include "readAldehydes.h"

#include <DHT.h>
#include <MQ131.h>
#include <MQ135.h>



/* EXTERN VARIABLES */

// modem
extern LoRaModem modem;
// timetosend (param1)
extern int timetosend;
// timetoreceive (param2)
extern int timetoreceive;
// activeT (param3)
extern bool activeT;
// activeH (param3)
extern bool activeH;
// activePM (param3)
extern bool activePM;
// activeO (param3)
extern bool activeO;
// activeB (param3)
extern bool activeB;
// activeA (param3)
extern bool activeA;
// activeAL (param3)
extern bool activeAL;
// activeGPS ((ADDED))
extern bool activeGPS;



/* Variables */
String statoT = "0";
String statoH = "0";
String statoPM = "0";
String statoO = "0";
String statoB = "0";
String statoA = "0";
String statoAL = "0";
String statoGPS = "0"; //((ADDED))
//bool calibrateO = true; //With this we can execute the Ozone sensor calibration just one time. (1 Time Calibration)
bool calibrateO = false; //With this we can execute the Ozone sensor calibration just one time. (SKIP CALIBRATION)



/* Functions Declarations */ 
//CAN WE REMOVE THE COMMENTS HERE?
// Read all the active sensor data and get an output string.
String read_data_from_sensor();

// Transfer data acquired to gateway
int send_data_to_gateway(String msg);

// Connect to gateway
int connect_to_gateway(String ae, String ak);

// Getting conf_data string
String exchange_data_with_gateway();

// Analyse conf_data and assign each value got from the string to the relative global variable (ActiveT,ActiveO..)
void set_conf_data(String cd);

// Error message: initiliazing library and MRKWAN module
void modem_start_err();

// Get modem info and display it(modem version, modem deviceEUI)
void info_modem();

// Error Message: Connection with gateway
void connect_to_gateway_message_error();

// Message succesfully sent
void message_sent_ok();

// Message failed to send
void message_sent_error();



/* Defining Functions */

String read_data_from_sensor() // was String
{

  float DHTValues[2] = {0.00,0.00}; //DHTValues is a buffer array used to avoid duplicate reading from DHT22.. Basically, we execute readTemp() and readHum() before 
                                     //readBenzene() so we can just keep them  in this array and pass them as parametres later. This should solve the problem. 
                                     //We only need a two values array, we initialize it at (0,00;0,00) first value.. TEMP, second value... HUM
  float GPSValues[2] = {0.00,0.00}; //GPSValues will store Latitude and Longitude that we will get from the function readGPS() ((ADDED))

  if(activePM){
    statoPM = String(readPM(), 3);
  }
  
  if(activeT){
    statoT = String(readTemp(), 3); 
    DHTValues[0] = statoT.toFloat();
  }

  if(activeH){
    statoH = String(readHum(), 3);
    DHTValues[1] = statoH.toFloat();
  }

  if(activeO){
    if(calibrateO){
      statoO = String(readOzono(calibrateO), 3);  //with this if block we calibrate the Ozone sensor just on the first cycle.
      calibrateO = false; 
    }
    statoO = String(readOzono(calibrateO), 3);
  }
  
  if(activeB){
    statoB = String(readBenzene(DHTValues[0],DHTValues[1]), 3); 
  }

  if(activeA){
    statoA = String(readAmmoniaca(), 3);
  }
  
  if(activeAL){
   statoAL = String(readAldeidi(), 3); 
  }

  if(activeGPS){
    //statoGPS = readGPS(); ((ADDED)) DEVO CAPIRE COME FARE LA LETTURA DEL GPS.
  }

  String msg = "Temperature:" + statoT + "°C " + "Humidity:" + statoH + "% " + "PM10:" + statoPM + "pcs/0.01cf " + "Ozone:" + statoO + "ppm " + "Benzene:" + statoB + "ppm " + "Ammonia:" + statoA + "ppm " + "Aldehydes:" + statoAL + "ppm";
  return msg;
}


int send_data_to_gateway(String msg)
{
  modem.beginPacket();
  modem.print(msg);
  return modem.endPacket(true);
}


int connect_to_gateway(String ae, String ak)
{
  return modem.joinOTAA(ae, ak); // connection e association with the gateway
}


String exchange_data_with_gateway()
{ 

  //if there is no available data, return an empty string. 
  if (!modem.available())
  {
    Serial.println();
    Serial.println("No downlink message available at the moment..");
    return "";
  }
  
  //if some data is available...
  char rcv[64];
  int i = 0;
  while (modem.available()) 
  {
    rcv[i++] = (char)modem.read();
  }
    
  //print in the console what we receive from the gateway just for debugging.
  Serial.println();
  Serial.print("Received: ");

  //return string received from the gateway (rcv)
  return rcv;
}



void set_conf_data(String cd)
{
  // indice iniziale, posto all'inizio della stringa
  int m = 0;
  // indice che si posiziona sul primo carattere che fa da separatore (parse)
  int n = cd.indexOf("|");

  // calcolo i successivi passi aggiornando gli indici per il parse:
  int contat = 0;
  // inizio while
  while( n>0 )
  {
    // valore parsato
    String token = cd.substring(n,m);

    m = n+1;
    n = cd.indexOf("|", n+1);

    // elaboro il token che mi serve
    // casi:
    switch(contat)
    {
      // se contat = 0, timetosend (param1)
      case 0:
        Serial.print("timetosend|");
        break;
      // se contat = 1, valore di timetosend (param1)
      case 1:
        // salvo il valore nella variabile di riferimento
        timetosend = token.toInt();
        Serial.print(timetosend);
        break;
      // se contat = 2, timetoreceive (param2)
      case 2:
        Serial.print("|timetoreceive|");
        break;
      // se contat = 3, valore di timetoreceive (param2)
      case 3:
        timetoreceive = token.toInt();
        Serial.print(timetoreceive);
        Serial.println();
        break;
    }

    // incremento il contat
    contat++;
  }
  // fine while
}

 
// NEW VERSION of set_conf_data JUST FOR SIMULATION PURPOSES
void set_conf_data_SIM(String data)
{
 
}
  


void modem_start_err()
{
  Serial.println("Cannot start MRKWAN module, please retry.. ");
}



void info_modem()
{
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("EUI code of your device is: ");
  Serial.println(modem.deviceEUI());
}



void connect_to_gateway_message_error()
{
  Serial.println("Something went wrong; Are you indoor? Please move your Arduino board near a window and restart the script");
}



void message_sent_ok()
{
  Serial.println("Message succesfully sent!");
}



void message_sent_error()
{
  Serial.println("Error sending the message...  ");
  Serial.println("Remember: You can send a limited number of message per minute, basing on your signal power!");
  Serial.println("Your signal power may range from 1 msg every few seconds to 1 msg per minute.");
}

#endif




/* IDEAS AND FUTURE IMPLEMENTATIONS 

REFACTORING: ALL DONE EXPECT set_conf_data WHICH MAY CHANGE

WE NEED A BATTERY TO MAKE ARDUINO INDEPENDENT .
https://www.instructables.com/Powering-Arduino-with-a-Battery/

CHECK BATTERY LEVEL:
https://forum.arduino.cc/t/battery-level-check-using-arduino/424054/3
https://forum.arduino.cc/t/measuring-the-battery-voltage-using-the-adc-on-mini-3v3-8mhz/422944


SIGNAL POWER:
https://forum.arduino.cc/t/reading-rf-signal-strength/576273/13

*/
