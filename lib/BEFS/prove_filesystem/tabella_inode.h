/*//ALGORITMO PER GESTIRE GRANDEZZE PIU' GRANDI DI 10 CIFRE
 	char num[10]; 
	int cif = 0;
	int array_cicli[6];

	for (int dd = 0; dd < sizeof(num); dd++){	
		if (settori[dd] > 0x30){
			num[dd] = settori[dd];
		}else{
			break;
		}
	}
	printint(stringa_int(num, sizeof(num)),COLORE_GIALLO_NERO);
	for (int dd = 0; dd < sizeof(size_settori); dd++){
		if (settori[dd] > 0x30){
			cif++;
		}
	}
	cif -= sizeof(num);
	if (cif < 0){	cif = 0;}
	printint(cif, COLORE_ROSSO_NERO);

	numeri_oltre_lim_ns(stringa_int(num, sizeof(num)), cif, array_cicli, (sizeof(array_cicli) / sizeof(int)));

	for (int dd = 0; dd < sizeof(array_cicli) / sizeof(int); dd++){
		printint(array_cicli[dd], COLORE_BIANCO_NERO);
		printchar(',', COLORE_BIANCO_NERO);
	}*/

	/*//CALCOLO GRANDEZZA TABELLA
	array_cicli[0] = array_cicli[0] / 512 * 40;
	for (int ee = 1; ee < (sizeof(array_cicli) / sizeof(int)); ee++){
		array_cicli[ee] = potenza(10, array_cicli[ee]) / 512 * 40;
	}
	array_cicli[sizeof(array_cicli) / sizeof(int)] += 1;

	for (int ee = 0; ee < 5; ee++){
		printint(array_cicli[ee], COLORE_BIANCO_NERO);
		printchar(',', COLORE_BIANCO_NERO);
	}
	*/
	//size_settori_long = size_settori_long / 512 * 40;

	//SCRITTURA TABELLA
	/*for (int dd = 0; dd < size_settori_long / 512 * 40 + 1; dd++){
		while(inb(porta_disco_default) & 0x80);
		while(!(inb(porta_disco_default) & 0x40));
		
		outb(1, 0x1f2);
	
		outb(((dd << 20) >> 0) & 0xff, 0x1f3);
		outb(((dd << 12) >> 8) & 0xff, 0x1f4);
		outb(((dd << 4) >> 16) & 0xff, 0x1f5);

		outb(0xe0 | ((0 & 1) << 4 | (((dd << 4) >> 24) & 0x0f)), 0x1f6);

		outb(0x30, porta_disco_default);

		while(!(inb(porta_disco_default) & 0x08));
			
		for(int ee = 0; ee < 256; ee++){
			outw(0x00, 0x1f0);
		}
	
		outb(0xe7, 0x1f7); // FFLUSH
	}*/
extern unsigned short porta_disco_default;
extern unsigned long int size_settori_long;
extern char size_settori[20];

