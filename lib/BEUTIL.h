//TOOLS GENERALI
//

//MATEMATICA (POS IN LIB PROVVISORIA)
int potenza(int base, int esponente){
	int risul = 1;
	for (int dd = 0; dd < esponente; dd++){
		risul *= base;
	}
	return risul;
}

typedef struct {
	int base;
	int esponente;
}notazione_scientifica;

void numeri_oltre_lim_ns (int base, int esponente, int array[], int array_size){
	array[0] = base;
	if (esponente <= 0){ return;}
	int numero_ripetizioni_esp = esponente / (array_size - 1);
	int dd = 1;
	while (dd <= numero_ripetizioni_esp){
		array[dd] = array_size;
		dd++;
	}
	if ((esponente % (array_size - 1)) > 0){
		array[esponente / array_size + 1] += (esponente % (array_size - 1));
	}
}

//QUANTE CIFRE CORRISPONEDE IL NUMERO INSERITO IN BINARIO
int cifre_in_bin (unsigned long int numero){
	unsigned long int num_cif = 0;
	while (numero > 0){
		numero /= 2;
		num_cif++;
	}
	return num_cif;
}

int num_cifre_numero (unsigned long int numero){
	int num_cifre = 0;
	while (numero > 0){
		numero /= 10;
		num_cifre++;
	}
	return num_cifre;
}

//DA STRINGA ESADECIMALE A INTERO

int stringa_int(char *buffer, int size_buffer){
	int da_ritorno = 0;
	int num_cifre = 0;
	while(num_cifre < size_buffer && buffer[num_cifre] != '\0'){
		num_cifre++;
	}
	for (int dd = 0; dd < num_cifre; dd++){
		da_ritorno *= 10;
		if (buffer[dd] >= '0' && buffer[dd] <= '9'){
			da_ritorno += buffer[dd] - '0';
		}
	}
	return da_ritorno;
}

void array_char_int (char array[], int array_size){
	for (int dd = 0; dd < array_size; dd++){
		array[dd] -= '0';
	}
}

int stringa_hex(char *buffer, int size_buffer) { 
	int da_ritorno = 0;
	int num_cifre = 0; 
	while (num_cifre < size_buffer && buffer[num_cifre] != '\0'){
		num_cifre++; 
	} 
	for (int dd = 0; dd < num_cifre; dd++) { 
		da_ritorno *= 16;
		if (buffer[dd] >= '0' && buffer[dd] <= '9') { 
			da_ritorno += buffer[dd] - '0'; 
		} if (buffer[dd] >= 'a' && buffer[dd] <= 'f') { 
			da_ritorno += buffer[dd] - 'a' + 10; 
		}
	} 
	return da_ritorno; 
}

unsigned short stringa_unsigned_short(char *buffer, int size_buffer){
	int da_ritorno = 0;
	int num_cifre = 0; 
	while (num_cifre < size_buffer && buffer[num_cifre] != '\0'){
		num_cifre++; 
	} 
	for (int dd = 0; dd < num_cifre; dd++) { 
		da_ritorno *= 16;
		if (buffer[dd] >= '0' && buffer[dd] <= '9') { 
			da_ritorno += buffer[dd] - '0'; 
		} if (buffer[dd] >= 'a' && buffer[dd] <= 'f') { 
			da_ritorno += buffer[dd] - 'a' + 10; 
		}
	} 
	return da_ritorno; 
}

typedef struct {
	char carattere;
	int carattere_dec;
	char carattere_hex;
}Tabella_Ascii;

Tabella_Ascii tabella_ascii[] = {
    {' ', 32, 0x20}, {'!', 33, 0x21}, {'"', 34, 0x22}, {'#', 35, 0x23},
    {'$', 36, 0x24}, {'%', 37, 0x25}, {'&', 38, 0x26}, {'\'', 39, 0x27},
    {'(', 40, 0x28}, {')', 41, 0x29}, {'*', 42, 0x2A}, {'+', 43, 0x2B},
    {',', 44, 0x2C}, {'-', 45, 0x2D}, {'.', 46, 0x2E}, {'/', 47, 0x2F},
    {'0', 48, 0x30}, {'1', 49, 0x31}, {'2', 50, 0x32}, {'3', 51, 0x33},
    {'4', 52, 0x34}, {'5', 53, 0x35}, {'6', 54, 0x36}, {'7', 55, 0x37},
    {'8', 56, 0x38}, {'9', 57, 0x39}, {':', 58, 0x3A}, {';', 59, 0x3B},
    {'<', 60, 0x3C}, {'=', 61, 0x3D}, {'>', 62, 0x3E}, {'?', 63, 0x3F},
    {'@', 64, 0x40}, {'A', 65, 0x41}, {'B', 66, 0x42}, {'C', 67, 0x43},
    {'D', 68, 0x44}, {'E', 69, 0x45}, {'F', 70, 0x46}, {'G', 71, 0x47},
    {'H', 72, 0x48}, {'I', 73, 0x49}, {'J', 74, 0x4A}, {'K', 75, 0x4B},
    {'L', 76, 0x4C}, {'M', 77, 0x4D}, {'N', 78, 0x4E}, {'O', 79, 0x4F},
    {'P', 80, 0x50}, {'Q', 81,0x51}, {'R', 82, 0x52}, {'S', 83, 0x53},
    {'T', 84, 0x54}, {'U', 85, 0x55}, {'V', 86, 0x56}, {'W', 87, 0x57},
    {'X', 88, 0x58}, {'Y', 89, 0x59}, {'Z', 90, 0x5A}, {'[', 91, 0x5B},
    {'\\', 92, 0x5C}, {']', 93, 0x5D}, {'^', 94, 0x5E}, {'_', 95, 0x5F},
    {'`', 96, 0x60}, {'a', 97, 0x61}, {'b', 98, 0x62}, {'c', 99, 0x63},
    {'d', 100, 0x64}, {'e', 101, 0x65}, {'f', 102, 0x66}, {'g', 103, 0x67},
    {'h', 104, 0x68}, {'i', 105, 0x69}, {'j', 106, 0x6A}, {'k', 107, 0x6B},
    {'l', 108, 0x6C}, {'m', 109, 0x6D}, {'n', 110, 0x6E}, {'o', 111, 0x6F},
    {'p', 112, 0x70}, {'q', 113, 0x71}, {'r', 114, 0x72}, {'s', 115, 0x73},
    {'t', 116, 0x74}, {'u', 117, 0x75}, {'v', 118, 0x76}, {'w', 119, 0x77},
    {'x', 120, 0x78}, {'y', 121, 0x79}, {'z', 122, 0x7A}, {'{', 123, 0x7B},
    {'|', 124, 0x7C}, {'}', 125, 0x7D}, {'~', 126, 0x7E}};

int stringa_ascii_dec (char *buffer){
	int ee = 0;
	for (int dd = 0; dd < 94; dd++){
		if (buffer[ee] == tabella_ascii[dd].carattere){
			ee++;
			return tabella_ascii[dd].carattere_dec;
		}
	}
}
