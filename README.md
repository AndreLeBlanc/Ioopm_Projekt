# fladdermusen

## Testa kod på flera maskiner
| **OBSERVERA** Make-kommandon som nämns i denna sektion gäller Makefile i
`resources`, så glöm inte att `cd resources` innan `make whatever`.
____
Makefilen som ligger i *resources* innehåller funktioner för att dels testa kod på flera maskiner samt sätta upp SSH-nycklar.
### First step - Skapa nycklar
```
make gen
```
kommer att skapa ssh-nycklar, tryck enter på varenda fråga för att låta ssh-keygen köra med standard-inställningar.

### Second step - Ladda upp nycklar till UU
```
make deploy_key
```
kommer att ladda upp din nyskapade publika nyckel (se ovan) till din hem-mapp på UU:s datorer samt lägga till den i auktoriserade nycklar (så att du kan använda den för att logga in utan lösenord).
*Detta kommando kommer även att skapa mappen fladdermusen/ i din hem-mapp*
Du kommer att behöva ditt A-lösenord samt inloggningsnamn, kom ihåg att svara *yes* när ssh frågar om du litar på servern eller inte.

~~### Third step - Ladda upp kod till UU~~

~~### Fourth step - Köra tester på alla maskiner~~

### Third step - Ladda upp och testa på alla maskiner
```
make go
```
kommer att zippa alla filer i fladdermusen, ladda upp filen till din hem-mapp på UU:s datorer, radera gamla filer i mappen *fladdermusen/* och unzippa de nya filerna.

Go kommer även att kompilera test-koden som valts att köras under *test:*, spy ut alla output till en fil enligt följande format
```
hostname.result.txt
```
Go kommer sedan att hämta hem filen ovan till din lokala maskin, en för varje test kommer att lägga sig i
```
resources/hostname.result.txt
```
**TADDAAAA**

~~### Fifth step - enjoy~~
### Fourth step - enjoy
Så för att summera, *steg 1 och steg 2* är endast för att sätta upp test-miljön.
Steg 3 ~~och 4~~ ska repeteras varje gång du uppdaterar koden och vill testa den.

# Coding Convention

## gc.c ##

Projektfilen. Det som inkluderas för att få tillgång till vår heap med allokering och garbage collection.

### allocator.c ###

Den filen som hantar all allokering och sånt.
Formatsträngar, kollar om minnet är fullt

### collector.c ###

Den filen som hanterar garbage collection.
Denna drar igång när minnet är fullt.
