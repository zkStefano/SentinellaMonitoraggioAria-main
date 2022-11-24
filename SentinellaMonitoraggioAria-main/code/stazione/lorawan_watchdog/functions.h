#ifndef functions_h
#define functions_h

#include "readTemperature.h"
#include "readHumidity.h"
#include "readPM.h"
#include "readOzone.h"
#include "readBenzene.h"
#include "readAmmonia.h"
#include "readAldehydes.h"
#include "readGPS.h"
#include <DHT.h>
#include <MQ131.h>
#include <MQ135.h>
#include "sensorsStates.h" //((ADDED))

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
extern bool activeAM;
// activeAL (param3)
extern bool activeAL;
// activeGPS ((ADDED))
extern bool activeGPS;
// summarize all the sensors States ((ADDED in sensorStates.h))
// lowBattery ((ADDED.. WHEN BATTERY <20.00))
extern bool lowBattery;


/* Variables + Initiliazing Values */
String statoT = "0";
String statoH = "0";
String statoPM = "0";
String statoO = "0";
String statoB = "0";
String statoAM = "0";
String statoAL = "0";
//ADDED 
String statoGPS = "0"; 
String dataGPS[] = {"",""}; //GPS  Values (latitude, longitude)
float battery = 100; //simulate the battery value -- every time we execute read data -0.5% (0.5% per loop)
bool calibrateO = false;// FALSE - MQ131 doesn't get calibrated || TRUE -  MQ131 gets calibrated the first time



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

//EXTRA
// Parse Data 
String getValue(String data, char separator, int index);

// Get number of separators (used for integrity)
int countCheck(String data, char separator);



/* Defining Functions */

