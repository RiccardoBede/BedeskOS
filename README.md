# BedeskOS
Sistema Operativo a 32bit sviluppato in C per eseguire I/O sulle porte/periferiche e con un mini filesystem per la gestione di dati

## Specifiche di BedeskOS
BedeskOS è un sitema operativo a 32bit sviluppato in kernel mode per la piattaforma i386.
Il sistema è totalmente scritto a mano, senza l' ausilio di librerie standard/esterne. 
> [!WARNING]
> Il filesystem è ancora in fase di programmazione, e presenta diversi problemi di funzionamento
> è **altamente sconsigliato** il salvataggio di dati importanti!

## Requisiti di Sistema
1. 100MB di RAM
2. 700MHz di CPU
3. < 128GB di HDD/SSD (dato che il filesystem elabora solo variabili a 32bit **`unsigned long int`**)

## Compilazione
Il sistema funziona all'interno di macchine virtuali come QEMU e VirtualBox, e anche su hardware reale.
1. `make clean`
2. `make`
3. `qemu-system-i386 -cdrom bkernel.iso -hda <HardDisk.img>`

## Comandi del Sistema
| Nome comando | Scopo | Funziona |
| --- | --- | --- |
| `stampa <string>` | stampa i caratteri immessi | `true` |
| `ptr [-c, -l, -s <string>, -rm <indirizzo>]` | allocazione in memoria + operazioni CRUD | `true` |
| `sys (-spg, -riv, -ibe, -ram, -panic <string>)` | suite di comandi di sistema | parziale |
| `clear` | pulisce lo schermo | `true` |
| `inb (porta)` | lettura da porta | `true` |
| `outb (comando, porta)` | invio valore a porta | `true` |
| `disco [-info, -format]` | suite di comandi per eseguire operazioni sul disco| parziale |
| `ls` | lista dei file presenti sul disco | `false` |
| `editor <file>` | creazione/lettura di un file presente sul disco | parziale |
