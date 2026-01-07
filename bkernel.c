#include "lib/BIO.h"
#include "lib/BALLOC.h"
#include "lib/BECLI/BSHELL.h"
#include "lib/BEUTIL.h"
#include "lib/BTIME.h"

#include "lib/BEFS/BEFS.h"


void main() {
	clear();
	print("Avvio Bkernel...", COLORE_BIANCO_NERO);
	print("OK\n", COLORE_VERDE_NERO);

	BEFS();

	while (1){
		char buffer[128];
		input(buffer, 0x1c, sizeof(buffer), COLORE_BIANCO_NERO);
		controllo_comando(buffer);
		//print(buffer, COLORE_BIANCO_NERO);
		print("\n", COLORE_BIANCO_NERO);
	}
	while (1) {
		__asm__ ("hlt");
	}
}
