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

//gateway variables
int gw = 0; //active gateway number
int gwFails = 0; //number of gateway fails ... might be extern this one
int gwAdded = 0; //number of gateway added


/* Variables + Initiliazing Values */
String statoT = "0"; //we may delete all these
String statoH = "0";
String statoPM = "0";
String statoO = "0";
String statoB = "0";
String statoAM = "0";
String statoAL = "0";
String statoGPS = "0"; 
String dataGPS[] = {"",""}; //GPS  Values (latitude, longitude) *** we may not need this
float battery = 100; //simulate the battery value -- every time we execute read data -0.5% (0.5% per loop)
bool calibrateO = true;// FALSE - MQ131 doesn't get calibrated || TRUE -  MQ131 gets calibrated the first time



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


//EXTRA -- ADDED
// Parse Data 
String getValue(String data, char separator, int index);

// Get number of separators (used for integrity)
int countCheck(String data, char separator);

// Reset sensorsValues
void resetValues();

// Build MSG String
String buildMSG(String lat,String lon);

// UPDATE GW LISTS
void updateGWList(String key);

/* Defining Functions */

String read_data_from_sensor() // was String
{
  float DHTValues[2] = {0.00,0.00}; //DHTValues is a buffer array used to avoid duplicate reading from DHT22.. Basically, we execute readTemp() and readHum() before 
                                     //readBenzene() so we can just keep them  in this array and pass them as parametres later. This should solve the problem. 
                                     //We only need a two values array, we initialize it at (0,00;0,00) first value.. TEMP, second value... HUM
  String GPSValues[2] = {"-","-"}; //GPSValues will store Latitude and Longitude that we will get from the function readGPS() ((ADDED))

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
  
  if(sensorsActiveFlags[4] and (sensorsActiveFlags[1] and sensorsActiveFlags[2])) {  //Benzene reliable read depends on having the temperature and humidity data, so if we skip Temp or Hum read, we also skip Benzene.
    statoB = String(readBenzene(DHTValues[0],DHTValues[1]), 2);  //since we reach this point only if we are reading dht sensor too, then it would be correct to don't use DHTValues[]
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

  Serial.println(gwAdded); //extra debug string to remove
  Serial.println(gw); //extra debug string to remove later
  Serial.println(listAppKey[1]); //check if we got the simulated app key
  /*ALTERNATIVE 1 => [VALUE,VALUE,VALUE] */
  String msg = sensorsValues[0] + '|' + sensorsValues[1] + "|" +  sensorsValues[2] + "|" + sensorsValues[3] + "|" +  sensorsValues[4] + "|" +  sensorsValues[5] + "|" +  sensorsValues[6] + "|" + GPSValues[0] + "|" + GPSValues[1] + "|" + battery;
  battery = battery - (((((double) rand() / (RAND_MAX)))+1)*0.25); //randomizza lo scaricamento tra -0.25 e -0.5
  resetValues();
  /*ALTERNATIVE 2 =>  [NAME:VALUE|]
  String msg = buildMSG(GPSValues[0],GPSValues[1]);
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
  Serial.println("Received downlink message correclty..");
  String resultD = rcv; //convert char[64] to string
  //return string received from the gateway (rcv)
  return resultD;
}


//DOWNLINK PARSING 
// OLD VERSION to parse conf_data 
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

 
// NEW VERSION .... NOT SIMULATED...
void set_conf_data_SIM(String data)  //WE HAVE THREE DIFFERENT TYPE OF DOWNLINK [CONF;ADD;REM] CONF STARTS WITH A NUMBER... ADD  STARTS WITH 'A'... REM STARTS WITH 'R'
{
  delay(4500);
  if (data[0] == '1' or data[0] == '0' ){ // ---------------------- CONF TYPE DOWNLINK... WORKING 100% --------------------------------
    
    String part1 = ""; //Splitted String
    int seps = countCheck(data,'|'); //count separators
    for (int k = 0 ; k  <= seps; k++){  // for each "|" separator we divide in two 
      if (k < seps){
      part1 = data.substring(0,data.indexOf("|"));
      data = data.substring((data.indexOf("|") + 1));
      }
      else {
        part1 = data; //if it's the last iteraton of the parsing we only need to set part1 = data since we won't have another | "
      }
  
  
      Serial.println(part1); //debug purposes
      Serial.println(data); //debug purposes
  
      if (k < 8){
        if ((part1.toInt() == 1) or (part1.toInt() == 0)){
          sensorsActiveFlags[k] = part1.toInt();
        } 
      }
      if (k == 8){
        if ((part1.toInt() > 10000) and (part1.toInt() < 200000)){
          timetosend = part1.toInt() ; 
        }
      }
      if (k == 9) {
        if ((part1.toInt() > 10000) and (part1.toInt() < 200000)){
        timetoreceive = part1.toInt() ; 
        }
      }
    }
  }
  if (data[0] == 'A'){ //---------------------ADD DOWNLINK TYPE STRING -----------------------------------
    String keyGW = data.substring((data.indexOf("|") + 1)); //we get only the key like this
    Serial.println("Extracted Value is: " + keyGW);
    delay(2000);
    updateGWList(keyGW);
  }
  if (data[0] == 'R'){ //----------------------REM DOWNLINK TYPE STRING : FUTURE IMPLEMENTATIONS --------
    String keyGW = data.substring((data.indexOf("|") + 1)); //we get only the key like this
    Serial.println("Extracted Value is:" + keyGW);
    //We need a function
  }
  
  //Serial.println(" INVALID DOWNLINK MESSAGE... ");
  return;
}


/*
void set_conf_data_SIM2(String data)
{
  String part1 = ""; // pre part to split
  int seps = countCheck(data,'|');
  //String divided[seps];  we may not use this
  for (int k = 0 ; k  <= seps; k++){


    if (k < seps){
    part1 = data.substring(0,data.indexOf("|"));
    data = data.substring((data.indexOf("|") + 1));
    }
    else {
      part1 = data;
    }


    Serial.println(part1); //debug purposes
    Serial.println(data); //debug purposes
    Serial.println(part1.substring((part1.indexOf(":") + 1)).toInt()); //debug


    if (k < 8){
      if ((part1.substring((part1.indexOf(":") + 1)).toInt() == 1) or (part1.substring((part1.indexOf(":") + 1)).toInt() == 0)){
        sensorsActiveFlags[k] = part1.substring((part1.indexOf(":") + 1)).toInt() ;
      } 
    }
    if (k == 8){
      if ((part1.substring((part1.indexOf(":") + 1)).toInt() > 10000) and (part1.substring((part1.indexOf(":") + 1)).toInt() < 200000)){
        timetosend = part1.substring((part1.indexOf(":") + 1)).toInt() ; 
      }
    }
    if (k == 9) {
      if ((part1.substring((part1.indexOf(":") + 1)).toInt() > 10000) and (part1.substring((part1.indexOf(":") + 1)).toInt() < 200000)){
      timetoreceive = part1.substring((part1.indexOf(":") + 1)).toInt() ; 
      }
    }
  }
}
*/

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
  for (int i= 0; i < 7 ; i++){  //change 7
    sensorsValues[i] = "-";
  }
}

//buildMSG:
String buildMSG(String lat, String lon){
  String ms = ""; //initialize ms
  //Sensors - GPS
  for (int i = 0; i < 6 ;i++){  //change 6
    ms = ms + getSensInfos(i);
  }
  //GPS
  ms = ms + "Latitude:"+lat+"|"+"Longitude:"+lon+"|";
  //Battery
  ms = ms + "Battery:"+battery+"|";
  //other to add... power signal.. gateway communicating to..?
  Serial.println(ms);
  return ms;
}


//in this foo (called by the arrival of a "specific downlink" (since we have two types of DOWNLINK (CONFIGURATION and ADDGATEWAY and REMOVEGATEWAY) - check if we have room to store it (<= MAXGW) )
void updateGWList(String key){
  Serial.println(gwAdded);
  //Serial.println(MAXGW);
  delay(2000);
  if (gwAdded < 4){ //WE CAN ADD A GATEWAY IF WE HAVEN'T ADD MORE THAN MAXGW ... substitute 4 with MAXGW
    if (gwAdded == 0){
      listAppEui[gwAdded] = "00000000000000000000000000000000";
      listAppKey[gwAdded] = SECRET_APP_KEY;
      delay(1000);
      gwAdded++;
    }
    delay(1000);
    listAppEui[gwAdded] = "00000000000000000000000000000000";
    listAppKey[gwAdded] = key;
    gwAdded++;
    delay(2000);
    Serial.println("Updated list of gateways");
    return;
  }
  else{
    Serial.println("Please remove one of the gateways ... With a downlink messagge type REMOVEGATEWAY");
    return;
  }
  
}


// in this foo we will check if it's the time to connect to a different gateway.
/*
void statusGW(){
  if (gwFails > 3){
    gw++; //change active gateway
    if (listAppEui[gw%MAXGW] == ""){
      statusGW(); //if next element of gw is empty then we re-do the function until we find a gateway where we can connect
    }
    else{
      int connected = connect_to_gateway(listAppEui[gw%MAXGW],listAppKey[gw%MAXGW]) //connect_to_gateway...
      if(!connected){
        connect_to_gateway_message_error();
        gwFails = 4;
        statusGW();
      }
      gwFails = 0;
      Serial.println(" Arduino  => Connected to gateway = " + gw);
      modem.minPollInterval(60);  
      return;
    }
  }
  return;
}
*/



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
