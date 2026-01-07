# BedeskOS
Sistema Operativo a 32bit sviluppato in C per eseguire I/O sulle porte/periferiche e con un mini filesystem per la gestione di dati

## Specifiche di BedeskOS
BedeskOS è un sitema operativo a 32bit sviluppato in kernel mode per la piattaforma i386.
Il sistema è totalmente scritto a mano, senza l' ausilio di librerie standard/esterne. 
> [!WARNING]
> Il filesystem è ancora in fase di programmazione e presenta diversi problemi di funzionamento.
> È **altamente sconsigliato** il salvataggio di dati importanti!

## Requisiti di Sistema
1. 100MB di RAM
2. 700MHz di CPU
3. < 128GB di HDD/SSD (dato che il filesystem elabora solo variabili a 32bit **`unsigned long int`**)

## Compilazione

### Pacchetti necessari
Derivate Debian e derivate: `build-essential` `nasm` `gcc` `grub-mkrescue` `ld` `grub-pc-bin` `grub-common` `xorriso` `mtools` 
<br><br>
Arch e derivate: `base-devel` `nasm` `gcc` `ld` `grub` `grub-mkrescue` `xorriso` `mtools`

Il sistema funziona all'interno di macchine virtuali come QEMU e VirtualBox, e anche su hardware reale.
1. `make clean`
2. `make`
3. `qemu-system-i386 -cdrom bkernel.iso -hda <HardDisk.img>`

## Comandi del Sistema
| Nome comando | Scopo | Funziona |
| --- | --- | --- |
| `stampa <string>` | stampa i caratteri immessi | `true` |
| `ptr [-c, -l <indirizzo>, -s <string>, -rm <indirizzo>]` | allocazione in memoria + operazioni CRUD | `true` |
| `sys (-spg, -riv, -ibe, -ram, -panic <string>)` | suite di comandi di sistema | parziale |
| `clear` | pulisce lo schermo | `true` |
| `inb (porta)` | lettura da porta | `true` |
| `outb (comando, porta)` | invio valore a porta | `true` |
| `disco [-info, -format]` | suite di comandi per eseguire operazioni sul disco| parziale |
| `ls` | lista dei file presenti sul disco | `false` |
| `editor <file>` | creazione/lettura di un file presente sul disco | parziale |

## Immagini del Sistema

![Avvio di BedeskOS](https://github.com/user-attachments/assets/14c7848a-1fa2-47be-a025-a9e5af982bea)

![CRUD con allocatore di memoria](https://github.com/user-attachments/assets/a3c845af-96d0-46e6-b69f-f66746ebfcc8)

![Operazioni sul disco](https://github.com/user-attachments/assets/58fd2cdd-29f9-4dbd-90b7-c8c339ce5ace)
