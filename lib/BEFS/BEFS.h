/*
	while(inb(0x1f7) & 0x80);
	while(!(inb(0x1f7)& 0x40));
	char buffer = 0x00;
	outb(1, 0x1f2);
	outb((1 >> 0) & 0xff,0x1f3); 
 	outb((1 >> 8) & 0xff, 0x1f4);
	outb((1 >> 16) & 0xff, 0x1f5);
	outb((0xe0 | ((0 & 1) << 4) | ((1 >> 24) & 0x0f)), 0x1f6);
	outb(0x30, 0x1f7);
	while(!(inb(0x1f7) & 0x08));
	for (int dd = 0; dd < 256; dd++){
		outw(buffer, 0x1f0);
	}
	outb(0xe7, 0x1f7);
	while(inb(0x1f7) & 0x80);
	print("FINE", COLORE_BIANCO_NERO);
}*/

#include "filesystem.h"

char size_settori[20];
unsigned long int size_settori_long;
unsigned short porta_disco_default = 0x1f7;

void scelta_disco (){
	int dd = 0;
	while (dd <= 10){
		int ee = 0;
		while (ee < 256){
			if (!(inb(porta_disco_default) & 0x80)){	
				outb(0xec, porta_disco_default);
				if ((inb(porta_disco_default) & 0x08)){
					return;
				}
			}
			
			ee++;
		}
		timer(1);
		if (dd == 10){
			print("\nERRORE: Disco alla porta ", COLORE_GIALLO_NERO);
			printhex(porta_disco_default, COLORE_GIALLO_NERO);
			print(" non risponde !", COLORE_GIALLO_NERO);
			print("Cambiare porta [y/n] ?", COLORE_GIALLO_NERO);
			char scelta[8];
			input (scelta, 0x1c, sizeof(scelta), COLORE_BIANCO_NERO);
			if (scelta[0] == 'y'){
				print("\nPorta: 0x", COLORE_GIALLO_NERO);
				char porta_disco[10];
				input(porta_disco, 0x1c, sizeof(porta_disco), COLORE_BIANCO_NERO);
				porta_disco_default = stringa_unsigned_short(porta_disco, sizeof(porta_disco));
				scelta_disco();
			}else{
				scelta_disco();
			}
		}
		dd++;
	}
}

void info_disco (){
	//while (inb(0x1f7) & 0x80);
	//outb(0xec, 0x1f7);
	//while(!(inb(0x1f7) & 0x08));
	char info[256];
	for (int dd = 0; dd < 256; dd++){
		info[dd] = inw(porta_disco_default - 0x07); //0x1f0
	}


	for (int dd = 0; dd < 256; dd++){
		if (dd >= 10 && dd <= 19){
			if (dd == 10){	print("Numero disco: ", COLORE_BIANCO_NERO);}
			printchar(info[dd], COLORE_BIANCO_NERO);
			if (dd == 19){	print("\n", COLORE_BIANCO_NERO);}
		}	
		if (dd >= 23 && dd <= 26){
			if (dd == 23){	print("Versione firmware: ", COLORE_BIANCO_NERO);}
			printint(info[dd], COLORE_BIANCO_NERO);
			if (dd == 26){	print("\n", COLORE_BIANCO_NERO);}
		}
		if (dd >= 26 && dd <= 46){
			if (dd == 26){	print("Modello: ", COLORE_BIANCO_NERO);}
			printchar(info[dd], COLORE_BIANCO_NERO);
			if (dd == 46){	print("\n", COLORE_BIANCO_NERO);}
		}
		if (dd == 60){
			if (dd == 60){	print("Settori totali (512 byte): ", COLORE_BIANCO_NERO);}
			size_settori_long = ((unsigned long)info[61] << 16) | (unsigned long)info[60];
			int ee = 0;
			while (size_settori_long > 0){
				size_settori[ee++] = (char)(size_settori_long % 10) + 0x30;
				size_settori_long /= 10;
			}
			for (int ff = 0; ff < sizeof(size_settori); ff++){
				if (size_settori[ff] > 0x39){	size_settori[ff] = 0x30;}
			}
			print(size_settori, COLORE_BIANCO_NERO);
			print("\n", COLORE_BIANCO_NERO);
		}
		if (dd == 84){
			print("Supporto LBA48: ", COLORE_BIANCO_NERO);
			printint(info[84], COLORE_BIANCO_NERO);
			if (info[84] == 0){
				if (dd == 100){
					print("Capienza totale: ", COLORE_BIANCO_NERO);
					printint((unsigned long long)info[103] << 48 | (unsigned long long)info[102] << 32 | (unsigned long long)info[101] << 16 | info[100], COLORE_BIANCO_NERO);
					print("\n", COLORE_BIANCO_NERO);
				}
			
			}
			print("\n", COLORE_BIANCO_NERO);
		}
	}
}

void formatta_disco (){
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
		print("OVERFLOW DISCO! DA FORMATTARE [2^28 SETTORI]", COLORE_ROSSOC_ROSSO);
	}
	char formatta = 0x00;
	for (unsigned long int dd = 0; dd < size_settori_long + 1; dd++){
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
			outw(formatta, (porta_disco_default - 7));
		}
		outb(0xe7, porta_disco_default);
	}
}

void BEFS (){
	scelta_disco();
	info_disco();
	//crea_tabella_inode(porta_disco_default, size_settori_long, size_settori);
//	verifica_tabella_inode_esistente();
//	crea_tabella_inode();

}
