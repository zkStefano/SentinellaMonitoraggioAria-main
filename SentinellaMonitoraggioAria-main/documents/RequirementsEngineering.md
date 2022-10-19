# :memo: Requirements Engineering
Una delle prime, e anche più importanti, fasi dello sviluppo software, atta a fornire il primo passo per ottenere una prima soluzione al problema generale di progettazione, è la *Requirements Engineering*, ovvero la definizione, analisi, documentazione e mantenimento dei requisiti. Ciò che si ottiene da questo processo analitico è un documento preciso, la *Specifica dei requisiti*, che conterrà entrambe le tipologie di requisiti, funzionali (le funzionalità, i servizi che il prodotto/sistema offre concretamente) e non funzionali (l'insieme di vincoli e standard "di qualità" che il sistema dovrà soddisfare come tempi di risposta, dimensione massima, portabilità...). 

Tuttavia, prima di redigere la specifica dei requisiti, è necessario comprendere ciò che il cliente desidera. Il processo di ingegneria dei requisiti, dunque, si articolerà in 4 punti:
* Elicitation: per comprendere le richieste e le funzionalità;
* Specification: per fissare i requisiti ottenuti nella fase precedente secondo un determinato criterio;
* Validation: per la verifica e validazione della specifica redatta;
* Negotiation: per concordare, negoziare le funzionalità richieste dal cliente con quelle realizzabili entro i limiti di tempo e costi prestabiliti.

L'analisi, ovviamente, sarà di tipo *Customer Driven*, dato che, come descritto precedentemente, il cliente è il centro del progetto e da lui arrivano le principali direttive per orientare (correttamente) il proseguimento dei lavori. Le modifiche al documento, poi, potranno essere definitivamente introdotte solo a seguito di un confronto con tutti i membri del team compreso, ovviamente, il cliente.

I requisiti ottenuti, infine, verranno organizzati secondo la tecnica MoSCoW (Must have, Should have, Could have, Won't have), come mostrato di seguito:

### Requisiti Funzionali
* **Must have**:
  * Rilevazione della temperatura;
  * Rilevazione della percentule di umidità;
  * Rilevazione della concentrazione di particolato (ovvero dell'insieme di sostanze solide e liquide sospese in aria), PM10;
  * Rilevazione della concentrazione di ozono (O3) nell'aria atmosferica;
  * Rilevazione della concentrazione di benzene (C6H6) nell'aria atmsoferica;
  * Rilevazione della concentrazione di ammoniaca (NH3) nell'aria atmosferica;
  * Rilevazione della concentrazione di aldeidi nell'aria atmosferica.
* **Should have**:
  * Rilevazione della posizione della sentinella tramite dispositivo di localizzazione (GPS);
  * Attivazione/disattivazione dei pin collegati ai sensori "a comando".
* **Won't have**:
  * Stampa, su uno schermo esterno, della concentrazione degli inquinanti.
### Requisiti non Funzionali
* **Must have**:
  * Il codice per la gestione dei sensori deve essre caratterizzato da un buon livello di modularità;
  * Ogni sensore avrà (singolarmente) a disposizione al massimo 30 secondi per fare una misurazione;
  * Il tempo totale di risposta dei sensori non dovrà superare i 210 secondi per singola rilevazione (30s * 7 sensori coinvolti);
  * Il prodotto finale dovrà essere efficiente, e quindi funzionare spendendo il minimo delle risorse richieste;
  * Il grado di interoperabilità del dispositivo, dovrà essere elevato. 
* **Should have**:
  * Deve essere facile individuare e correggere eventuali errori che si presentano;
  * La sentinella deve essere compatta, occupando la minore quantità di spazio possibile.
* **Could have**:
  * La sentinella deve poter essere utilizzata in tutte le condizioni climatiche (dovrà, dunque, essere impermeabile, reistente al caldo e al freddo, robusta...).
* **Won't have**:
  * Deve essere possibile, in caso di necessità, trasferire il codice su un hardware diverso. 
