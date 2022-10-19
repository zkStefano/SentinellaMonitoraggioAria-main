# State Machine Diagram
Il diagramma utilizzato per modellare il comportamento, e che, quindi, rappresenterà la *Vista comprtamentale* del prototipo, è lo **State Machine Diagram**. Con esso, è possibile, infatti, visualizzare tutti i possibili stati in cui un sistema può trovarsi, mostrare come (a quali condizioni, o al verificarsi di quali eventi), il passaggio da uno stato ad un altro, avviene e quali attività, in seguito ad una transizione, il sistema deve svolgere.

![StateMachineDiagram](https://user-images.githubusercontent.com/96539194/189529081-9974f129-cd8f-41b1-b39d-235f7b9afeab.PNG)

In questo caso particolare, il diagramma è composto da 2 stati "principali", <code>Setup</code> e <code>Main Cycle</code>, uno dei quali è uno stato composito, contenente una serie di sottostati che modellano il flusso di esecuzione del programma che gestice la sentinella.

I primi due stati rappresentano le due funzioni principali di Arduino, ovvero setup() e loop(), dove, la prima, viene generalmente invocata una sola volta all'avvio dello sketch, e verrà eseguita solamente una volta dopo ogni accensione o ripristino della scheda Arduino, mentre, la seconda (una volta impostati i valori all'avvio della funzione setup()) esguirà ripetutamente le istruzioni conenute al suo interno, fino al verificarsi di una determinata condizione che ne interromperà il ciclo.

Nello stato di Setup, dunque, sono contenute le attività di inizializzazione libreria e modulo MKRWAN, delle variabili timetosend, timetoreceive, previousMillisS, previousMillisR e connessione con il gateway, mentre, nello stato Main Cycle, sono comprese tutte le attività che si riferiscono alle fasi di **Uplink** e **Downlink**; vediamole nel dettaglio.

Inizialmente si procede con il setup e la calibrazione (più o meno veloce) di tutti i 7 sensori coinvolti, fase che, terminata, lascerà spazio all'attività di rilevazione dati da parte dei sensori abilitati. Terminata la lettura dei dati, si procede con la loro aggregazione e memorizzazione in una stringa che, ovviamente, dovrà essere spedita al gateway in attesa.

A prescindere dall'esito dell'invio, contemporaneamente, si procede con la verifica della disponibilità di eventuali parametri per la riconfigurazione della sentinella. Nel caso in cui questi dati esistano, allora si procede con la loro trasmissione, seguita dalla rispettiva riconfigurazione della sentinella, in caso contrario, si riparte con il ciclo appena descritto.
