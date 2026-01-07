void sys_spegni (){
	__asm__ (
		"movw $0x5307, %ax\n"
		"movw $0x0001, %bx\n"
		"movw $0x0003, %cx\n"
		"int $0x15\n"
	);
}

void sys_riavvia (){
	__asm__ ("int $0x19\n");
}

void sys_iberna (){
	while (1){
		__asm__("hlt");
	}
}

void sys_panic (){
	char buffer[1024];
	print(" msg=", COLORE_BIANCO_NERO);
	input(buffer, 0x1c, sizeof(buffer), COLORE_ROSSO_NERO);
	clear();
	panic(buffer);
}

void sys_ram (){
	printint(grandezza_ram(), COLORE_BIANCO_NERO);
	print(" Kb", COLORE_BIANCO_NERO);
}

Comandi lista_sottocomandi_sys[] = {{"spg", sys_spegni}, {"riv", sys_riavvia}, {"ibe", sys_iberna}, {"ram", sys_ram}, {"panic", sys_panic}};

void sistema (){
	print(" ?", COLORE_ROSSO_NERO);
	char buffer[8];
	input(buffer, 0x1c, sizeof(buffer), COLORE_BIANCO_NERO);
	controllo_sottocomando(buffer, lista_sottocomandi_sys, (sizeof(lista_sottocomandi_sys) / sizeof(lista_sottocomandi_sys[0])));
}
