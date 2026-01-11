//SCHEMI COLORE PER STAMPA TESTO A SCHERMO
//	
//	COLORE_(COLORE_TESTO)_(COLORE_SFONDO)

#define VIDEO_MEMORY 0xb8000
#define CARATTERI_RIGA 80
#define RIGHE 25

#define COLORE_NERO_NERO 0x0
#define COLORE_BLU_NERO 0x1
#define COLORE_VERDE_NERO 0x2
#define COLORE_CIANO_NERO 0x3
#define COLORE_ROSSO_NERO 0x4
#define COLORE_MAGENTA_NERO 0x5
#define COLORE_GIALLO_NERO 0x6
#define COLORE_BIANCO_NERO 0x7
#define COLORE_GRIGIOS_NERO 0x8
#define COLORE_CIANO_BLU 0x9
#define COLORE_VERDEC_VERDE 0xa
#define COLORE_CIANO_CIANO 0xb
#define COLORE_ROSSOC_ROSSO 0xc
#define COLORE_MAGENTAC_MAGENTA 0xd
#define COLORE_GIALLOC_GIALLO 0xe
#define COLORE_BIANCOC_BIANOCO 0xf

//BUFFER TATSIERA
#define BUFFER_TASTIERA 0x60
#define STATO_BUFFER_TASTIERA 0x64

char inb (unsigned short porta){
	char buffer;
	__asm__ ("in %%dx, %%al" : "=a" (buffer) : "d" (porta));
	return buffer;
}

void outb (char comando, unsigned short  porta){
	__asm__ ("outb %0, %1" : : "a" (comando), "dN" (porta));
}

void outw (unsigned short buffer, unsigned short porta){
	__asm__ ("outw %0, %1" : : "a" (buffer), "dN" (porta));
}

char inw (unsigned short porta){
	char buffer;
	__asm__ ("inw %%dx, %%ax" : "=a" (buffer) : "dN" (porta));
	return buffer;
}

/*typedef enum{
	true,
	false
}bool;*/

typedef struct{
	char carattere;
	char colore;
}CarattereColore;

static int cc = 0;

void cursore (){
	CarattereColore *video_memory = (CarattereColore *)VIDEO_MEMORY;
	video_memory[cc] = (CarattereColore){219, COLORE_BIANCO_NERO};
}

void cancella_cursore(){
	CarattereColore *video_memory = (CarattereColore *)VIDEO_MEMORY;
	video_memory[cc] = (CarattereColore){' ', COLORE_NERO_NERO};
}

void acapo() { 
	CarattereColore *video_memory = (CarattereColore *)VIDEO_MEMORY;
	int caratteri_restanti = CARATTERI_RIGA - (cc % CARATTERI_RIGA);
	for (int i = 0; i < caratteri_restanti; i++) { 
		video_memory[cc] = (CarattereColore){' ', COLORE_NERO_NERO};
		cc++;
	} 
	if (cc % CARATTERI_RIGA != 0){
		cc += CARATTERI_RIGA - (cc % CARATTERI_RIGA);
	}
}

void cancella_char (){
	CarattereColore *video_memory = (CarattereColore *)VIDEO_MEMORY;
	video_memory[cc] = (CarattereColore){0x00, COLORE_NERO_NERO};
	cc--;
}

void print (const char messaggio[], char colore){
	CarattereColore *video_memory = (CarattereColore *)VIDEO_MEMORY;
	int index_messaggio = 0;
	cancella_cursore();
	for (cc; messaggio[index_messaggio] != '\0'; cc++){
		/*if (messaggio[cc] == '\n'){
			acapo();
			index_messaggio++;
		}*/
		video_memory[cc] = (CarattereColore){messaggio[index_messaggio], colore};
		index_messaggio++;
		if (index_messaggio > 0 && messaggio[index_messaggio - 1] == '\n'){
			acapo();
		}
	}	
	cursore();
}

void printchar (const char carattere, char colore){
	CarattereColore *video_memory = (CarattereColore *)VIDEO_MEMORY;
	cancella_cursore();
	video_memory[cc] = (CarattereColore){carattere, colore};
	cc++;
	cursore();
}

void printint (long int numero, char colore){
	//int numero = *(int *)p_numero;
	char buffer[20];
	long int dd = 0;
	if (numero == 0){
		buffer[dd++] = '0';
		printchar('0', colore);
	}else{
		if (numero < 0){
			printchar('-', colore);
			numero = -numero;
		}
		while (numero != 0){
			buffer[dd++] = (numero % 10) + '0';
			numero = numero / 10;
		}
		while (dd >= 0){
			printchar(buffer[dd--], colore);
		}
	}
}

void printhex (int numero, char colore){
	//int numero = *(int *)p_numero;
	char buffer[10];
	int dd = 0;
	if (numero == 0){
		buffer[dd++] = '0';
		printchar('0', colore);
	}else{
		if (numero < 0){
			printchar('-', colore);
			numero = -numero;
		}
		while (numero != 0){
			buffer[dd++] = (numero % 16) + '0';
			numero = numero / 16;
		}
		while (dd >= 0){
			switch (buffer[dd]){
				case 0x3a:
					printchar('a', colore);
					dd--;
					break;
				case 0x3b:
					printchar('b', colore);
					dd--;
					break;
				case 0x3c:
					printchar('c', colore);
					dd--;
					break;
				case 0x3d:
					printchar('d', colore);
					dd--;
					break;
				case 0x3e:
					printchar('e', colore);
					dd--;
					break;
				case 0x3f:
					printchar('f', colore);
					dd--;
					break;
				default:
					printchar(buffer[dd--], colore);
					break;
			}
		}
	}
}

