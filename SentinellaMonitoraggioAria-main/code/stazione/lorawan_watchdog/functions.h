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
#include "sensorsStates.h" 



/* EXTERN VARIABLES -- IMPORTED FROM LORAWAN_WATCHDOG.INO*/

extern String appEui;
extern String appKey;
extern LoRaModem modem; 
extern int timetosend; 
extern int timetoreceive;
extern int connected;
extern int failedConnection;
extern int failedTriesConnection;

/* Variables + Initiliazing Values */
String statoGPS = "0"; //Get Latitude and Longitude combined. 
int battery = 100; // Simulate the battery draining via this variable
bool calibrateO = true;// FALSE - MQ131 doesn't get calibrated || TRUE -  MQ131 gets calibrated the first time



/* Functions Declarations */ 

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

// Uplink Error Status
void uplink_error_status(int c, int e);

// Get number of a separator in a string 
String getValue(String data, char separator, int index);

// Get number of separators (used for integrity)
int countCheck(String data, char separator);

// Reset sensorsValues
void resetValues();

// Build uplink message string
String buildMSG(String lat,String lon);

// Critical value check/Outlier check
void checkCritical(int pos); //position posizione

// Connection during loop() if: (1). we have not connected at the beginning; (2). we have disconnected <--- how? 3 missed uplinks(?)
void gatewayConnection();


/* Defining Functions */

String read_data_from_sensor() 
{
  float DHTValues[2] = {0.00,0.00}; //DHTValues is a buffer array used to avoid duplicate reading from DHT22.. Basically, we execute readTemp() and readHum() before 
                                     //readBenzene() so we can just keep them  in this array and pass them as parametres later. This should solve the problem. 
                                     //We only need a two values array, we initialize it at (0,00;0,00) first value.. TEMP, second value... HUM
  String GPSValues[2] = {"-","-"}; //GPSValues will store Latitude and Longitude that we will get from the function readGPS() ((ADDED))

  if(sensorsActiveFlags[0]){
    sensorsValues[0] = String(readPM(), 1); 
    checkCritical(0);
    
  }
  
  if(sensorsActiveFlags[1]){
    DHTValues[0] = readTemp() ; 
    sensorsValues[1] = DHTValues[0]; 
    checkCritical(1);
  }

  if(sensorsActiveFlags[2]){
    DHTValues[1]= readHum() ;
    sensorsValues[2] = DHTValues[1]; 
    checkCritical(2);
  }

  if(sensorsActiveFlags[3]){
    /*if(calibrateO){
      sensorsValues[3] = String(readOzono(calibrateO), 1);  //with this if block we calibrate the Ozone sensor just on the first cycle.
      calibrateO = false; 
    }
    sensorsValues[3] = String(readOzono(calibrateO), 1);
    checkCritical(3);*/
  }
  
  if(sensorsActiveFlags[4] and (sensorsActiveFlags[1] and sensorsActiveFlags[2])) {  //Benzene reliable read depends on having the temperature and humidity data, so if we skip Temp or Hum read, we also skip Benzene.
    /*sensorsValues[4] = String(readBenzene(DHTValues[0],DHTValues[1]), 1); 
    checkCritical(4); */
  }

  if(sensorsActiveFlags[5]){
    sensorsValues[5] = String(readAmmoniaca(), 1);
    checkCritical(5);
  }
  
  if(sensorsActiveFlags[6]){
    sensorsValues[6] = String(readAldeidi(), 1); 
    checkCritical(6);
  }

  if(sensorsActiveFlags[7]){ 
    statoGPS = readGPS();
    GPSValues[0] = getValue(statoGPS, '|', 0);
    GPSValues[1] = getValue(statoGPS, '|', 1);
  }

  if(battery <= 0){
    battery = 0;
    timetosend = 0;
    timetoreceive = 0;
    Serial.println("Battery = 0, Arduino turned off...");
  }

  String msg = sensorsValues[0] + '|' + sensorsValues[1] + "|" +  sensorsValues[2] + "|" + sensorsValues[3] + "|" +  sensorsValues[4] + "|" +  sensorsValues[5] + "|" +  sensorsValues[6] + "|" + GPSValues[0] + "|" + GPSValues[1] + "|" + battery;
  //battery = 45;
  resetValues();
  return msg;
}


int send_data_to_gateway(String msg)
{ 
  if(battery > 0){
    battery = battery - (((((double) rand() / (RAND_MAX)))+1)*0.25); //randomizza lo scaricamento tra -0.25 e -0.5
    //battery = battery - (random(100, 500) / 100.0); 
    modem.beginPacket();
    modem.print(msg);
    return modem.endPacket(true);
  }else{
    return 0;
  }
}


int connect_to_gateway(String ae, String ak)
{
  return modem.joinOTAA(ae, ak); // connection and association with the gateway
}


String exchange_data_with_gateway()
{ 
  Serial.println();
  if (!modem.available()) //if there is no available data, return an empty string. 
  {
    Serial.print("No downlink message available at the moment...");
    return "";
  }
  //if some data is available...
  char rcv[55];
  int i = 0;
  while (modem.available()) 
  {
    rcv[i++] = (char)modem.read();
  }
  Serial.println("Received downlink message correctly..");
  String resultD = rcv; //convert char[64] to string  and return string received from the gateway (rcv)
  return resultD;
}


