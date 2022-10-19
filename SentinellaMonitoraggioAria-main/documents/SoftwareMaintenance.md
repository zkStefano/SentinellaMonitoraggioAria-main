# :toolbox: Software Maintenance
Al termine del ciclo di vita di un software, uno degli aspetti più importanti da tenere in considerazione, è la *Manutenzione del software*; come già è stato accennato in vari documenti, una delle qualità fondamentali che dovrebbe presentare il codice finale, è la modularità. Questa proprietà, unita ad una buona documentazone, dovrebbe, infatti, agevolare l'attività futura di manutenzione del software.
## Reverse engineering
Per quanto riguarda la stesura del codice, in realtà, non si è partiti da zero, bensì, inizialmente, ci si è trovati di fronte al problema di studiare e comprendere un codice già strutturato, con l'obbiettivo di implementare nuove funzionalità e realizzarne una documentazione.

Come possiamo capire, non è stata una vera e propria attività di **reverse engineering**, infatti, ad essa, è stata affiancata anche una attività di **forwarding engineering** per la quale sono stati realizzati, da zero, nuovi requisiti, nuovo design, nuove funzionalità...

Ciò che, nello specifico, è stato realzzato, è la gestione dei sensori collegati alla sentinella, procedendo in maniera incrementale, aggiungendo progressivamente, senza modificare il codice esistente, nuove funzionalità che permettessero la rilevazione di varie grandezze a partire dalla temperatura.

Dunque, sono state applicate una serie di piccole trasformazioni, in continuità con quanto precedentemente realizzato, che hanno modificato poco il codice preesistente, ma, la cui applicazione consecutiva, ne ha aggiornato le funzionalità.
