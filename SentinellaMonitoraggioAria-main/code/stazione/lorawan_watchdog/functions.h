#ifndef funzioni_h
#define funzioni_h

#include "readTemp.h"
#include "readHum.h"
#include "readPM.h"
#include "readOzono.h"
#include "readBenzene.h"
#include "readAmmoniaca.h"
#include "readAldeidi.h"

#include <DHT.h>
#include <MQ131.h>
#include <MQ135.h>

/* dichiarazione variabili esterne */
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

/* Dichiarazione variabili */
String statoT = "0";
String statoH = "0";
String statoPM = "0";
String statoO = "0";
String statoB = "0";
String statoA = "0";
String statoAL = "0";

/* dichiarazione funzioni e procedure */

// lettura dati dal sensore
String read_data_from_sensor();

// invio dati al gateway
int send_data_to_gateway(String msg);

// connessione con il gateway
int connect_to_gateway(String ae, String ak);

// ottenimento della stringa di riconfigurazione (conf_data) inviata dal gateway
String exchange_data_with_gateway();

// analizza conf_data e assegna ogni valore contunuto nella stringa 
// alla relativa variabile globale
void set_conf_data(String cd);

// messaggio di errore inizializzazione libreria e modulo MKRWAN
void modem_start_err();

// messaggi di info modem(modem version, modem deviceEUI)
void info_modem();

// messaggio di errore connessione con gateway
void connect_to_gateway_message_error();

// messaggio di invio corretto
void message_sent_ok();

// messaggio di invio errore
void message_sent_error();

/* implementazioni funzioni e procedure */

// lettura dati dal sensore
String read_data_from_sensor() // era String
{
  if(activePM){
    statoPM = String(readPM(), 3);
  }
  
  if(activeT){
    statoT = String(readTemp(), 3); 
  }

  if(activeH){
    statoH = String(readHum(), 3);
  }

  if(activeO){
    statoO = String(readOzono(), 3);
  }
  
  if(activeB){
    statoB = String(readBenzene(), 3);
  }

  if(activeA){
    statoA = String(readAmmoniaca(), 3);
  }
  
  if(activeAL){
   statoAL = String(readAldeidi(), 3); 
  }

  String msg = "Temperatura:" + statoT + "°C " + "Umidità:" + statoH + "% " + "PM10:" + statoPM + "pcs/0.01cf " + "Ozono:" + statoO + "ppm " + "Benzene:" + statoB + "ppm " + "Ammoniaca:" + statoA + "ppm " + "Aldeidi:" + statoAL + "ppm";

  return msg;
}

// invio dati al gateway
int send_data_to_gateway(String msg)
{
  modem.beginPacket();
  modem.print(msg);
  return modem.endPacket(true);
}

// connessione con il gateway
int connect_to_gateway(String ae, String ak)
{
  // connessione e associazione con il gateway
  return modem.joinOTAA(ae, ak);
}

// ottenimento della stringa di riconfigurazione (conf_data) inviata dal gateway
String exchange_data_with_gateway()
{ 
  // controllo che ci siano dati disponibili per essere letti
  // se non sono disponibili, ritorno una stringa vuota
  if (!modem.available())
  {
    Serial.println();
    Serial.println("Nessun messaggio di downlink ricevuto in questo momento");
    return "";
  }
  
  // se sono disponibili, ritorno la stringa ricevuta in downlink dal gateway
  char rcv[64];
  int i = 0;
  while (modem.available()) 
  {
    rcv[i++] = (char)modem.read();
  }
    
  // stampo nel serial monitor quello che viene ricevuto dal gateway (a scopo di debug)
  Serial.println();
  Serial.print("Ricevuto: ");

  // stringa di ritorno ricevuta dal gateway
  return rcv;
}

// analizza conf_data e assegna ogni valore contunuto 
// nella stringa alla relativa variabile globale
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

// messaggio di errore inizializzazione libreria e modulo MKRWAN
void modem_start_err()
{
  Serial.println("Impossibile avviare il modulo, riprovare");
}

// messaggi di info modem(modem version, modem deviceEUI)
void info_modem()
{
  Serial.print("La versione del tuo modulo è: ");
  Serial.println(modem.version());
  Serial.print("L' EUI del tuo dispositivo è: ");
  Serial.println(modem.deviceEUI());
}

// messaggio di errore connessione con gateway
void connect_to_gateway_message_error()
{
  Serial.println("Qualcosa è andato storto; sei al chiuso? Avvicinati ad una finestra e riavvia Arduino");
}

// messaggio di invio corretto
void message_sent_ok()
{
  Serial.println("Messaggio inviato correttamente!");
}

// messaggio di invio errore
void message_sent_error()
{
  Serial.println("Errore durante l'invio del messaggio :(");
  Serial.println("(puoi inviare un numero limitato di messaggi al minuto, a seconda della potenza del segnale");
  Serial.println("può variare da 1 messaggio ogni paio di secondi a 1 messaggio ogni minuto)");
}

#endif
