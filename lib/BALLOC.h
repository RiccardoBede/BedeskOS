//BALLOC allocatore di memoria
//
//SINTASSI
//
//int a* = (int *)balloc();
//

//#include "lib/BIO.h"
#define ENTRATA_BKERNEL 0x1000

unsigned int grandezza_ram() {
    unsigned char valore_reg;
    asm volatile (
        "outb %1, %%dx\n"
        "inb %%dx, %0\n"
        : "=a"(valore_reg)
        : "a"((unsigned char)0x15), "d"((unsigned short)0x70)
    );
    unsigned int valore_reg_basso = valore_reg;
    asm volatile (
        "outb %1, %%dx\n"
        "inb %%dx, %0\n"
        : "=a"(valore_reg)
        : "a"((unsigned char)0x16), "d"((unsigned short)0x70)
    );
    unsigned int valore_reg_alto = valore_reg;
    unsigned int grandezza_mem = (valore_reg_alto << 8) | valore_reg_basso;
    return grandezza_mem;
}


static char origine = ENTRATA_BKERNEL;

int balloc (){
	//ENTRATA_BKERNEL+=0x0001;
	unsigned int dd = 0;
	for (dd = ENTRATA_BKERNEL; dd < grandezza_ram(); dd++){
		char *puntatore = (char *)dd;
		if(*puntatore == 0x00){
			print("(0x", COLORE_BIANCO_NERO);
			printhex(dd, COLORE_BIANCO_NERO);
			print(") == 0x00", COLORE_BIANCO_NERO);
			break;
		} /*else{
			printhex(stringa_int(*puntatore), COLORE_BIANCO_NERO);
			print("\n", COLORE_BIANCO_NERO);
		}*/
	}
	return dd;
}

void bfree (void *puntatore){
	char *ptr_libera = (char *)puntatore;
	*ptr_libera = 0x00;
	printindir(&puntatore, COLORE_BIANCO_NERO);
	print(" => 0x00", COLORE_BIANCO_NERO);	
}
