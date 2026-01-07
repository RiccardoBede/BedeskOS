extern unsigned short porta_disco_default;
extern unsigned long int size_settori_long;
extern char size_settori[20];

long unsigned int trova_settore_libero (){
	disco_size();

	for (unsigned long int dd = 0; dd < (size_settori_long); dd++){
		while (inb(porta_disco_default) & 0x80);
		while (!(inb(porta_disco_default) & 0x40));

		outb(1, (porta_disco_default - 5));

		outb(((dd) >> 0) & 0xff, (porta_disco_default - 4));
		outb(((dd) >> 8) & 0xff, (porta_disco_default - 3));
		outb(((dd) >> 16) & 0xff, (porta_disco_default - 2));

		outb(0xe0 | ((0 & 1) << 4 | (((dd << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

		outb(0x20, porta_disco_default);

		while (!(inb(porta_disco_default) & 0x08));
		
		char settore[256];
		int contatore_byte_liberi = 0;

		for (int ee = 0; ee < 256; ee++){
			settore[ee] = inw((porta_disco_default - 7));
		}
		outb(0xe7, porta_disco_default);
		
		for (int ee = 0; ee < sizeof(settore); ee++){
			if (settore[ee] < 0x30){
				contatore_byte_liberi++;
			}
			else{	break;}
		}

		if (contatore_byte_liberi == sizeof(settore)){	
			outb(0xe7, porta_disco_default);
			printint(dd+1, COLORE_GIALLO_NERO);
			return dd+1;
		}

	}	
}

void scrivi_settore (char nome[], unsigned long int settore){
	while (1){
		char buffer[256 - (10 + 16 + 1)];

		input(buffer, 0x1c, sizeof(buffer), COLORE_BIANCO_NERO);

		while (inb(porta_disco_default) & 0x80);
		while (!(inb(porta_disco_default) & 0x40));

		outb(1, (porta_disco_default - 5));

		outb(((settore) >> 0) & 0xff, (porta_disco_default - 4));
		outb(((settore) >> 8) & 0xff, (porta_disco_default - 3));
		outb(((settore) >> 16) & 0xff, (porta_disco_default - 2));

		outb(0xe0 | ((0 & 1) << 4 | (((settore << 4) >> 24) & 0x0f)), (porta_disco_default - 1));

		outb(0x30, porta_disco_default);

		while (!(inb(porta_disco_default) & 0x08));

		int numero_caratteri_buffer = 0;
		for (numero_caratteri_buffer; buffer[numero_caratteri_buffer] != '\0'; numero_caratteri_buffer++);
		int index_buffer = 0;
		for (int ee = 0; ee < 256; ee++){
			if (ee <= 16 && nome[ee] != '\0'){	
				outb(nome[ee], (porta_disco_default - 7));
			}
			if (nome[ee] == '\0'){	outb(0x3b, (porta_disco_default - 7));}
			printint(numero_caratteri_buffer, COLORE_GIALLO_NERO);
			if (numero_caratteri_buffer == sizeof(buffer) && ee == sizeof(buffer)){
				outb(0xaa, (porta_disco_default - 7));
					
				settore = trova_settore_libero();
				printint(settore, COLORE_GIALLO_NERO);

				outb((char)(settore & 0xff), (porta_disco_default - 7));
				outb((char)((settore >> 8) & 0xff), (porta_disco_default - 7));
				outb((char)((settore >> 16) & 0xff), (porta_disco_default - 7));
				outb((char)((settore >> 24) & 0xff), (porta_disco_default - 7));

				scrivi_settore(nome, settore);
			}

			outb(buffer[index_buffer], (porta_disco_default - 7));
			index_buffer++;

			if (buffer[index_buffer] == ':' && buffer[index_buffer+1] == 'q'){
				outb(0xe7, porta_disco_default);
				return;
			}

			if (buffer[index_buffer] == '\0'){
				break;
			}
		}
		outb(0xe7, porta_disco_default);
	}
}
