#include "STRUCT.h"

#include "ptr.h"
#include "stampa.h"
#include "sys.h"
#include "help.h"
#include "clear.h"
#include "inb.h"
#include "outb.h"
#include "disco.h"

#include "ls.h"

#include "editor.h"
#include "VGA.h"


Comandi lista_comandi[] = {{"stampa", stampa}, {"ptr", puntatore}, {"sys", sistema}, {"help", help}, {"clear", clear}, {"inb", input_b}, {"outb", output_b}, {"disco", disco}, {"editor", editor}, {"startVGA", start_VGA}, {"ls", ls}};

void controllo_comando(char *input) {
    int uguale;
    int dd = 0;
    int ee = 0;
    for (dd = 0; dd < sizeof(lista_comandi) / sizeof(lista_comandi[dd]); dd++) {
        uguale = 1;
        for (ee = 0; lista_comandi[dd].comando[ee] != '\0' && input[ee] != '\0'; ee++) {
            if (lista_comandi[dd].comando[ee] != input[ee]) {
                uguale = 0;
                break;
            }
        }
        if (uguale && lista_comandi[dd].comando[ee] == '\0' && input[ee] == '\0') {
            lista_comandi[dd].funzione();
            return;
        }
    }
    print("ERR: COMANDO SCONOSCIUTO", COLORE_ROSSO_NERO);
    return;
}

void controllo_sottocomando(char *input, Comandi *sotto_comandi, int num_comandi) {
    int uguale;
    int dd, ee;
    for (dd = 0; dd < num_comandi; dd++) {
        uguale = 1;
        for (ee = 0; sotto_comandi[dd].comando[ee] != '\0' && input[ee] != '\0'; ee++) {
            if (sotto_comandi[dd].comando[ee] != input[ee]) {
                uguale = 0;
                break;
            }
        }
        if (uguale && sotto_comandi[dd].comando[ee] == '\0' && input[ee] == '\0') {
            sotto_comandi[dd].funzione();
            return;
        }
    }
    print("ERR: COMANDO SCONOSCIUTO", COLORE_ROSSO_NERO);
    return;
}

