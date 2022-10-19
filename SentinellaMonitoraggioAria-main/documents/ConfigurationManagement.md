# :clipboard: Configuration Management
Tutto ciò che verrà prodotto in seguito alle ore di lavoro (che sia documentazione o codice), verrà sempre caricato e salvato su una repository GitHub condivisa tra tutti gli interessati al progetto facilitando così la cooperazione tra i membri del team, ma anche la supervisione di clienti ed eventuali superiori.

Vediamo, dunque, come questo archivio è organizzato e come viene gestito.
## Struttura dell'archivio
La repository è suddivisa in 2 cartelle principali:
* Cartella <code>code</code>, che contiene tutto il codice relativo al funzionamento dei singoli sensori e della stazione di monitoraggio completa.
* Cartella <code>documents</code>, che contiene tutti i file relativi all'ambito progettazione, a partire dal project plan iniziale.
## Issues
Generalmente, durante il fine settimana, si definiscono le attività ed i compiti da svolgere nei 7 giorni successivi. Queste attività vengono "tradotte" in <code>issues</code> su GitHub da gestire poi come vedremo; oviamente, quando necessario, sarà possibile creare nuovi issues anche durante la settimana di lavoro.

Per la gestione delle attività ci si affida ad una classica Kanban board (<code>AirMonitoringStation</code>) che definisce i vari stati in cui un issue può trovarsi. E' suddivisa in 4 colonne: **Backlog**, **To Do**, **In Progess**, **Done** dove:
* **Backlog**: è la colonna che contiene tutte le attività da svolgere;
* **To Do**: è la colonna che comprende le attività che si è scelto di svolgere, ma che ancora non sono state intraprese;
* **In Progress**: quando una attività è iniziata si sposta l'issue corrispondente in questa colonna dove vi rimane fino a quando non viene testata, validata e verificata la correttezza della soluzione che ne consegue;
* **Done**: è la colonna in cui i vari issues vengono spostati quando chiusi (e, di conseguenza, quando l'attività associata si è conclusa correttamente).
## Branches
Il "default branch", o branch principale, è il <code>main</code>. In questo ramo sono presenti tutti i file, tra documentazione e codice, definitivi, dunque opportunamente controllati e testati per garantirne la correttezza.

Quando si intraprende una nuova attività, l'idea è quella di aprire un branch dedicato (a partire dal <code>main</code>, associandogli un nome significativo) nel quale verranno effettuate tutte le varie operazioni (aggiunte di funzionalità, correzione di errori, creazione di nuova documentazione...) connesse con quella specifica attività. Concluso l'issue, viene creata una <code>Pull Request</code> con la quale si pone all'attenzione del team il lavoro svolto, permettendo di controllare e revisionare quanto fatto. In caso di dubbi si lascia un commento nel quale verrà espresso "ciò che non va" permettendo all'autore di intervenire in maniera mirata, altrimenti si approva la Pull Request.

Con l'approvazione della Pull Request il contenuto del branch dedicato può essere unito al main e l'issue corrispondente può essere chiuso e spostato nella colonna **Done**. Il branch, tuttavia, non verrà chiuso immediatamente, in modo tale da essere, poi, disponibile nuovamente in caso di grossi aggiornamenti successivi che riguarderanno il proprio contenuto.
