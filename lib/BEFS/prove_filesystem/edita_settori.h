extern unsigned short porta_disco_default;
extern unsigned long int size_settori_long;
extern char size_settori[20];

char char_settore_file[10];
unsigned long int settore_file = 0;

void scrivi_settore(){
	char firma_continuazione_file = 0xaa;

	int num_caratteri_buffer = 0;
	char buffer[502];
	while (1){
		input(buffer, 0x1c, sizeof(buffer), COLORE_BIANCO_NERO);

		while(inb(porta_disco_default) & 0x80);
		while(!(inb(porta_disco_default) & 0x40));

		outb(settore_file, (porta_disco_default - 5));

		outb(((settore_file) >> 0) & 0xff, (porta_disco_default - 4));
		outb(((settore_file) >> 8) & 0xff, (porta_disco_default - 3));
		outb(((settore_file) >> 16) & 0xff, (porta_disco_default - 2));
	
		outb(0xe0 | ((0 & 1) << 4 | (((settore_file << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

		outb(0x30, porta_disco_default);

		while(!(inb(porta_disco_default) & 0x08));
	
		//int num_caratteri_buffer = 0;
		int dd = 0;
		while (buffer[dd] != '\0'){	
			if (buffer[dd] == 0x1c){	break;}
			num_caratteri_buffer++;
			dd++;
		}

		for (int ee = 0; ee < 512; ee++){
			outb(buffer[ee],(porta_disco_default - 7));
			if (ee == 502 && num_caratteri_buffer >= 496){	
				settore_libero_per_file(char_settore_file);
				outw(firma_continuazione_file, (porta_disco_default - 7));
				for (int ff = 0; ff < sizeof(char_settore_file); ff++){
					outw(char_settore_file[ff], (porta_disco_default - 7));
				}
				outw(firma_continuazione_file, (porta_disco_default - 7));
				outb(0xe7, porta_disco_default);
			
				settore_file = stringa_int(char_settore_file, sizeof(char_settore_file));
				break;
			}
		}
		outb(0xe7, porta_disco_default);
		break;
	}
	for (int ee = 0; ee < 512; ee++){
		outw(buffer[ee],(porta_disco_default - 7));
		if (ee == 502 && num_caratteri_buffer >= 496){	
			settore_libero_per_file(char_settore_file);
			outw(firma_continuazione_file, (porta_disco_default - 7));
			for (int ff = 0; ff < sizeof(char_settore_file); ff++){
				outw(char_settore_file[ff], (porta_disco_default - 7));
			}
			outw(firma_continuazione_file, (porta_disco_default - 7));
			outb(0xe7, porta_disco_default);
			
			settore_file = stringa_int(char_settore_file, sizeof(char_settore_file));
			break;
		}
	}
	outb(0xe7, porta_disco_default);

}

void start_editor(char nome[], int size_nome){
	crea_inode(nome, size_nome);
	char numero_settore_file[20];
	trova_inode(nome, size_nome, numero_settore_file);
	
	if (settore_file == 0){
		settore_libero_per_file(numero_settore_file);
		settore_file = stringa_int(numero_settore_file, sizeof(numero_settore_file));
	}
	scrivi_settore();
}
