unsigned short int stringa_unsigned_short (char*, int);
void input_b(){
	print(" 0x", COLORE_MAGENTA_NERO);
	char buffer;
	char porta[10];
	input(porta, 0x1c, sizeof(porta), COLORE_MAGENTA_NERO);
	unsigned short porta_us = stringa_unsigned_short(porta, sizeof(porta));
	__asm__ volatile("in %%dx, %%al" : "=a" (buffer) : "dN" (porta_us));
	printint(buffer, COLORE_BIANCO_NERO);
}
