#include "BEFS.h"

void crea_file (char nome_file[], int size_nome_file){
	char buffer[256];
	input(buffer, 0x00, sizeof(buffer), COLORE_BIANCO_NERO);

	//salvare i dati poi vedere con input e interrup 0x00 è esc o \ |

}

void leggi_file (){
	//stampa, prendi anche in buffer_set_corr le informazioni che dicono se ci sono più settori
}

void verifica_esistenza_file (char nome_file[], int size_nome_file){
	char buffer_set_corr[256];
	unsigned long int trova_set_vuoto = 0;
	for (trova_set_vuoto; trova_set_vuoto < size_settori_long; trova_set_vuoto++){
		while (inb(porta_disco_default) & 0x80);
		while (!(inb(porta_disco_default) & 0x40));

		outb(1, 0x1f2);
		outb(((trova_set_vuoto << 20) >> 0) & 0xff, 0x1f3);
		outb(((trova_set_vuoto << 12) >> 8) & 0xff, 0x1f4);
		outb(((trova_set_vuoto << 4) >> 16) & 0xff, 0x1f5);

		outb(0xe0 | ((0 & 1) << 4 | (((trova_set_vuoto << 4) >> 24) & 0x0f)), 0x1f6);
		outb(0x20, porta_disco_default);

		while (!(inb(porta_disco_default) & 0x08));
		for (int buffer_set = 0; buffer_set < sizeof(buffer_set_corr); buffer_set++){
			buffer_set_corr[buffer_set] = inw(0x1f0);
		}
		int corretto = 0;
		for (int corrente = 0; corrente < size_nome_file; corrente++){
			if (nome_file[corrente] == buffer_set_corr[corrente]){	corretto++;}
		}
		if (corretto == size_nome_file){
			leggi_file();
		}
	}

	for (trova_set_vuoto = 0; trova_set_vuoto < size_settori_long; trova_set_vuoto++){
		while (inb(porta_disco_default) & 0x80);
		while (!(inb(porta_disco_default) & 0x40));

		outb(1, 0x1f2);
		outb(((trova_set_vuoto << 20) >> 0) & 0xff, 0x1f3);
		outb(((trova_set_vuoto << 12) >> 8) & 0xff, 0x1f4);
		outb(((trova_set_vuoto << 4) >> 16) & 0xff, 0x1f5);

		outb(0xe0 | ((0 & 1) << 4 | (((trova_set_vuoto << 4) >> 24) & 0x0f)), 0x1f6);
		outb(0x20, porta_disco_default);

		while (!(inb(porta_disco_default) & 0x08));
		for (int buffer_set = 0; buffer_set < sizeof(buffer_set_corr); buffer_set++){
			buffer_set_corr[buffer_set] = inw(0x1f0);
		}
		int vuoto = 0;
		for (int buffer_set = 0; buffer_set < sizeof(buffer_set_corr); buffer_set++){
			if (buffer_set_corr[buffer_set] == 0x00){	vuoto++;}
		}
		if (vuoto == sizeof(buffer_set_corr)){
			crea_file(nome_file, size_nome_file);
		}
	} 

}

