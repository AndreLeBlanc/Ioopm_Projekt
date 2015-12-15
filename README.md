# fladdermusen

## gc.c ##

Projektfilen. Det som inkluderas för att få tillgång till vår heap med allokering och garbage collection.

### heap.c ###

Den filen som hantar all allokering och sånt.
Formatsträngar, kollar om minnet är fullt

#### header ####

formatsträng eller vad den innehåller

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
