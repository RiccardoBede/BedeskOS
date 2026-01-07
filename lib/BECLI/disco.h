void info (){
	print("\n", COLORE_BIANCO_NERO);
	scelta_disco();
	info_disco();
}

void formatta (){
	scelta_disco();
	formatta_disco();
	print("\nDISCO FORMATTATO CON (0x00)!", COLORE_GIALLO_NERO);
}

Comandi lista_sottocomandi_disco[] = {{"info", info}, {"format", formatta}};

void disco(){
	print(" -", COLORE_BIANCO_NERO);
	char buffer[10];
	input(buffer, 0x1c, sizeof(buffer), COLORE_GIALLO_NERO);
	controllo_sottocomando(buffer, lista_sottocomandi_disco,(sizeof(lista_sottocomandi_disco) / sizeof(lista_sottocomandi_disco[0])));
}
