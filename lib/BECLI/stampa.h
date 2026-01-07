void stampa (){
	print(" \"", COLORE_MAGENTA_NERO);
	char buffer[1024];
	input(buffer, 0x1c, sizeof(buffer), COLORE_BIANCO_NERO);
	print("\"\n", COLORE_MAGENTA_NERO);
	print(buffer, COLORE_BIANCO_NERO);
}
