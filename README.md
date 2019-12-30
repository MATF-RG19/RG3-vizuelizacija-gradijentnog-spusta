# Vizuelizacija gradijentnog spusta

## :memo: Opis
"Vizuelizacija gradijentnog spusta" pruža uvid u pronalaženje lokalnog minimuma
funkcije sa dve promenljive primenom gradijentnog spusta. Iako se takve
vrednosti mogu analitički izračunati to je često veoma komplikovano, pa se
pribegava tehnikama poput pomenute. Gradijentni spust je prikazan na nekoliko
funkcija različitih karakteristika.

## :video_game: Pokretanje i komande
Po pokretanju programa, potrebno je odabrati neku od dostupnih funkcija:
eliptički paraboloid, hiperbolički paraboloid, sinusna funkcija sa dve
promenljive i treći stepen proizvoda dve promenljive. To se postiže pritiskom na
naki od tastera `1`, `2`, `3` ili `4`.

Na `i` se animacija instancira i tada se mala sfera postavlja na proizvoljnu
poziciju odabrane mnogostrukosti. Vizuelizaciju je moguće pokrenuti na `g`, 
dok se na `s` može zaustaviti.

Vrednost parametra obučavanja (`learning rate`) je moguće smanjivati
(`q`) i povećavati (`e`). Ukoliko vrednost ovog parametra postane prevelika,
dolazi do divergencije, a ukoliko je premala gotovo da neće doći do promena.

Scenu je moguće kontrolisati putem strelica (translacije), skrolovanja mišem
(skaliranje) i pokretima mišem (rotacije). Na `r` se pogled vraća u početni.

## :wrench: Zavisnosti i instalacija
* Na operativnom sistemu Windows je dovoljno pokrenuti `GDViz.sln`.
* Na Linux operativnim sistemima je dovoljno oktucati `make` u terminalu.
**Potrebno je preći u** `makefile` **branch**. Nakon pokretanja `make` naredbe,
biće generisan fajl `GDViz` koji je potrebno pokrenuti. Za uklanjanje svih propratnih
fajlova, dovoljno je otkucati `make clean`.

## :warning: Ostalo
* Biblioteka za rad sa slikama je preuzeta sa sledeće [adrese](https://github.com/nothings/stb).
* Neophodno je imati instaliran `OpenGL` na sistemu.
