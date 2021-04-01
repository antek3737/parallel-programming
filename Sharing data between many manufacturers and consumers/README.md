The project consists of two independent programs: manufacturer and consumer. 
Manufacturer creates defined number of processes that are sending defined number of chars and putting it into their files named "in_PID.txt".
Then, created processes are waiting till all chars have been received by consumers.
Consumer creates defined number of processes that are receiving chars and putting it into their files calles "out_PID.txt".
The project is using mainly named pipe (fifo).


Wykorzystując potoki nazwane (fifo) uogólnić zadanie producent–konsument na wielu producentów i
wielu konsumentów. W zadaniu wykorzystać jedno łącze nazwane.
Projekt składa się z dwóch programów (plików): producent i konsument. Programy uruchamiane
niezależnie.
Argumentem wywołania programu producent jest liczba producentów oraz liczba znaków
„wyprodukowanych” przez każdego producenta (np. ./producent 50 1000). Program producenta
tworzy określoną liczbę procesów, które zapisują dane do potoku, a następnie czeka na ich
zakończenie.
Argumentem wywołania programu konsument jest liczba konsumentów (np. ./konsument 30).
Program konsumenta tworzy określoną liczbę procesów, które odczytują dane z potoku, a następnie
czeka na ich zakończenie.
Programy sprawdzają dopuszczalny limit procesów, które może użytkownik uruchomić w danym
momencie i w przypadku, kiedy limit jest większy lub równy liczbie tworzonych procesów, uruchamia
zadanie.
Każdy z producentów losuje określoną drugim argumentem wywołania liczbę znaków. Wylosowane
znaki zapisuje – po jednym znaku - do potoku oraz swojego pliku z danymi (np. we_1250.txt, gdzie
podany numer to pid procesu, który utworzył dany plik). Każdy z konsumentów odczytuje po jednym
znaku z potoku i zapisuje w swoim pliku z wynikami (np.: wy_1320.txt). Po zakończeniu zadana liczba
znaków we wszystkich plikach we_*.txt musi się równać liczbie znaków we wszystkich plikach wy_*.txt.
Po zakończeniu zadania (pobraniu z łącza ostatniego znaku) jeden z procesów usuwa utworzone łącze. 
