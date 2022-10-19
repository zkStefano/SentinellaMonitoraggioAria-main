# Component Diagram
Il secondo diagramma che comporrà la *Vista strutturale* del prototipo di sentinella di monitoraggio, è il **Component Diagram**, che permette la visualizzazione dei componenti (nuclei di funzionalità) costituenti la stazione.

![ComponentDiagram](https://user-images.githubusercontent.com/96539194/189503629-656c01ec-0729-4ba1-89c5-778c525adf08.PNG)

In questo diagramma troviamo evidenziati, appunto, componenti e servizi che ogni elemento fornisce o richiede; scendendo nel dettaglio, dunque, abbiamo:
* **Manager**: componente fondamentale in quanto è colui che gestisce il comportamento della sentinella. Fornisce la possibilità di connettersi ad un gateway con cui scambiare dati in uplink o downlink, richiede un controllo per verificare il corretto invio del messaggio contenente i dati raccolti dai sensori e, la riconfigurazione della sentinella in caso di presenza di parametri per la stessa;
* **UplinkBuffer**: componente che rappresenta la rilevazione, la memorizzazione e la gestione dei valori che assumono le grandezze oggetto della misurazione. Richiede la ricezione dei dati misurati da ogni sensore, mentre fornisce la possibilità di inviarli al gateway e, al Manager, la possibilità di verificare il corretto invio degli stessi.
* **DownlinkBuffer**: componente che rappresenta la ricezione, memorizzazione e gestione dei parametri che il gateway invia alla sentinella. Richiede, ovviamente, i parametri di riconfigurazione (inviati quando disponibili), mentre fornisce la possibilità di esguire un parse della stringa di parametri assegnando ogni valore alla relativa variabile globale di riferimento, riconfigurando la sentinella secondo le varie esigenze.