void printindir (void *indirizzo, char colore){
	int numero = *(int *)indirizzo;
	char buffer[10];
	int dd = 0;
	while (numero != 0){
		buffer[dd++] = (numero % 16) + '0';
		numero = numero / 16;
	}
	print("(0x", colore);
	while (dd >= 0){
		switch (buffer[dd]){
			case 0x3a:
				printchar('a', colore);
				dd--;
				break;
			case 0x3b:
				printchar('b', colore);
				dd--;
				break;
			case 0x3c:
				printchar('c', colore);
				dd--;
				break;
			case 0x3d:
				printchar('d', colore);
				dd--;
				break;
			case 0x3e:
				printchar('e', colore);
				dd--;
				break;
			case 0x3f:
				printchar('f', colore);
				dd--;
				break;
			default:
				printchar(buffer[dd--], colore);
				break;
		}
	}
	print(")", colore);
}

void clear (){
	CarattereColore *video_memory = (CarattereColore *)VIDEO_MEMORY;
	for (int dd = 0; dd < (CARATTERI_RIGA * RIGHE); dd++){
		video_memory[dd] = (CarattereColore){0, COLORE_NERO_NERO};
	}
	cc = 0;
}

unsigned char lettura_porta (unsigned short porta){
	char val_ret;
	__asm__ ("in %%dx, %%al" : "=a" (val_ret) : "d" (porta));
	return val_ret;
}


typedef struct {
    char codice;
    char carattere;
    char carattere_shift;
} mappa;

mappa tabella_mappatura[] = {
    {0x02, '1', '!'}, {0x03, '2', '@'}, {0x04, '3', '#'}, {0x05, '4', '$'}, {0x06, '5', '%'}, {0x07, '6', '^'},
    {0x08, '7', '&'}, {0x09, '8', '*'}, {0x0A, '9', '('}, {0x0B, '0', ')'}, {0x0C, '-', '_'}, {0x0D, '=', '+'}, 
    {0x0E, '\b', '\b'}, {0x0F, '\t', '\t'},
    {0x10, 'q', 'Q'}, {0x11, 'w', 'W'}, {0x12, 'e', 'E'}, {0x13, 'r', 'R'}, {0x14, 't', 'T'}, {0x15, 'y', 'Y'},
    {0x16, 'u', 'U'}, {0x17, 'i', 'I'}, {0x18, 'o', 'O'}, {0x19, 'p', 'P'}, {0x1A, '[', '{'}, {0x1B, ']', '}'},
    {0x1C, '\n', '\n'},
    {0x1E, 'a', 'A'}, {0x1F, 's', 'S'}, {0x20, 'd', 'D'}, {0x21, 'f', 'F'}, {0x22, 'g', 'G'}, {0x23, 'h', 'H'},
    {0x24, 'j', 'J'}, {0x25, 'k', 'K'}, {0x26, 'l', 'L'}, {0x27, ';', ':'}, {0x28, '\'', '\"'}, {0x2B, '\\', '|'},
    {0x2C, 'z', 'Z'}, {0x2D, 'x', 'X'}, {0x2E, 'c', 'C'}, {0x2F, 'v', 'V'}, {0x30, 'b', 'B'}, {0x31, 'n', 'N'},
    {0x32, 'm', 'M'}, {0x33, ',', '<'}, {0x34, '.', '>'}, {0x35, '/', '?'}, {0x39, ' ', ' '}
};

void input(char buffer[], char interrup, int buffer_size, char colore) {
    int dd = 0;
    int shift = 0;
    while (dd < (buffer_size - 1)) {
        while (!(lettura_porta(0x64) & 0x01));
        char scancode = lettura_porta(0x60);

        if (scancode == 0x2A || scancode == 0x36) { // Shift premuto
            shift = 1;
            continue;
        }
        if (scancode == 0xAA || scancode == 0xB6) { // Shift rilasciato
            shift = 0;
            continue;
        }

        if (scancode == interrup) {
	    buffer[dd] = '\0';
            break;
        }

	for (int ee = 0; ee < sizeof(tabella_mappatura) / sizeof(mappa); ee++) {
            if (scancode == tabella_mappatura[ee].codice) {
                char carattere;
                if (shift) {
                    carattere = tabella_mappatura[ee].carattere_shift;
                } else {
                    carattere = tabella_mappatura[ee].carattere;
                }
                buffer[dd++] = carattere;
                if (carattere == '\n'){
                	acapo();
                }
		if (carattere == '\b'){
			cancella_char();
			cancella_cursore();
			cursore();
			buffer[dd-=2] == 0x00;
		}
		if (carattere != '\b'){
			printchar(carattere, colore);
		}
                break;
            }
        }
        shift = 0;
    }
    buffer[dd] = '\0'; 
}

void panic (char *buffer){
	clear();
	print("KERNEL PANIC: ", COLORE_ROSSOC_ROSSO);
	print(buffer, COLORE_ROSSOC_ROSSO);
	while (1){
		__asm__("hlt");
	}
}
