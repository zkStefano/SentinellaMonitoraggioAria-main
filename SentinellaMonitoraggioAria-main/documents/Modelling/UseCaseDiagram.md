# Use Case Diagram
Per modellare i casi d'uso, è stato utilizato lo **Use Case Diagram** che ci permette di identificare, in maniera immediata, i casi d'uso, e, quindi, le funionalità del sistema.

![UseCaseDiagram](https://user-images.githubusercontent.com/96539194/189479941-8c8b3f3e-008d-4e1f-b400-0ef854c460de.PNG)

L'attore principale è la sentinella di monitoraggio dell'aria (<code>SentinellaCompleta</code>), che, come sappiamo, è composta da vari sensori (<code>DHT22, Grove Dust Sensor, MQ131, MQ135, MQ137, MQ138</code>), ognuno dei quali svolge la propria funzione.

La sentinella, come possiamo osservare, ha varie funzionalità, alcune delle quali, però, sono fornite dai sensori di cui è composta. Autonomamente, la sentinella, può, per prima cosa, "inizializzarsi" e, successivamente, gestire la raccolta dei dati ed i dati raccolti. Questa seconda funzionalità, tuttavia, necessita dell'esistenza di opportuni sensori in grado di rilevare gli inquinanti e le grandezze desiderati.

Ogni sensore, infatti, ha il compito di campionare una specifica grandezza (<code>readTemp, readHum, readPM, readOzono, readBenzene, readAmmoniaca, readAldeidi</code>) ed infatti, ogni caso d'uso specifico, dettaglia il caso d'uso astratto <code>readX</code>.

Infine, la sentinella, comunica con il gateway, con cui può scambiare sia i dati raccolti, che i dati per la propria riconfigurazione; ovviamente la trasmissione dati presuppone la presenza di qualcuno che possa ricevere le informazioni inviate, mentre, la successiva riconfigurazione della sentinella, potrà avvenire, solamente se esistono dei dati di riconfigurazione.