//DOWNLINK PARSING 
void set_conf_data_SIM(String data) 
{

  //variables for a robust downlink check
  int correctValues = 0; //variable for integrity check of downlink
  int newStates[9]; 
  
  delay(2500);
  if (data[0] == '1' or data[0] == '0' ){ // ---------------------- CONF TYPE DOWNLINK... WORKING 100% --------------------------------
    String part1 = ""; //Splitted String
    int seps = countCheck(data,'|'); //count separators
    if (seps == 9){
    for (int k = 0 ; k  <= seps; k++){  // for each "|" separator we divide in two 
      if (k < seps){
      part1 = data.substring(0,data.indexOf("|"));
      data = data.substring((data.indexOf("|") + 1));
      }
      else {
        part1 = data; //if it's the last iteraton of the parsing we only need to set part1 = data since we won't have another | "
      }
      //Serial.println(part1); //debug purposes
      //Serial.println(data); //debug purposes
  
      if (k < 8){
        if ((part1.toInt() == 1) or (part1.toInt() == 0)){
          correctValues++;
          newStates[k] = part1.toInt();
        } 
      }
      if (k == 8){
        if ((part1.toInt() > 10000) and (part1.toInt() < 200000)){
          correctValues++;
          newStates[k] = part1.toInt();
        }
      }
      if (k == 9) {
        if ((part1.toInt() > 10000) and (part1.toInt() < 200000)){
        correctValues++;
        newStates[k] = part1.toInt();
        }
      }
    }
    //Updating effective status of sensors
    if(correctValues == 10){
    for (int k = 0 ; k  <= seps; k++){
     if (k<8){
      sensorsActiveFlags[k] = newStates[k];
    }
    if (k==8){
      timetosend = newStates[k];
    }
    if (k==9) {
      timetoreceive = newStates[k];
    }
    }
    Serial.println("Downlink message accepted..");
    Serial.println("Sensors activated/deactivated as wanted..");
    return;
    
    }
    }
  }
  Serial.println("Invalid downlink message.. ");
  return;
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
  Serial.println("Message successfully sent!");
  return;
}

void message_sent_error()
{
  Serial.println("Error sending the message...  ");
  Serial.println("Remember: You can send a limited number of message per minute, basing on your signal power!");
  Serial.println("Your signal power may range from 1 msg every few seconds to 1 msg per minute.");
  return;
}

//Bug : this function is not working correctly. Prints are working in an unexpected way, printing half of the string. Hypotetic solution: Insert this part of code directly in lorawan_watchdog ino
void uplink_error_status(float c, float e)
{
  float k = c-e;  //korrekt
  float ratioCorr = k/c; // percentage of uplink messages sent
  float ratioErr = e/c; // percentage of uplink messages not sent
  Serial.println(k);
  Serial.println("Uplink summary at cycle : " + String(c));
  delay(2000);
  Serial.println("Total cycles until now : " + String(c));
  delay(2000);
  Serial.println("Correct uplink message sent: " + String(k));
  delay(2000);
  Serial.println("Errors : " + String(e));
  delay(2000);
  Serial.println("Error Ratio : " + String(ratioErr));
  delay(2000);
  Serial.println("Sent Ratio: " + String(ratioCorr));
  return;
}

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

//Check Critical Value and Outliers Values
void checkCritical(int pos){
  //check if its outlier 
  if (((sensorsValues[pos].toFloat()) < (-100*sensorsAlerts[pos])) or ((sensorsValues[pos].toFloat()) > (100 * sensorsAlerts[pos]))){
    Serial.println(sensors[pos] + " -> " + "Outlier value detected for sensor " + sensorsNames[pos] + ": " + String(sensorsValues[pos]));
    //Update outliers.txt
    return;
  }
  //check if its critical
  else if (sensorsValues[pos].toFloat() > sensorsAlerts[pos]){ //LOWER ALERTS
    if (sensorsValues[pos].toFloat() > sensorsCriticalAlerts[pos]){
      Serial.println(sensors[pos] + " -> " + "Alert! Critical value detected for sensor "  + sensorsNames[pos] + ": " + String(sensorsValues[pos]));
      return;
    }
    Serial.println(sensors[pos] + " -> " + "High value detected for sensor "  + sensorsNames[pos] + ": " + String(sensorsValues[pos]));
    return;
  }
  //it is not critical
  return;
}


//Connect to a gateway --
void gatewayConnection(){
  Serial.println("Arduino => Trying to connect with Gateway.."); //If we are in this block it means we are not connected.
  connected = connect_to_gateway(appEui, appKey); //we get 1 or 0 ... 1 means succesfully connected / 0 means we could not reach a gateway
  if (!connected) 
  {
    connect_to_gateway_message_error(); //print out error 
    failedConnection++;
    Serial.println("Cannot reach a nearby gateway..");
    Serial.println("Connection fails: " + String(failedConnection)); 
    return;
    }
  Serial.println("Arduino => Connected to gateway.."); //means we are online
  Serial.println("Since we are connected we set to 0 failedConnections..");
  return;
}

#endif
/* IDEAS SECTION

Insert future upgrades here..

*/
