# fladdermusen

##Rapport
https://www.overleaf.com/4056909wctnkn#/11860949/

##GC related tasks

###Compile and run tests
Dependencies: CUnit
`make run_test`

###Documentation
Dependencies: doxygen, CUnit
`make doc`

###LCov, visualise code coverage
Dependencies: lcov, gcov, CUnit
`make lcov`

###Valgrind, check memory leaks and misc. errors.
Dependencies: valgrind, CUnit
`make valgrind`

###Compile and run GUI, toolbox for our Heap and Garbage Collector.
`make test_gui`
____

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

### heap.c ###

Den filen som hantar all allokering och sånt.
Formatsträngar, kollar om minnet är fullt

#### header ####

formatsträng eller vad den innehåller

[ pekare till formatsträng : bitvektor : forwarding address : kopieringsflagga ]





1. En pekare till en formatsträng
2. En mer kompakt representation av objektets layout
3. En pekare till en funktion som hanterar skräpsamling av ett objekt
med komplex layout (t.ex. struktar med unioner, se § 3.3.4, om detta
implementeras)
4. En forwarding-adress
5. En flagga som anger om objektet redan är överkopierat till den passiva
arean vid skräpsamling

start with formatsstring then implement bitvector later.


### collector.c ###

Den filen som hanterar garbage collection.

Denna drar igång när minnet är fullt.


### traverser.c ###


can be seperated into traversing heap and stack.

heap will be more


# Current Tasks #

## Group ##
Strictly design connections between modules.

## Carl André ##
Keep working on heap
CUnit.
DDD.

## Malin Haubir ##
Traversing the stack

## Viktor ##
Traversing the heap

## Uncertain pointers ##

Heap pointers are safe because we have metadata

Stack pointers don't have metadata, so all those pointers are unsafe.

## Bartlett ##

If something is uncertain, don't move it. That page is active.

Even if a little bit of trash is left, it's eventually going to be collected.

Sooner or later everything will be collected.

Move to active pages.

When debuggning, set whole page to 0 when it's passive. But this doesn't need to be implemented in the end.
Important to zero out metaimnfo.
