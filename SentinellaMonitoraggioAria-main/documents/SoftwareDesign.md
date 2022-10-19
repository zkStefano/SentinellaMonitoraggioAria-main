# :mag_right: Software Design
Come visto nella documentazione rigurdante la <code>[Software Architecture](SoftwareArchitecture/)</code>, per lo svilupppo del sistema di gestione della sentinella ci si è basati sullo stile architetturale *Main Program with Subroutines*, con lo scopo di rendere il codice il più modulare possibile.
I dati e le attività caratteristiche della stazione, sono, quindi, gestiti da funzioni diverse, separate in più moduli. Ovviamente, la suddivisione in moduli, per quanto possibile, è stata realizzata cercando di applicare uno tra i principi più importanti da rispettare durante la stesura di un codice: ottenere un software con un basso livello di accoppiamento ed un alto livello di coesione.

Nel caso specifico del codice della sentinella, il numero di moduli nel quale è suddiviso ammonta ad 8 (escludendo il programma principale).
Per quel che rigurda la coesione, ovvero il livello al quale gli elementi di un modulo sono funzionalmente correlati, si ha un valore abbastanza elevato in quanto:
* Tutti gli elementi presenti in un modulo (alcuni sono composti da un singolo elemento) sono da utilizzare in succesione (**Procedural cohesion**);
* Gli elementi di un componente comunicano tra di loro per poi fornire un risultato (**Communicational cohesion**).

Mentre, per quanto riguarda l'accoppiamento, ovvero il grado di interdipendenza tra moduli, si ha un valore basso. Come già visto, infatti, lo stile archietturale scelto, "impone" una gerarchia tra i moduli i quali, almeno in parte, dipenderanno sempre l'uno dall'altro; dunque, una loro eventuale modifica, potrebbe andare a colpirne uno o più tra quelli a loro correlati.

Nel nostro caso, infatti, essendo il prodotto finale una sentinella IoT per la rilevazione della qualità dell'aria, i vari componenti che la costituiscono, dovranno, obbligatoriamente, passarsi dati, valori, informazioni... ed essere, dunque, fortemente accoppiati (**Content coupling**).

## Pattern utilizzato
In conclusione, per quanto riguarda il design pattern da seguire, come è possibile notare anche dalla strttura del codice, non ne è stato utilizzato uno in particolare; tra i vari disponibili, quello che ha più tratti in comune con l'organizzazione del programma è il *Facade pattern*, in quanto, idealmente, abbiamo delle funzionalità esposte in maniera semplice nel programma principale (che, appunto, assume il ruolo di facciata e rappresenta l'unico punto centralizzato di accesso) le quali, poi, sono implementate nel dettaglio nei vari moduli di cui il programma è composto.
