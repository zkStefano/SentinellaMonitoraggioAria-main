# Una sentinella IoT adattativa per il monitoraggio della qualità dell’aria

Progettazione di una sentinella per la misurazione della concentrazione degli inquinanti nell'aria. La sentinella è realizzata con una scheda Arduino MKR 1300, e con montati i seguenti moduli:
* Bread Board
* Antenna LoRa
* DHT22 <- Temperatura + Umidità
* PM10 <- PM10
* MQ131 <- Ozono
* MQ135 <- Benzene
* MQ137 <- Ammonia
* MQ138 <- Aldeidi
* NEO BLOX 6M <- GPS


## Struttura del codice

Il codice è composto da diversi file, in particolare:
* lorawan_watchdog.ino <-- Esso rappresenta il punto di partenza del programma. Vengono dichiarate le variabili globali, importate le librerie e gli altri moduli ed eseguito il setup() e il loop() dello sketch stesso.
* arduino_secrets.h e sensorsStates.h <-- Questi due files contengono delle strutture dati usate globalmente. in particolare arduino_secrets contiene i dati sensibili appEui e appKey del progetto, mentre sensorsStates contiene un insieme di array che serve ad alleggerire il codice.
* functions.h <-- Questo file contiene la dichiarazione e definizione di tutte le funzioni necessarie
* tutto il resto (readPM10, readTemp..) <--  Per ciascun valore da rilevare abbiamo creato un file a parte per gestire meglio le funzioni.


## Posizione Sensori

Siccome abbiamo scelto una soluzione posizionale, ecco di seguito l'ordine dei sensori nei vari array di sensorsStates.h
0 - PM10
1 - Temp
2 - Hum
3 - Ozone
4 - Benzene
5 - Ammonia
6 - Aldehydes
7 - GPS


## Uplink / Downlink:

Per la comunicazione con l'application-server tramite il gateway usiamo LoRaWaN. E' necessario usare delle stringhe costruite a modo per evitare errori.
L'Uplink, gestito autonomamente dalla sentinella, produce un messaggio, che viene poi criptato in base 64 di questo tipo:

0.62|19.40|50.00|0.05|302320|12.34|6.29|45.80|9.80|99.80|   //ogni valore è un float

L'ordine segue quello di posizione sensori. Notare che 45.80 e 9.80 sono i due valori Latitudine e Longitudine (il sensore Gps richiede due valori) e l'ultimo rappresenta il valore, simulato della batteria. 

------------------------------------------------

Per quanto riguarda il downlink, la sentinella è in grado di ricevere un messaggio del tipo

1|1|1|1|1|1|1|1|20000|30000 

i primi 8 valori rappresentano se si vuole leggere o meno il sensore, seguono l'ordine di Posizione Sensori. Essi possono essere o 0 o 1, altri valori vengono scartati. Gli ultimi due invece rappresentano il tts e ttr (rispettivamente time to send e time to receive) che possono essere compresi tra 10000 e 200000, altrimenti vengono scartati.
Avevo previsto anche il ricevimento di altri due possibili tipi di downlink.


## Prossimi aggiornamenti

Il codice ha bisogno di rifinitura e pulizia in alcune parti, di rimozione delle vecchie variabili, però è funzionante.

## Crediti
Il progetto (lato sentinella) è stato realizzato da Stefano Cattaneo, partendo dal codice realizzato prima da Samuel Locatelli e poi da Matteo Pettovello.
Il lato server (gateway + application server) viene sviluppato e gestito da Filippo Barbieri e Lorenzo Mazzola
Il tutto coordinato dalla professoressa Patrizia Scandurra.
