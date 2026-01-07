void crea_puntatore (){
	char *c_ptr = (char*)balloc();
}
void leggi_puntatore (){
	print(" -0x", COLORE_BLU_NERO);
	char buffer[10];
	input(buffer, 0x1c, sizeof(buffer), COLORE_BLU_NERO);
	char *l_ptr = (char*)stringa_hex(buffer);
	print(l_ptr, COLORE_BIANCO_NERO);
}
void scrivi_puntatore (){
	print(" -0x", COLORE_VERDE_NERO);
	char buffer[10];
	input(buffer, 0x39, sizeof(buffer), COLORE_VERDE_NERO);
	char *s_ptr = (char*)stringa_hex(buffer);
	print(" valore=", COLORE_BIANCO_NERO);
	char buffer_val[1024];
	input(buffer_val, 0x1c, sizeof(buffer), COLORE_BIANCO_NERO);
	*s_ptr = stringa_ascii_dec(buffer_val);
}
void rimuovi_puntatore (){
	print(" -0x", COLORE_ROSSO_NERO);
	char buffer[10];
	input(buffer, 0x1c, sizeof(buffer), COLORE_ROSSO_NERO);
	char *r_ptr = (char*)stringa_hex(buffer);
	//printhex(stringa_int(buffer), COLORE_ROSSO_NERO);
	bfree(r_ptr);
}
Comandi lista_sottocomandi_ptr[] = {{"c", crea_puntatore},{"rm", rimuovi_puntatore}, {"l", leggi_puntatore}, {"s", scrivi_puntatore}};
void puntatore (){
	print(" -", COLORE_GIALLO_NERO);
	char buffer[8];
	input(buffer, 0x1c, sizeof(buffer), COLORE_BIANCO_NERO);
	controllo_sottocomando(buffer, lista_sottocomandi_ptr, (sizeof(lista_sottocomandi_ptr) / sizeof(lista_sottocomandi_ptr[0])));
}
