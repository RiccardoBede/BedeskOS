//TIMER
//

#define FREQUENZA_PIT 1193182

static unsigned long int tick = 0;

void timer (unsigned long int giri){
	//unsigned long int frequenza = FREQUENZA_PIT / millisecondi;
	unsigned long int dd = 0;
	outb(0x36, 0x43);
	outb(255 & 0xff, 0x40);
	outb((255 >> 8) & 0xff, 0x40);
	
	outb(0x00, 0x43);
	while (dd != (giri * 5 * potenza(10, 3))){
		if (inb(0x40) == 122){	dd++;}
	}
}

