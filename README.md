# Vizuelizacija gradijentnog spusta

## Opis
"Vizuelizacija gradijentnog spusta" pruža uvid u pronalaženje lokalnog minimuma
funkcije sa dve promenljive primenom gradijentnog spusta. Iako se takve
vrednosti mogu analitički izračunati to je često veoma komplikovano, pa se
pribegava tehnikama poput pomenute. Gradijentni spust je prikazan na nekoliko
funkcija različitih karakteristika.

## Pokretanje
Po pokretanju programa, potrebno je odabrati neku od dostupnih funkcija:
eliptički paraboloid, hiperbolički paraboloid, sinusna funkcija sa dve
promenljive i treći stepen proizvoda dve promenljive.

Vrednost parametra obučavanja (`learning rate`) je moguće smanjivati
(`q`) i povećavati (`e`). Ukoliko vrednost ovog parametra postane prevelika,
dolazi do divergencije, a ukoliko je premala gotovo da neće doći do promena.

Na `i` se animacija instancira i tada se mala sfera postavlja na proizvoljnu
poziciju odabrane mnogostrukosti. Vizuelizaciju je moguće pokrenuti na `g`, 
dok se na `s` može zaustaviti.

Scenu je moguće kontrolisati putem strelica (translacije), skrolovanja mišem
(skaliranje) i pokretima mišem (rotacije). Na `r` se pogled vraća u početni.

## Zavisnosti i instalacija
Korišćeno razvojno okruženje je Visual Studio. Dovoljno je pokrenuti `*.sln` fajl. Korišćena je biblioteka `OpenGL` (`FreeGlut`).
Za pokretanje na Linux sitemima, potrebno je preći u branch `makefile` u kom se nalazi Makefile za lakše pokretanje same aplikacije.

Biblioteka za rad sa slikama je preuzeta sa sledeće [adrese](https://github.com/nothings/stb).
