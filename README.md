# fladdermusen

## gc.c ##

Projektfilen. Det som inkluderas för att få tillgång till vår heap med allokering och garbage collection.

### allocator.c ###

Den filen som hantar all allokering och sånt.
Formatsträngar, kollar om minnet är fullt

### collector.c ###

Den filen som hanterar garbage collection.
Denna drar igång när minnet är fullt. 
