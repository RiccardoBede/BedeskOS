extern unsigned short porta_disco_default;
extern unsigned long int size_settori_long;
extern char size_settori[20];

unsigned long int disco_size (){
	char num[9];
	for (int dd = 0; dd < sizeof(num); dd++){
		if (size_settori[dd] >= 0x30){
			num[dd] = size_settori[dd];
		}else{
			break;
		}
	}
	if (stringa_int(num, sizeof(num)) > (potenza(2, 28) - 1)){
		print("OVERFLOW DISCO! [PRESI 2^28 SETTORI]", COLORE_GIALLO_NERO);
		return potenza(2, 28) - 1;
	}

	return stringa_int(num, sizeof(num)) + 1;
}

unsigned long int trova_settore_libero (){
	for (unsigned long int numero_settore = 1; numero_settore < disco_size(); numero_settore++){
		while(inb(porta_disco_default) & 0x80);
		while(!(inb(porta_disco_default) & 0x40));

		outb(1, (porta_disco_default - 5));
		
		outb(((numero_settore) >> 0) & 0xff, (porta_disco_default - 4));
		outb(((numero_settore) >> 8) & 0xff, (porta_disco_default - 3));
		outb(((numero_settore) >> 16) & 0xff, (porta_disco_default - 2));

		outb(0xe0 | ((0 & 1) << 4 | (((numero_settore << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

		outb(0x20, porta_disco_default);

		while(!(inb(porta_disco_default) & 0x08));
		
		char buffer_settore[512];
		int num_spazi_vuoti = 0;
		for(int ee = 0; ee < 512; ee++){
			buffer_settore[ee] = inw((porta_disco_default - 7));
		}
		outb(0xe7, porta_disco_default);
		for (int ee = 0; ee < sizeof(buffer_settore); ee++){
			if (buffer_settore[ee] <= 0x20){
				num_spazi_vuoti++;
			}
		}
		if (num_spazi_vuoti == sizeof(buffer_settore)){	return numero_settore;}
	}
	print("DISCO PIENO!", COLORE_ROSSOC_ROSSO);
	return 0;
}

unsigned long int scrivi_settore (char *nome, char *buffer, unsigned long int numero_settore){
	bool illegale = false;
	int numero_caratteri_buffer = 0;
	int index_buffer = 0;

	unsigned long int nuovo_settore = trova_settore_libero();
	unsigned long int copia_nuovo_settore = nuovo_settore;

	if (numero_settore == 0 || numero_settore > disco_size()){
		print("NUMERO SETTORE ILLEGALE! [PRESO PRIMO SETTORE LIBREO]", COLORE_ROSSOC_ROSSO);
		illegale = true;
	}

	if (!(illegale)){
		while(inb(porta_disco_default) & 0x80);
		while(!(inb(porta_disco_default) & 0x40));

		outb(1, (porta_disco_default - 5));

		outb(((numero_settore) >> 0) & 0xff, (porta_disco_default - 4));
		outb(((numero_settore) >> 8) & 0xff, (porta_disco_default - 3));
		outb(((numero_settore) >> 16) & 0xff, (porta_disco_default - 2));

		outb(0xe0 | ((0 & 1) << 4 | (((numero_settore << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

		outb(0x30, porta_disco_default);

		while(!(inb(porta_disco_default) & 0x08));
		
		for(numero_caratteri_buffer; buffer[numero_caratteri_buffer] != '\0'; numero_caratteri_buffer++);

		for(int dd = 0; dd < 512; dd++){
			if (dd <= 16){
				if (nome[dd] != '\0'){
					outw(nome[dd], (porta_disco_default - 7));
				}else{
					outw(0x3b, (porta_disco_default - 7));
					dd = 16;
				}
			}else{
				if (index_buffer >= numero_caratteri_buffer){
					outw(0x1f, (porta_disco_default - 7));
				}else{	
					outw(buffer[index_buffer], (porta_disco_default - 7));
				}
				index_buffer++;
			}
			if (dd == 486){
				outw(0xaa, (porta_disco_default - 7));
				while (nuovo_settore > 0){
					outw((char)(nuovo_settore % 10) + 0x30, (porta_disco_default - 7));
					nuovo_settore /= 10;
				}
				outb(0xe7, porta_disco_default);
				return copia_nuovo_settore;
			}
		}
		outb(0xe7, porta_disco_default);
	}else{
		for (numero_settore = 1; numero_settore < disco_size(); numero_settore++){
			char buffer_settore[512];
			int numero_spazi_vuoti_settore = 0;

			while(inb(porta_disco_default) & 0x80);
			while(!(inb(porta_disco_default) & 0x40));

			outb(1, (porta_disco_default - 5));

			outb(((numero_settore) >> 0) & 0xff, (porta_disco_default - 4));
			outb(((numero_settore) >> 8) & 0xff, (porta_disco_default - 3));
			outb(((numero_settore) >> 16) & 0xff, (porta_disco_default - 2));

			outb(0xe0 | ((0 & 1) << 4 | (((numero_settore << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

			outb(0x20, porta_disco_default);

			while(!(inb(porta_disco_default) & 0x08));

			for (int dd = 0; dd < 512; dd++){
				buffer_settore[dd] = inw((porta_disco_default - 7));
			}
			outb(0xe7, porta_disco_default);
	
			for (int dd = 0; dd < sizeof(buffer_settore); dd++){
				if (buffer_settore[dd] < 0x30){	numero_spazi_vuoti_settore++;}
			}

			if (numero_spazi_vuoti_settore == 512){
				return scrivi_settore(nome, buffer, numero_settore);
			}
		}
	}
	return numero_settore;
}

unsigned long int leggi_settore (char *buffer, int size_buffer, unsigned long int numero_settore){
	bool illegale = false;
	if (numero_settore == 0 || numero_settore > disco_size()){
		print("NUMERO SETTORE ILLEGALE!", COLORE_ROSSOC_ROSSO);
		illegale = true;
	}

	if (!(illegale)){
		char buffer_settore[512];

		while(inb(porta_disco_default) & 0x80);
		while(!(inb(porta_disco_default) & 0x40));

		outb(1, (porta_disco_default - 5));

		outb(((numero_settore) >> 0) & 0xff, (porta_disco_default - 4));
		outb(((numero_settore) >> 8) & 0xff, (porta_disco_default - 3));
		outb(((numero_settore) >> 16) & 0xff, (porta_disco_default - 2));

		outb(0xe0 | ((0 & 1) << 4 | (((numero_settore << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

		outb(0x20, porta_disco_default);

		while(!(inb(porta_disco_default) & 0x08));

		for (int dd = 0; dd < 512; dd++){
			if (dd <= size_buffer){
				buffer_settore[dd] = buffer[dd] = inw((porta_disco_default - 7));
				printchar(buffer[dd], COLORE_GIALLO_NERO);
				printchar(',', COLORE_BIANCO_NERO);
			}else{
				buffer_settore[dd] = inw((porta_disco_default - 7));
			}
		}
		outb(0xe7, porta_disco_default);

		if (buffer_settore[486] == 0xaa){
			char num[10];
			int index_num = 0;

			for (int dd = 486; dd <= sizeof(buffer_settore); dd++){
				if (buffer_settore[dd] >= 0x30){
					num[index_num] = buffer_settore[dd];		
				}else{
					break;
				}
			}

			return stringa_int(num, sizeof(num));
		}
	}

	return 0;
}

unsigned long int cerca_file (char *nome, int size_nome){
	for (unsigned long int numero_settore = 1; numero_settore < disco_size(); numero_settore++){
		while(inb(porta_disco_default) & 0x80);
		while(!(inb(porta_disco_default) & 0x40));

		outb(1, (porta_disco_default - 5));

		outb(((numero_settore) >> 0) & 0xff, (porta_disco_default - 4));
		outb(((numero_settore) >> 8) & 0xff, (porta_disco_default - 3));
		outb(((numero_settore) >> 16) & 0xff, (porta_disco_default - 2));

		outb(0xe0 | ((0 & 1) << 4 | (((numero_settore << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

		outb(0x20, porta_disco_default);
	
		while(!(inb(porta_disco_default) & 0x08));

		char nome_settore[16];
		int caratteri_corrispondenti = 0;

		for (int dd = 0; dd < 512; dd++){
			if (dd <= 16){
				nome_settore[dd] = inw((porta_disco_default - 7));
			}
			if (nome_settore[dd] == 0x3b){
				break;
			}
		}
		outb(0xe7, porta_disco_default);

		print(nome_settore, COLORE_BIANCO_NERO);
		
		for (int dd = 0; dd < size_nome; dd++){
			if (nome[dd] == nome_settore[dd]){	caratteri_corrispondenti++;}
			else{	
				caratteri_corrispondenti = 0;
			}
		
			if (caratteri_corrispondenti == size_nome){
				return numero_settore;
			}
		}

	}
	return 0;
}

unsigned long int modifica_settore (char *buffer, unsigned long int numero_settore){
	int numero_caratteri_buffer = 0;
	int index_buffer = 0;

	unsigned long int nuovo_settore = trova_settore_libero();
	unsigned long int copia_nuovo_settore = nuovo_settore;

	if (numero_settore == 0 || numero_settore > disco_size()){
		print("NUMERO SETTORE ILLEGALE!", COLORE_ROSSOC_ROSSO);
		return 0;
	}else{
		while(inb(porta_disco_default) & 0x80);
		while(!(inb(porta_disco_default) & 0x40));

		outb(1, (porta_disco_default - 5));

		outb(((numero_settore) >> 0) & 0xff, (porta_disco_default - 4));
		outb(((numero_settore) >> 8) & 0xff, (porta_disco_default - 3));
		outb(((numero_settore) >> 16) & 0xff, (porta_disco_default - 2));

		outb(0xe0 | ((0 & 1) << 4 | (((numero_settore << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

		outb(0x30, porta_disco_default);

		while(!(inb(porta_disco_default) & 0x08));

		for(numero_caratteri_buffer; buffer[numero_caratteri_buffer] != '\0'; numero_caratteri_buffer++);

		for(int dd = 0; dd < 512; dd++){
			if (dd > 16){
				if (dd <= index_buffer){
					outw(buffer[index_buffer], (porta_disco_default - 7));
				}else{
					outw(0x00, (porta_disco_default - 7));
				}
				index_buffer++;
			}
			if (dd == 486){
				outw(0xaa, (porta_disco_default - 7));
				while (nuovo_settore > 0){
					outw((char)(nuovo_settore % 10) + 0x30, (porta_disco_default - 7));
					nuovo_settore /= 10;
				}
				outb(0xe7, porta_disco_default);
				return copia_nuovo_settore;
			}
		}
		outb(0xe7, porta_disco_default);
	}
	return 0;
}

void formatta_settore (unsigned long int numero_settore){
	if (numero_settore > disco_size()){
		print("NUMERO SETTORE ILLEGALE!", COLORE_ROSSOC_ROSSO);
		return;
	}

	if (numero_settore == 0){
		for (numero_settore = 1; numero_settore < disco_size(); numero_settore++){
			while(inb(porta_disco_default) & 0x80);
			while(!(inb(porta_disco_default) & 0x40));

			outb(1, (porta_disco_default - 5));

			outb(((numero_settore) >> 0) & 0xff, (porta_disco_default - 4));
			outb(((numero_settore) >> 8) & 0xff, (porta_disco_default - 3));
			outb(((numero_settore) >> 16) & 0xff, (porta_disco_default - 2));

			outb(0xe0 | ((0 & 1) << 4 | (((numero_settore << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

			outb(0x30, porta_disco_default);

			while(!(inb(porta_disco_default) & 0x08));
			
			for (int dd = 0; dd < 512; dd++){
				outw(0x00, (porta_disco_default - 7));
			}
			outb(0xe7, porta_disco_default);
		}
	}else{
		while(inb(porta_disco_default) & 0x80);
		while(!(inb(porta_disco_default) & 0x40));

		outb(1, (porta_disco_default - 5));

		outb(((numero_settore) >> 0) & 0xff, (porta_disco_default - 4));
		outb(((numero_settore) >> 8) & 0xff, (porta_disco_default - 3));
		outb(((numero_settore) >> 16) & 0xff, (porta_disco_default - 2));

		outb(0xe0 | ((0 & 1) << 4 | (((numero_settore << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

		outb(0x30, porta_disco_default);

		while(!(inb(porta_disco_default) & 0x08));

		for (int dd = 0; dd < 512; dd++){
			outw(0x00, (porta_disco_default - 7));
		}
		outb(0xe7, porta_disco_default);
	}

}

void stampa_lista_file (){
	print("\n", COLORE_BIANCO_NERO);
	for (unsigned long int settore_corrente = 1; settore_corrente < disco_size(); settore_corrente++){
		while(inb(porta_disco_default) & 0x80);
		while(!(inb(porta_disco_default) & 0x40));

		outb(1, (porta_disco_default - 5));

		outb(((settore_corrente) >> 0) & 0xff, (porta_disco_default - 4));
		outb(((settore_corrente) >> 8) & 0xff, (porta_disco_default - 3));
		outb(((settore_corrente) >> 16) & 0xff, (porta_disco_default - 2));
		
		outb(0xe0 | ((0 & 1) << 4 | (((settore_corrente << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

		outb(0x20, porta_disco_default);

		while(!(inb(porta_disco_default) & 0x08));

		char nome_file[16];
		int index_nome = 0;

		for (int dd = 0; dd < 512; dd++){
			char carattere = inw((porta_disco_default - 7));
			if (carattere == 0x3b){
				break;
			}
			if (index_nome < sizeof(nome_file)){
				if (carattere != 0x00 || carattere != 0x1f){
					nome_file[index_nome] = carattere;//inw((porta_disco_default - 7));	
					index_nome++;
				}
			}
		}
		
		outb(0xe7, porta_disco_default);

		print(nome_file, COLORE_BIANCO_NERO);
		print("    ( ", COLORE_GIALLO_NERO);
		printint(settore_corrente, COLORE_GIALLO_NERO);
		print(" )\n", COLORE_GIALLO_NERO);
	}
}