String read_data_from_sensor() // was String
{
  float DHTValues[2] = {0.00,0.00}; //DHTValues is a buffer array used to avoid duplicate reading from DHT22.. Basically, we execute readTemp() and readHum() before 
                                     //readBenzene() so we can just keep them  in this array and pass them as parametres later. This should solve the problem. 
                                     //We only need a two values array, we initialize it at (0,00;0,00) first value.. TEMP, second value... HUM
  String GPSValues[2] = {"",""}; //GPSValues will store Latitude and Longitude that we will get from the function readGPS() ((ADDED))

  if(sensorsActiveFlags[0]){
    statoPM = String(readPM(), 2); 
    sensorsValues[0] = statoPM; //EXTRA ALTERNATIVE
  }
  
  if(sensorsActiveFlags[1]){
    statoT = String(readTemp(), 2); 
    DHTValues[0] = statoT.toFloat();
    sensorsValues[1] = DHTValues[0]; //EXTRA ALTERNATIVE
  }

  if(sensorsActiveFlags[2]){
    statoH = String(readHum(), 2);
    DHTValues[1] = statoH.toFloat();
    sensorsValues[2] = DHTValues[1]; //EXTRA ALTERNATIVE
  }

  if(sensorsActiveFlags[3]){
    if(calibrateO){
      statoO = String(readOzono(calibrateO), 2);  //with this if block we calibrate the Ozone sensor just on the first cycle.
      calibrateO = false; 
    }
    statoO = String(readOzono(calibrateO), 2);
    sensorsValues[3] = statoO; //EXTRA ALTERNATIVE
  }
  
  if(sensorsActiveFlags[4]){
    statoB = String(readBenzene(DHTValues[0],DHTValues[1]), 2); 
    sensorsValues[4] = statoB; //EXTRA ALTERNATIVE
  }

  if(sensorsActiveFlags[5]){
    statoAM = String(readAmmoniaca(), 2);
    sensorsValues[5] = statoAM; //EXTRA ALTERNATIVE
  }
  
  if(sensorsActiveFlags[6]){
   statoAL = String(readAldeidi(), 2); 
   sensorsValues[6] = statoAL; //EXTRA ALTERNATIVE
  }

  if(sensorsActiveFlags[7]){ //TRYING TO HANDLE STATE CHECK WITH JUST AN ARRAY
    statoGPS = readGPS();
    GPSValues[0] = getValue(statoGPS, '|', 0);
    GPSValues[1] = getValue(statoGPS, '|', 1);

  }

  String msg = "PM10:" + sensorsValues[0] + "pcs/0.01cf " + "Temperature:" + statoT + "°C " + "Humidity:" + statoH + "% " +  "Ozone:" + statoO + "ppm " + "Benzene:" + statoB + "ppm " + "Ammonia:" + statoAM + "ppm " + "Aldehydes:" + statoAL + "ppm " + "Latitude: " + GPSValues[0] + "° " + "Longitude: " + GPSValues[1] + "° " + "Battery: " + battery + "% ";
  //String msg = "Temperature:" + statoT + "°C " + "Humidity:" + statoH + "% " + "PM10:" + sensorsValues[0] + "pcs/0.01cf " + "Ozone:" + statoO + "ppm " + "Benzene:" + statoB + "ppm " + "Ammonia:" + statoAM + "ppm " + "Aldehydes:" + statoAL + "ppm " + "Latitude: " + GPSValues[0] + "° " + "Longitude: " + GPSValues[1] + "° " + "Battery: " + battery + "% ";
  battery = battery - (((((double) rand() / (RAND_MAX)))+1)*0.25); //randomizza lo scaricamento tra -0.25 e -0.5
  /*ALTERNATIVA
  String msg = buildMSG();
  battery = battery - (((((double) rand() / (RAND_MAX)))+1)*0.25);
  resetValues();
  */ 
  //if (battery < 20.00){
    //lowBattery = true;
  //}
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
void set_conf_data_SIM(String data)  //SPLIT THE STRING BY ',' 
{
 String part2 = data.substring(16); //we create a substring of data.  EX: DATA is [1,1,1,1,1,1,1,0,50000,80000] => PART2 is [50000,80000]
 String tts = ""; //since we do not know the lenght of tts  we build it by reading them. They might have 4,5,6 characters so we build a dynamic string
 String ttr = ""; //since we do not know the lenght of ttr  we build it by reading them. They might have 4,5,6 characters so we build a dynamic string
 bool over = false; //this flag let us know when have read the whole tts and we should start with ttr;
 int result = 0;
 
 /* DEBUG STRINGS
 Serial.print("Pre Splitting: " + data + "   LENGTH: ");
 Serial.println( data.length()); */
 if (!((data.length() < 24 || data.length() > 35) && (countCheck(data,',') < 9))){  //part 1: edit this condition value (sx = 24, dx = 35) like this: -add sensors (+2 dx), -remove sensors (-2 sx) .... part 2 : for each sensor we expect "0/1" and ',' so assuming Z = nSensors + 1 .. it should be always be verified data.count(',')> Z
 for (int i = 0; i < 17 ; i = i +2){
   //First 8 Sensors states (PM10,TEMP,HUM,OZONE,BENZENE,AM,AL,GPS)
   if (i < 16) {
    result=data.substring(i,i+1).toInt();
    if (result == 0) {
       sensorsActiveFlags[i/2]  = false;
    }  
    else{
       sensorsActiveFlags[i/2] = true;
    }
    Serial.println("State of " + sensorsNames[i/2] + " sensor => "  + sensorsActiveFlags[i/2]);
    //Serial.println(getSensInfos[i/2]);
   }
   //tts
   if(i>15) { //LAST CYCLE --- I only need to read the last two values which are stored in "part2"
    for (int j = 0; j < part2.length(); j++){ 
      if (over == false){
        if(part2[j] == ','){
          over = true;
        } 
        else{
          tts += part2[j];
        } 
      }
      else{
        ttr += part2[j];
      }

    }
    Serial.println("tts => " + tts);
    timetosend = tts.toInt();
    Serial.println("ttr => " + ttr);
    timetoreceive = ttr.toInt();
   }
 }
 }
 else{
  Serial.println("Configuration String is incorrect, please retry..");
 }
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

////FUNZIONI EXTRA AGGIUNTE 

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

//countCheck will be rewritten in checkConfData
int countCheck(String data, char separator)
{
  int countSeparators = 0;
  for (int i = 0; i < data.length(); i++){
    if (data.charAt(i) == separator){ countSeparators ++; }
  }
  return countSeparators;
}

//resetValues: after each cycle it automatically resets the sensorsValues array.
void resetValues(){
  for (i= 0; i < sensorsValues.lenght(); i++){
    sensorsValues[i] = "-";
  }
}

//buildMSG:
String buildMSG(){
  String ms = ""; //initialize ms
  //Sensors - GPS
  for (i = 0; i< sensorsValues.lenght()-1;i++){
    ms = ms + getSensInfos(i);
  }
  //GPS
  ms = ms + "Latitude:"+GPSValues[0]+"|"+"Longitude:"+GPSValues[1]+"|";
  //Battery
  ms = ms + "Battery:"+battery+"|";
  //other to add... power signal.. gateway communicating to..?
  Serial.println(ms);
  return ms;
}



/* IDEAS AND FUTURE IMPLEMENTATIONS 


WE NEED A BATTERY TO MAKE ARDUINO INDEPENDENT .
https://www.instructables.com/Powering-Arduino-with-a-Battery/

CHECK BATTERY LEVEL:
https://forum.arduino.cc/t/battery-level-check-using-arduino/424054/3
https://forum.arduino.cc/t/measuring-the-battery-voltage-using-the-adc-on-mini-3v3-8mhz/422944

SIGNAL POWER:
https://forum.arduino.cc/t/reading-rf-signal-strength/576273/13

COMUNICAZIONE CON ARDUINO TRAMITE IL SERIAL MONITOR: https://www.youtube.com/watch?v=qCjCRBLv_VM
Approfondimento su Serial.Read (array di 64 bit per il trasferimento PC -> ARDUINO) e Serial.available (numero byte nel buffer)
String to int: https://docs.arduino.cc/built-in-examples/strings/StringToInt
.H per permettere di usare l'array https://stackoverflow.com/questions/7670816/create-extern-char-array-in-c

https://stackoverflow.com/questions/58919042/im-having-an-2d-array-redefinition-error ---> if i would like to use sensorStates for all the files link it with #include to lorawan_watchdog.ino and then i could use extern (to try)


/* IDEAS ...

1) Low Battery Mode --> WHAT WE CAN DO HERE? Slow Down (tts*2,ttr*2... but this may interfere with the conf_data read and so be useless..)
Example
Initial Scenario: Battery 100%, tts: 20k ms, 40kms;
Simulate Conf_data received at Battery 21%:  set tts: 30k ms, 50k ms;
Low Battery activated: Battery 20%, tts: 40k ms, 80k ms;
Simulate Cond_data received at Battery 19%: set tts: 35k ms, 60k ms;      <------ BUT WE CANNOT GO BELOW 40k/80k 

2) Can't we aggregate all 'StatoH,StatoT,StatoPM10' in an array structure in sensorStates?
Please see sensorsStates.h

*/
