#include <avr/io.h>
#include <avr/eeprom.h>

#define ARRAY_START_ADDRESS 0

uint8_t numbers[24] = { 
							0x1, 0x3, 0xc, 0x3, 0x6, 0xf, 0x8, 0x6, 
							0x7, 0x0, 0x5, 0x2, 0x4, 0xe, 0xd, 0xb, 
							0x8, 0x2, 0xa, 0x9, 0xb, 0x4, 0x1, 0x0
					  };

unsigned char lives = 6;
unsigned char wins = 0;
unsigned char losts = 0;
int point = 0;
unsigned char r = 0;
unsigned char pattern[4];
char dotControl[4]={1,2,4,8};
unsigned char patterns[6][4];

void clearPattern()
{
	for (int i=0;i<8;i++)
	{
		PORTA=0x00;
		PORTD=0xFF;
		PORTA=dotControl[i];
		_delay_ms(2);
	}
}

void fill()
{
	for (int i = 0; i < 4; i++)
	{
		PORTA = 0x00;
		PORTD = patterns[r][i];
		PORTA = dotControl[i];
		_delay_ms(2);
	}
}

void fill2()
{
	for (int i = 0; i < 4; i++)
	{
		PORTA = 0x00;
		PORTD = pattern[i];
		PORTA = dotControl[i];
		_delay_ms(2);
	}
}

void getPatterns()
{	
	if(eeprom_read_byte((uint8_t*)24) != 255)
	{
		wins = eeprom_read_byte((uint8_t*)24);
		losts = eeprom_read_byte((uint8_t*)25);
		point = eeprom_read_byte((uint8_t*)26);
		
		for (int i = 0; i < 6; i++)
			for (int j = 0; j < 4; j++)
				patterns[i][j] = eeprom_read_byte((uint8_t*)(i*4 + j));
	}
	else
	{
		for (int i = 0; i < 6; i++)
			for (int j = 0; j < 4; j++)
			{
				patterns[i][j] = numbers[i*4 + j];
				eeprom_write_byte((uint8_t*)i, numbers[i]);
			}
	}
}


void cyclically_shift() {
	int temp;
	int shift = 13;
	
	for (int i = 0; i < 24; i++)
		numbers[i] = eeprom_read_byte((uint8_t*)i);
	
	while (shift > 0) {
		temp = numbers[23];
		
		for (int i = 23; i > 0; i--) {
			numbers[i] = numbers[i - 1];
		}
		
		numbers[0] = temp;
		shift--;
	}
	
	for(int i = 0; i < 24; i++)
		eeprom_write_byte((uint8_t*)i, numbers[i]);
}
 
 void mydelay()
 {
	 for(int i = 0; i < 200; i++)
	 {
		 fill2();
		 _delay_us(10);
	 }
 }