void settore_libero_per_file (char coordinate_settore_libero[]){	
	char buffer_settore_corrente[256];
	char num[10];
	for (int dd = 0; dd < sizeof(num); dd++){	
		if (size_settori[dd] > 0x30){
			num[dd] = size_settori[dd];
		}else{
			break;
		}
	}
	size_settori_long = stringa_int(num, sizeof(num));

	if (size_settori_long > (potenza(2, 28) - 1)){	
		size_settori_long = potenza(2, 28) - 1;
		print("OVERFLOW DISCO! [PRESI 2^28 SETTORI]", COLORE_GIALLO_NERO);
	}

	for (unsigned long int dd = ((size_settori_long / 512 * 40) + 1); dd < size_settori_long; dd++){
		while(inb(porta_disco_default) & 0x80);
		while(!(inb(porta_disco_default) & 0x40));

		outb(dd, (porta_disco_default - 5));

		outb(((dd) >> 0) & 0xff, (porta_disco_default - 4));
		outb(((dd) >> 8) & 0xff, (porta_disco_default - 3));
		outb(((dd) >> 16) & 0xff, (porta_disco_default - 2));

		outb(0xe0 | ((0 & 1) << 4 | (((dd << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

		outb(0x20, porta_disco_default);

		while(!(inb(porta_disco_default) & 0x08));
		
		for (int ee = 0; ee < 256; ee++){
			buffer_settore_corrente[ee] = inw((porta_disco_default - 7));
		}
		int num_spazi_liberi = 0;
		for (int ee = 0; ee < sizeof(buffer_settore_corrente); ee++){
			if (buffer_settore_corrente[ee] == 0x00){
				num_spazi_liberi++;
			}else{
				break;
			}
			if (num_spazi_liberi == 256){
				int ff = 0;
				while (dd > 0){
					coordinate_settore_libero[ff++] = (char)(dd % 10) + 0x30;
					dd /= 10;
				}
				outb(0xe7, porta_disco_default);
				return;
			}
		}
		outb(0xe7, porta_disco_default);
	}
}

void crea_inode (char nome[], int size_nome){
	char buffer_settore_corr[256];
	unsigned long int num_settore_corrente = 0;

	char num[10];
	for (int dd = 0; dd < sizeof(num); dd++){	
		if (size_settori[dd] > 0x30){
			num[dd] = size_settori[dd];
		}else{
			break;
		}
	}
	size_settori_long = stringa_int(num, sizeof(num));

	if (size_settori_long > (potenza(2, 28) - 1)){	
		size_settori_long = potenza(2, 28) - 1;
		print("OVERFLOW DISCO! [PRESI 2^28 SETTORI]", COLORE_GIALLO_NERO);
	}

	for (unsigned long int dd = 0; dd < ((size_settori_long / 512 * 40) + 1); dd++){
		while(inb(porta_disco_default) & 0x80);
		while(!(inb(porta_disco_default) & 0x40));

		outb(dd, (porta_disco_default - 5));

		outb(((dd) >> 0) & 0xff, (porta_disco_default - 4));
		outb(((dd) >> 8) & 0xff, (porta_disco_default - 3));
		outb(((dd) >> 16) & 0xff, (porta_disco_default - 2));

		outb(0xe0 | ((0 & 1) << 4 | (((dd << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

		outb(0x20, porta_disco_default);

		while(!(inb(porta_disco_default) & 0x08));

		for (int ee = 0; ee < 256; ee++){
			buffer_settore_corr[ee] = inw((porta_disco_default - 7));
		}
		int num_spazi_liberi = 0;
		for (int ee = 0; ee < sizeof(buffer_settore_corr); ee++){
			if (buffer_settore_corr[ee] == 0x00){	
				num_spazi_liberi++;
			}else{
				num_spazi_liberi = 0;
			}
			if (num_spazi_liberi == 42){	
				while(inb(porta_disco_default) & 0x80);
				while(!(inb(porta_disco_default) & 0x40));
				
				outb(dd, (porta_disco_default - 5));
				
				outb(((dd) >> 0) & 0xff, (porta_disco_default - 4)); 	
				outb(((dd) >> 8) & 0xff, (porta_disco_default - 3)); 	
				outb(((dd) >> 16) & 0xff, (porta_disco_default - 2));

				outb(0xe0 | ((0 & 1) << 4 | (((dd << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

				outb(0x30, porta_disco_default);

				while(!(inb(porta_disco_default) & 0x08));
				char coordinate_settore_libero[20];
				for (int ff = 0; ff < 256; ff++){
					if (ff == (ee - 40)){
						for (int gg = 0; gg < size_nome; gg++){
							outw(nome[gg], (porta_disco_default - 7));
						}
						outw(0x3b, (porta_disco_default - 7));
						settore_libero_per_file(coordinate_settore_libero);
						for (int gg = 0; gg < sizeof(coordinate_settore_libero); gg++){
							if (coordinate_settore_libero[gg] != '\0'){
								outw(coordinate_settore_libero[gg], (porta_disco_default - 7)); //numero del settore libero dopo la tabella
								printint(stringa_int(coordinate_settore_libero, sizeof(coordinate_settore_libero)), COLORE_GIALLO_NERO);
							}
						}
						outw(0x3b, (porta_disco_default - 7));
					}
					outw(0x00, (porta_disco_default - 7));
				}

				outb(0xe7, porta_disco_default);
				return;
			}
		}
		outb(0xe7, porta_disco_default);
	}
	print("TABELLA INODE PIENA! (DISCO PIENO)", COLORE_ROSSOC_ROSSO);

}

void trova_inode (char nome[], int size_nome, char numero_settore_file[]){
	char num[10];
	char buffer_settore_corr[256];
	for (int dd = 0; dd < sizeof(num); dd++){	
		if (size_settori[dd] > 0x30){
			num[dd] = size_settori[dd];
		}else{
			break;
		}
	}
	size_settori_long = stringa_int(num, sizeof(num));

	if (size_settori_long > (potenza(2, 28) - 1)){	
		size_settori_long = potenza(2, 28) - 1;
		print("OVERFLOW DISCO! [PRESI 2^28 SETTORI]", COLORE_GIALLO_NERO);
	}

	for (unsigned long int dd = 0; dd < ((size_settori_long / 512) * 40); dd++){
		while(inb(porta_disco_default) & 0x80);
		while(!(inb(porta_disco_default) & 0x40));

		outb(dd, (porta_disco_default - 5));

		outb(((dd) >> 0) & 0xff, (porta_disco_default - 4));
		outb(((dd) >> 8) & 0xff, (porta_disco_default - 3));
		outb(((dd) >> 16) & 0xff, (porta_disco_default - 2));

		outb(0xe0 | ((0 & 1) << 4 | (((dd << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

		outb(0x20, porta_disco_default);

		while(!(inb(porta_disco_default) & 0x08));
		int corrispondenza_nome_file = 0;
		for (int ee = 0; ee < 256; ee++){
			buffer_settore_corr[ee] = inw((porta_disco_default - 7));
		}
		int ff = 0;
		for (int ee = 0; ee < 256; ee++){
			if (buffer_settore_corr[ee] == nome[ff]){
				corrispondenza_nome_file++;
			}else{	
				corrispondenza_nome_file = 0;
			}

			if (corrispondenza_nome_file == size_nome*2){
				int hh = 0;
				for (int gg = ee; gg < (256 - ee); gg++){
					if (buffer_settore_corr[ee] == 0x3b){
						while (buffer_settore_corr[ee] != 0x3b){
							numero_settore_file[hh] = buffer_settore_corr[ee];
							ee++;
							hh++;
						}
						outb(0xe7, (porta_disco_default));
						return;
					}
				}
			}
			if (ff == size_nome){ 	ff = 0;}
		}

	}
	print("IL FILE NON ESISTE => (CREAZIONE)", COLORE_GIALLO_NERO);
	return;
}

/*void modifica_inode (char *nome, int settori[], char *cartella){

}*/

void elimina_inode (char *nome){
	char num[10];
	for (int dd = 0; dd < sizeof(num); dd++){	
		if (size_settori[dd] > 0x30){
			num[dd] = size_settori[dd];
		}else{
			break;
		}
	}
	size_settori_long = stringa_int(num, sizeof(num));

	if (size_settori_long > (potenza(2, 28) - 1)){	
		size_settori_long = potenza(2, 28) - 1;
		print("OVERFLOW DISCO! [PRESI 2^28 SETTORI]", COLORE_GIALLO_NERO);
	}

	for (unsigned long int dd = 0; dd < ((size_settori_long / 512 * 40) + 1); dd++){
		while(inb(porta_disco_default) & 0x80);
		while(!(inb(porta_disco_default) & 0x40));

		outb(dd, (porta_disco_default - 5));

		outb(((dd) >> 0) & 0xff, (porta_disco_default - 4));
		outb(((dd) >> 8) & 0xff, (porta_disco_default - 3));
		outb(((dd) >> 16) & 0xff, (porta_disco_default - 2));

		outb(0xe0 | ((0 & 1) << 4 | (((dd << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

		outb(0x20, porta_disco_default);

		while(!(inb(porta_disco_default) & 0x08));
		char buffer_nome_file[256];
		int num_caratteri_corr = 0;	
		for (int ee = 0; ee < 256; ee++){
			buffer_nome_file[ee] = inw((porta_disco_default - 7)); //vedere se il nome del file è presente e se è uguale ogni carattere se si, si fa l'override di 40 caratteri compresi
			       //quelli del nome con 0x00
		}
		int ff = 0;
		for (int ee = 0; ee < 256; ee++){
			if (buffer_nome_file[ee] == nome[ff]) { 
				num_caratteri_corr++;
			}else{	num_caratteri_corr = 0;}

			if (num_caratteri_corr == 16){
				while(inb(porta_disco_default) & 0x80);
				while(!(inb(porta_disco_default) & 0x40));

				outb(dd, (porta_disco_default - 5));

				outb(((dd) >> 0) & 0xff, (porta_disco_default - 4));
				outb(((dd) >> 8) & 0xff, (porta_disco_default - 3));
				outb(((dd) >> 16) & 0xff, (porta_disco_default - 2));

				outb(0xe0 | ((0 & 1) << 4 | (((dd << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

				outb(0x30, porta_disco_default);

				while(!(inb(porta_disco_default) & 0x08));

				for (int gg = 0; gg < 256; gg++){
					if (gg == ee - 16){
						for (int hh = 0; hh < 40; hh++){
							outw(0x00,(porta_disco_default - 7));
						}
					}
				}
								
				outb(0xe7, porta_disco_default);
				return;
			}

			if (ff == 16){	ff = 0;}
			ff++;
		}
		outb(0xe7, porta_disco_default);
	}
	print("FILE INESISTENTE!", COLORE_ROSSOC_ROSSO);
}

void crea_tabella_inode (){	
 	char num[10];
	for (int dd = 0; dd < sizeof(num); dd++){	
		if (size_settori[dd] > 0x30){
			num[dd] = size_settori[dd];
		}else{
			break;
		}
	}
	size_settori_long = stringa_int(num, sizeof(num));

	if (size_settori_long > (potenza(2, 28) - 1)){	
		size_settori_long = potenza(2, 28) - 1;
		print("OVERFLOW DISCO! [PRESI 2^28 SETTORI]", COLORE_GIALLO_NERO);
	}
	/*char scrivi[5];
	input(scrivi, 0x1c, sizeof(scrivi), COLORE_GIALLO_NERO);*/
	
	char inizializzazione_tab = 0x00;
	char firma_tab = 0xff;
	
	for (unsigned long int dd = 0; dd < ((size_settori_long / 512 * 40) + 1); dd++){
		while(inb(porta_disco_default) & 0x80);
		while(!(inb(porta_disco_default) & 0x40));

		outb(dd, (porta_disco_default - 5));

		outb(((dd) >> 0) & 0xff, (porta_disco_default - 4));
		outb(((dd) >> 8) & 0xff, (porta_disco_default - 3));
		outb(((dd) >> 16) & 0xff, (porta_disco_default - 2));

		outb(0xe0 | ((0 & 1) << 4 | (((dd << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

		outb(0x30, porta_disco_default);

		while(!(inb(porta_disco_default) & 0x08));

		for (int ee = 0; ee < 256; ee++){
			if (ee < 8 && dd == 0){	
				outw(firma_tab, (porta_disco_default - 7));
			}else{
				outw(inizializzazione_tab, (porta_disco_default - 7));
			}
		}
		outb(0xe7, porta_disco_default);
	}	

}

void verifica_tabella_inode_esistente (){
	char num[10];
	for (int dd = 0; dd < sizeof(num); dd++){	
		if (size_settori[dd] > 0x30){
			num[dd] = size_settori[dd];
		}else{
			break;
		}
	}
	size_settori_long = stringa_int(num, sizeof(num));

	if (size_settori_long > (potenza(2, 28) - 1)){	
		size_settori_long = potenza(2, 28) - 1;
		print("OVERFLOW DISCO! [PRESI 2^28 SETTORI]", COLORE_GIALLO_NERO);
	}
	unsigned char verifica_firma[256];
	for (unsigned long int dd = 0; dd < size_settori_long; dd++){
		while(inb(porta_disco_default) & 0x80);
		while(!(inb(porta_disco_default) & 0x40));

		outb(dd, (porta_disco_default - 5));

		outb(((dd) >> 0) & 0xff, (porta_disco_default - 4));
		outb(((dd) >> 8) & 0xff, (porta_disco_default - 3));
		outb(((dd) >> 16) & 0xff, (porta_disco_default - 2));

		outb(0xe0 | ((0 & 1) << 4 | (((dd << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

		outb(0x20, porta_disco_default);

		while(!(inb(porta_disco_default) & 0x08));

		for (int ee = 0; ee < 256; ee++){
			verifica_firma[ee] = inw((porta_disco_default - 7));
		}
		int ff = 0;
		for (int ee = 0; ee < 256; ee++){
			if (verifica_firma[ee] == 0xff){
				ff++;
			}else{
				ff = 0;
			}
			if (ff == 8){
				outb(0xe7, porta_disco_default);
				return;
			}
		}
		outb(0xe7, porta_disco_default);
	}
	print("TABELLA INODE INESISTENTE! (CREAZIONE TAB. INODE)", COLORE_GIALLO_NERO);
	crea_tabella_inode();
}
