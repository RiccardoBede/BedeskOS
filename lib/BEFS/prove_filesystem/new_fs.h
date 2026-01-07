extern unsigned short porta_disco_default;
extern unsigned long int size_settori_long;
extern char size_settori[20];


//nell inserimento di un inode ci dovrebbe essere un errore per cui è segnato il contatore dd come numero del settore corrispondente
//al file. Bisogna implemenare una funzione per trovare il settore da usare


/*unsigned short inwB (unsigned short porta){
	unsigned short buffer;
	__asm__("inw %%dx, %%ax" : "=a" (buffer) : "dN" (porta));
	return buffer;
}

void outwB (unsigned short buffer, unsigned short porta){
	__asm__("outw %0, %1" : : "a" (buffer), "dN" (porta));
}*/

void disco_size (){
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
		size_settori_long = potenza(2, 28);
		print("OVERFLOW DISCO! [PRESI 2^28 SETTORI]", COLORE_GIALLO_NERO);
	}
}

void crea_tabella_inode (){
	disco_size();
	print("DENTRO", COLORE_VERDE_NERO);	
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
			if (ee <= 8 && dd == 0){
				outw((char)0xff, (porta_disco_default - 7));
			}else{
				outw(0x00, (porta_disco_default - 7));
			}
		}
		outb(0xe7, porta_disco_default);
	}
	return;	
}

void trova_firma_tabella (){
	disco_size();
	
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

		char ricerca_firma[8];
		int index_ricerca = 0;
		char possibile_firma[256];

		for (int ee = 0; ee < 256; ee++){
			possibile_firma[ee] = inw((porta_disco_default - 7));
			
			if (index_ricerca >= sizeof(ricerca_firma)){	
				print("TABELLA INODE TROVATA!", COLORE_VERDE_NERO);
				outb(0xe7, porta_disco_default);	
				return;
			}
		
			if (possibile_firma != 0xffff){
				continue;
			}else{
				printhex(possibile_firma[ee], COLORE_GIALLO_NERO);
				ricerca_firma[index_ricerca] = possibile_firma[ee];
				index_ricerca++;
			}	
		}
	}
	outb(0xe7, porta_disco_default);
	print("TABELLA INODE NON TROVATA! (CREAZIONE TAB.)", COLORE_GIALLO_NERO);
	crea_tabella_inode();
}

