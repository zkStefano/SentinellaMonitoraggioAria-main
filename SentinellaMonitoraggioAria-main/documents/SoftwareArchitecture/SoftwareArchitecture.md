# :classical_building: Software Architecture
L'architettura software del sistema per la gestione della sentinella di monitoraggio è una architettura relativamente semplice, in quanto si rifà allo stile architetturale *Main Program with Subroutines*. Questo pattern, infatti, è quello che meglio si adatta al nostro caso, essendo uno schema molto utilizzato per applicazioni di rilevazione, estrazione, elaborazione dati.

La caratteristica principale di questo stile architetturale è l'avere un "main program" che "gestisce" una serie variabile, in numero, di funzioni, con scopi/obbiettivi differenti, ma complementari, "passando" ogni volta il controllo dell'esecuzione a questi sottomoduli; al termine di ogni operazione il controllo ritorna nuovamente al programma principale che "sceglierà", poi, a chi concederlo.

Come possiamo capire da questa proprietà, il sistema è stato modellato secondo una gerarchia di funzioni, risultato della scomposizione funzionale di un unico grande problema complesso che, idealmente, conteneva tutte le funzionalità richieste. I connettori più utilizzati sono, quindi, le *procedure calls* (con eventuali *nested calls*), che "mettono in comunicazione" due componenti permettendo lo sfruttamento di un servizio, e il sopra descritto trasferimento del controllo.
Il programma che deriva da questo pattern architetturale non è "piatto", bensì, come già accennato, avrà una struttura gerarchica, rappresentabile tramite un grafico ad albero, ed anche, da un certo punto di vista, caratterizzata da alcuni layer; si avranno, infatti, gruppi di funzioni "immediatamente sotto" il main program, che ne chiameranno altre per svolgere servizi sempre più "elementari", con l'obbiettivo finale di avere a disposizione dati ben struttrati da utilizzare.

Nel nostro caso particolare, il main program è rappresentato dal programma <code>lorawan_watchdog</code> che contiene tutte le chiamate a funzioni necessarie per realizzare il compito della sentinella; tra queste funzioni abbiamo:
* **read_data_from_sensor()**, è la funzone che ha il compito di prendere i dati misurati dai vari sensori, mostrarli a video e concatenarli nella stringa **msg** da inviare al gateway. Non è richiesto alcun parametro e viene restituita, appunto, la variabile **msg**;
* **send_data_to_gateway()**, è una funzione (a cui è necessario fornire come parametro la stringa **msg** da inviare al gateway) che evoca sequenzialmente <code>modem.beginPacket()</code>, <code>modem.print(str)</code>, <code>modem.endPacket()</code> che, rispettivamente, avviano il proceso di invio di un pacchetto LoRaWAN, aggiungono caratteri al pacchetto da spedire, completano il processo di trasmissione del pacchetto resituendo un valore intero (1 se il pacchetto è stato inviato correttamente, 0 se si è verificato un errore);
* **connect_to_gateway()**, è una funzione che ha lo scopo di conettere la sentinella al gateway evocando la funzione <code>modem.joinOTAA(appEui, appKey)</code> fornendo come parametri le stringhe **appEui**,**appKey**. In aggiunta, tenta di connettersi alla rete LoRaWAN utilizzando l'attivazione OTA. I valori che ritorna possono essere 1, se la connessione è avvenuta correttamente, 0 altrimenti;
* **exchange_data_with_gateway()**, è una funzione che ha lo scopo di ottenere dal gateway i parametri per la riconfigurazione della sentinella. Ritorna, se disponibile, la stringa contenente i parametri di riconfigurazione, **conf_data**;
* **set_conf_data()**, è una funzione che, ottenuto come parametro di ingresso **conf_data**, analiza la stringa e assegna ad ogni variabile globale il nuovo valore corrispondente;
* **modem_start_err()**, è una procedura che viene invocata quando si verifica un errore di inizializzazione della libreria e del modulo MKRWAN;
* **info_modem()**, è una procedura che stampa sul monitor seriale la verione del modulo e il devEui del dispositivo utilizzato.
* **connect_to_gateway_message_error()**, è una procedura che informa sulla fallimentare connessione al gateway; 
* **message_sent_ok()**, è una prcedura che informa sulla corretta trasmissione del messaggio al gateway;
* **message_sent_error()**, è una proedura che infroma sull'errata trasmissone del messaggio al gateway.

Alcune funzioni, poi, possono a loro volta richiamare altre funzioni per chiedere un ulteriore servizio, necessario per il loro corretto funzionamento; questo è, ad esempio, il caso della funzione <code>read_data_from_sensor</code> che
chiama le seguenti funzioni:
* **readTemp()**, per la rilevazione della temperatura;
* **readHum()**, per la rilevazione della percentuale di umidità;
* **readPM()**, per la rilevazione della concentrazione di particolato nell'aria;
* **readOzono()**, per la rilevazione della concentrazione di ozono;
* **readBenzene()**, per la rilevazione della concentrazione di benzene;
* **readAmmoniaca()**, per la rilevazione della concentrazione di ammoniaca nell'aria atmosferica;
* **readAldeidi()**, per la rilevazione della concentrazione di aldeidi in aria.

Come si è potuto notare, questo patern architteturale fornisce al sistema una struttura molto semplice, tuttavia, permette anche di aumentare il valore di una qualità che, come visto nel documento di <code>[Software Quality](SoftwareQuality.md)</code>, è di grande importanza, ovvero la modularità del codice; infatti, sfruttando questo stile, è possibile suddividere il programma in più moduli richiamabili quando necessario, ma anche riutilizzabili per altre applicazioni simili.
