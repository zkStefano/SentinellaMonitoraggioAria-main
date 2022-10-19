# Class Diagram
Un primo diagramma, che andrà a comporre la *Vista strutturale* della sentinella di monitoraggio, è il **Class Diagram**.

![ClassDiagram](https://user-images.githubusercontent.com/96539194/189480915-80e250a1-18cf-4d54-aa84-8a0e763c65f6.PNG)

In questo Class Diagram, che modella la struttura modulare del sistema, abbiamo 10 classi, che identificano tutti gli "attori" coinvolti nella realizzazione del prototipo richiesto:
* Una classe astratta <code>*Sensore*</code>, non istanziabile, implementata dalle varie sottoclassi che rappresentano i veri e propri sensori:
  * **DHT22**: sensore per la rilevazione della temperatura della percentuale di umidità;
  * **Grove Dust Sensor**: sensore per la rilevazione della concentrazione di PM10;
  * **MQ131**: sensore per la rilevazione della concentrazione di ozono;
  * **MQ135**: sensore per la rilevazione della concentrazione di benzene;
  * **MQ137**: sensore per la rilevazione della concentrazione di ammoniaca;
  * **MQ138**: sensore per la rilevazione dela concentrazione di aldeidi;
* Una classe <code>SentinellaFunzioni</code> che rappresenta la sentinella con tutte le funzoni che propone per la gestione e lo scambio di dati;
* Una classe <code>Main</code> che identifica il programma principale che gestisce la seninella, inzializzandola, connettendola al gateway, verificando che le comuniazioni avvengano correttamente...
* Una classe <code>Gateway</code> che rappresenta il gateway con il quale la sentinella comunica, trasmettendo i dati raccolti e, se disponibili, ricevendo i parametri per la propria riconfigurazione.
