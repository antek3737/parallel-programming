##### ENG:
The project consists of two independently starting up programs: manufacturer and consumer. 
  Manufacturer creates defined number of processes that are sending defined number of chars and saving them into their files named "in_PID.txt".
Then, manufacturer waits till all processes  have been ended.
  Consumer creates defined number of processes that are receiving chars and putting it into their files calles "out_PID.txt".
Then, consumer waits till all processes  have been ended.
  Moreover, each program checks if the limit of running processes will not be exceed.
  When the last char is received, one of processes remotes pipe.
  Functionality of the project is mainly based on named pipe(fifo).

##### PL:
Projekt składa się z dwóch programów uruchamianych niezależnie: producenta i konsumenta.
  Producent tworzy określoną liczbę procesów, które wysyłają  określoną liczbę znaków  do potoku oraz zapisują je do swojego pliku o nazwie "in_PID.txt", a następnie czeka na ich
zakończenie.
  Konsument tworzy określoną liczbę procesów, które odczytują znaki z potoku i zapisują je do swojego pliku o nazwie "out_PID.txt", a następnie
czeka na ich zakończenie.
  Programy sprawdzają dopuszczalny limit procesów, które może użytkownik uruchomić w danym
momencie.
  Po pobraniu z łącza ostatniego znaku, jeden z procesów usuwa utworzone łącze. 
  Projekt oparty jest głównie o łącze nazwane(fifo).
<\p>
