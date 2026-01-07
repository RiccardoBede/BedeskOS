unsigned long int disco_size();
unsigned long int scrivi_settore(char *, char *, unsigned long int); //nome, buffer, numero_settore
unsigned long int leggi_settore(char *, int, unsigned long int); //buffer, size_buffer, numero_settore
unsigned long int cerca_file(char *, int); //nome, size_nome
unsigned long int trova_settore_libero();
unsigned long int modifica_settore(char *, unsigned long int); //buffer, numero_settore

void editor (){
	print(" nome=", COLORE_BIANCO_NERO);
	char nome_file[16];
	input(nome_file, 0x1c, sizeof(nome_file), COLORE_BIANCO_NERO);
	int dd = 0;
	while (nome_file[dd] != '\0'){	dd++;}
	clear();
	print(nome_file, COLORE_BIANCO_NERO);
	print("\n", COLORE_BIANCO_NERO);

	//trova_settore_libero();	
	unsigned long int settore_file = cerca_file(nome_file, dd);
	if (settore_file != 0){
		char scelta[2];		
		print("FILE ESISTENTE\n", COLORE_GIALLO_NERO);
		while (1){
			print("[M]modifica [E]elimina ?", COLORE_GIALLO_NERO);
			input(scelta, 0x1c, sizeof(scelta), COLORE_BIANCO_NERO);
			print("\n", COLORE_BIANCO_NERO);

			if (scelta[0] == 'M' || scelta[0] == 'm'){

				break;
			}
			if (scelta[0] == 'E' || scelta[0] == 'e'){
				//for(;;) con leggi (perche trova la firma 0xaa) e cancella
				for(;;){
					char buffer[512];
					unsigned long int settore_da_cancellare = leggi_settore(buffer, sizeof(buffer), settore_file);
					if (settore_da_cancellare != 0){
						formatta_settore(settore_file);
						formatta_settore(settore_da_cancellare);
					}else{	break;}
				}
				break;
			}
			if (scelta[0] != 'M' || scelta[0] != 'm' || scelta[0] != 'E' || scelta[0] != 'e'){
				continue;
			}
		}
	}else{
		print("CREAZIONE FILE\n", COLORE_GIALLO_NERO);
		int ee = 0;
		while (1){
			unsigned long int settore_file = trova_settore_libero();
			
			if (settore_file != 0){
				bool fine = false;
				char buffer[486];
				input(buffer, 0x01, sizeof(buffer), COLORE_BIANCO_NERO);

				for (int dd = 0; dd < sizeof(buffer); dd++){
					if (buffer[dd] == 0x01){	fine = true;}
				}
				
				scrivi_settore(nome_file, buffer, settore_file);
				
				if (fine){	break;}
				
				if((((80 * 25) - 16)) / (ee * sizeof(buffer)) < 1){
					clear();
				}
			}
			if (settore_file == 0){
				break;
			}
		}
	}

}