void crea_inode (char nome[], int size_nome){
	char settore_inode[512];

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

		for (int ee = 0; ee < 512; ee++){
			settore_inode[ee] = inw((porta_disco_default - 7));
		}
		outb(0xe7, porta_disco_default);

		int numero_spazi_per_file = 0;

		for (int ee = 0; ee < sizeof(settore_inode); ee++){
			if(numero_spazi_per_file >= (size_nome + 2 + num_cifre_numero(dd))){
				int num_cifre_nome = 0;
				for (int ff = (ee - (size_nome + 2 + num_cifre_numero(dd))); ff < (size_nome + 2 + num_cifre_numero(dd)); ff++){
					if (num_cifre_nome >= size_nome){
						settore_inode[ff++] = 0x3b;
						int numero_settore = dd;
						while (numero_settore > 0){
							settore_inode[ff] = (char)(numero_settore % 10) + 0x30;
							numero_settore /= 10;
							ff++;
						}
						settore_inode[ff++] = 0x3b;
						outb(0xe7, porta_disco_default);
										
						while(inb(porta_disco_default) & 0x80);
						while(!(inb(porta_disco_default) & 0x40));

						outb(dd, (porta_disco_default - 5));

						outb(((dd) >> 0) & 0xff, (porta_disco_default - 4));
						outb(((dd) >> 8) & 0xff, (porta_disco_default - 3));
						outb(((dd) >> 16) & 0xff, (porta_disco_default - 2));

						outb(0xe0 | ((0 & 1) << 4 | (((dd << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

						outb(0x30, porta_disco_default);

						while(!(inb(porta_disco_default) & 0x08));
					
						for (int gg = 0; gg < sizeof(settore_inode); gg++){
							outw(settore_inode[gg], (porta_disco_default - 7));
						}
						outb(0xe7, porta_disco_default);

						return;
					}
					settore_inode[ff] = nome[num_cifre_nome];
					num_cifre_nome++;
				}
			}

			if (settore_inode[ee] < 0x20){
				numero_spazi_per_file++;
			}else{	numero_spazi_per_file = 0;}
		}

	}
	print("IMPOSSIBILE AGGIUNGERE IL FILE, TABELLA PIENA => (DISCO PIENO)!", COLORE_ROSSOC_ROSSO);	
}

void elimina_inode (char nome[], int size_nome){
	char settore_inode[512];

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

		for (int ee = 0; ee < 512; ee++){
			settore_inode[ee] = inw((porta_disco_default - 7));
		}
		outb(0xe7, porta_disco_default);

		int numero_lettere_corrispondenti_nome_file = 0;
		int index_nome = 0;

		for (int ee = 0; ee < sizeof(settore_inode); ee++){
			if (numero_lettere_corrispondenti_nome_file >= size_nome){
				ee++;
				int numero_settore = ee;
				while(settore_inode[numero_settore] != 0x3b){
					numero_settore++;
				}
				numero_settore = ee - numero_settore;
				ee++;
				for (int ff = (ee - (size_nome + numero_settore)); ff < (size_nome + numero_settore); ff++){
					settore_inode[ff] = 0x00;
				}
								
				while(inb(porta_disco_default) & 0x80);
				while(!(inb(porta_disco_default) & 0x40));

				outb(dd, (porta_disco_default - 5));

				outb(((dd) >> 0) & 0xff, (porta_disco_default - 4));
				outb(((dd) >> 8) & 0xff, (porta_disco_default - 3));
				outb(((dd) >> 16) & 0xff, (porta_disco_default - 2));

				outb(0xe0 | ((0 & 1) << 4 | (((dd << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

				outb(0x30, porta_disco_default);

				while(!(inb(porta_disco_default) & 0x08));
				
				for (int gg = 0; gg < sizeof(settore_inode); gg++){
					outw(settore_inode[gg], (porta_disco_default - 7));
				}
				outb(0xe7, porta_disco_default);
				return;

			}
			
			if (settore_inode[ee] == nome[index_nome]){
				numero_lettere_corrispondenti_nome_file++;
				index_nome++;
			}else{
				numero_lettere_corrispondenti_nome_file = 0;
				index_nome = 0;
			}
		}

	}
	print("IMPOSSIBILE ELIMINARE IL FILE, [FILE INESISTENTE]!", COLORE_ROSSOC_ROSSO);	
}

unsigned long int cerca_inode (char nome[], int size_nome){
	char settore_inode[512];

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

		for (int ee = 0; ee < 512; ee++){
			settore_inode[ee] = inw((porta_disco_default - 7));
		}
		outb(0xe7, porta_disco_default);

		int numero_lettere_corrispondenti_nome_file = 0;
		int index_nome = 0;

		for (int ee = 0; ee < sizeof(settore_inode); ee++){
			if (numero_lettere_corrispondenti_nome_file >= size_nome){
				ee++;
				int numero_settore = ee;
				char array_numero_settore[20];
				int index_array = 0;
				while(settore_inode[numero_settore] != 0x3b){
					array_numero_settore[index_array] = settore_inode[numero_settore];
				       numero_settore++;
				       index_array++;
				}
				return stringa_int(array_numero_settore, sizeof(array_numero_settore));
			}
			
			if (settore_inode[ee] == nome[index_nome]){
				numero_lettere_corrispondenti_nome_file++;
				index_nome++;
			}else{
				numero_lettere_corrispondenti_nome_file = 0;
				index_nome = 0;
			}
		}

	}
	print("IMPOSSIBILE TROVARE IL FILE, [FILE INESISTENTE]!", COLORE_ROSSOC_ROSSO);	
	return 0;
}
