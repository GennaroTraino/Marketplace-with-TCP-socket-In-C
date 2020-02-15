# Marketplace-with-TCP-socket-In-C
This is a small project: A marketplace with TCP socket in C language.

Manuale Utente
Per poter eseguire il programma e testarlo, bisogna scaricare ed estrarre la cartella
Marketplace.zip.
Il progetto include 11 file: i file sorgente .c dei 3 Server e dei 2 client, più 3 file sorgente delle
3 librerie con i rispettivi 3 file header.
Per eseguire il programma è necessario compilare i 3 file dei Server e i 2 dei Client ed
eseguirli da shell differenti.

Per compilare il ServerM
  $ gcc -o serverM ServerM.c List.c Wrapped.c IO.c
Per Eseguire
  $ ./serverM
  
Per compilare il ServerN
  $ gcc -o serverN ServerN.c List.c Wrapped.c IO.c
Per Eseguire
  $ ./serverN

Per compilare il ServerC
  $ gcc -o serverC ServerC.c List.c Wrapped.c IO.c
Per Eseguire
  $ ./serverC

Per compilare il Client
  $ gcc -o client Client.c List.c Wrapped.c IO.c
Per Eseguire
  $ ./client

Per compilare il ClientN
  $ gcc -o clientN ClientN.c List.c Wrapped.c IO.c
Per Eseguire
  $ ./clientN

Come primo passo si esegue prima il ServerM, poi i Server C ed N ed infine si aprono i client
tutti in 5 terminali distinti.

Una volta eseguito il clientN possiamo testare il programma inserendo le credenziali:
  username: utente1
  Pin: 1234

Sarà ora possibile interagire con i comandi dall’interfaccia del terminale. Si inseriscono di
volta in volta il codice delle operazioni che si vogliono effettuare e infine si seguono le
indicazioni stampate dal terminale.
