unsigned short int stringa_unsigned_short (char*, int);
void output_b(){
	print(" 0x", COLORE_VERDE_NERO);
	char comando[10];
	char porta[10];
	input(comando, 0x1c, sizeof(comando), COLORE_VERDE_NERO);
	print(",", COLORE_BIANCO_NERO);
	print(" 0x", COLORE_MAGENTA_NERO);
	input(porta, 0x1c, sizeof(porta), COLORE_MAGENTA_NERO);
	unsigned short porta_us = stringa_unsigned_short(porta, sizeof(porta));
	unsigned char comando_hex = stringa_hex(comando);
	__asm__("outb %0, %1" : : "a" (comando_hex), "dN" (porta_us));
}

