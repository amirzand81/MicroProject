#define F_CPU 16000000UL			
#include <avr/io.h>			
#include <util/delay.h>			
#include <stdlib.h>
#include "lcd_funcs.h"
#include "keypad.h"
#include "string.h"
#include "stdio.h"
#include "matrix.h"

unsigned char tmp = 0, tmp2 = 0, tmp3 = 0, tmp4 = 0;
char key = 0;
char str[21];
EEMEM int A;

int main()
{
	LCD_Init();
	getPatterns();
	cyclically_shift();
	
	LCD_String_xy(0,5, "Welcome to");
	LCD_String_xy(1,3, "Pattern Guess!");
	
	memset(str,0,sizeof(str[0])*21);
	DDRD = 0xFF;
	DDRA = 0xFF;
	
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 4; j++)
			patterns[i][j] = ~patterns[i][j] | 0xf0 ;

	_delay_ms(3000);
	
	while(1)
	{
		LCD_Clear();
		LCD_String("1->New game");
		LCD_Command(0xC0);
		LCD_String("2->Information");
	
		while(key != 49 && key != 50)
		{
			key = keypress();
		}
	
		if(key == 49)
		{
			unsigned char isLost = 0;
			
			for (int j = 0; j < 4; j++)
				pattern[j] = 0xff;
		
			LCD_Clear();
			LCD_String_xy(0,4, "This pattern");
			LCD_String_xy(1,0, "hide after 5 seconds");
			
			for (int j = 0; j < 450; j++)
				fill();
		
			clearPattern();
			LCD_Clear();

			while(!isLost)
			{
				fill2();
				
				sprintf(str,"Lives : %d",lives);
				LCD_String_xy(0, 0, str);
				
				sprintf(str,"Point : %d",point);
				LCD_String_xy(1, 0, str);
				
				fill2();

				key = keypress();
				if(key == 0) continue;
								
				switch(key)
				{
					case 'C':
						tmp = 3;
						tmp2 = 8;
						break;
						
					case '0':
						tmp = 3;
						tmp2 = 4;
						break;

					case '=':
						tmp = 3;
						tmp2 = 2;
						break;

					case '+':
						tmp = 3;
						tmp2 = 1;
						break;

					case '1':
						tmp = 2;
						tmp2 = 8;
						break;

					case '2':
						tmp = 2;
						tmp2 = 4;
						break;

					case '3':
						tmp = 2;
						tmp2 = 2;
						break;

					case '-':
						tmp = 2;
						tmp2 = 1;
						break;

					case '4':
						tmp = 1;
						tmp2 = 8;
						break;

					case '5':
						tmp = 1;
						tmp2 = 4;
						break;

					case '6':
						tmp = 1;
						tmp2 = 2;
						break;

					case 'x':
						tmp = 1;
						tmp2 = 1;
						break;

					case '7':
						tmp = 0;
						tmp2 = 8;
						break;

					case '8':
						tmp = 0;
						tmp2 = 4;
						break;

					case '9':
						tmp = 0;
						tmp2 = 2;
						break;

					case '/':
						tmp = 0;
						tmp2 = 1;
						break;
				}
				
				tmp3 = patterns[r][tmp] & tmp2;
				tmp4 = pattern[tmp] & tmp2;
				
				fill2();

				// repeated coordination
				if( tmp3 == 0 && tmp4 == 0)
				{
					continue;
				}
				
				// wrong coordination
				if( tmp3 && tmp4 )
				{
					DDRB |= 8;
					PORTB |= 8;
					mydelay();
					PORTB &= 0xF7;
					
					point -= 1;
					lives--;
					if(lives == 255)
					{
						losts++;
						isLost++;
						continue;
					}
				}
				
				// correct coordination
				else
				{
					point += 3;
					pattern[tmp] &= ~tmp2;
					
					DDRB |= 4;
					PORTB |= 4;
					mydelay();
					PORTB &= 0xFB;
				}
				
				fill2();
				unsigned char isWin = 1;
				
				for(unsigned char i = 0; i < 4; i++)
					 if(pattern[i] != patterns[r][i])
						 isWin = 0;

				if(isWin)
				{
					for (int i = 0; i < 4; i++)
					{
						pattern[i] = 0xFF;
					}
					
					r++;
					
					if(r == 6)
					{
						wins++;
						
						LCD_Clear();
						LCD_String_xy(0, 6, "You  Won");
						LCD_String_xy(1, 7, "Ha  Ha");

						eeprom_write_byte((uint8_t*)24, wins);
						eeprom_write_byte((uint8_t*)25, losts);
						eeprom_write_byte((uint8_t*)26, point);
						
						while(1);
					}
					
					LCD_Clear();
					LCD_String_xy(0,4, "new pattern!");
					LCD_String_xy(1,0, "hide after 5 seconds");
					
					for (int j = 0; j < 450; j++)
						fill();
				}
					
				LCD_Clear();
			}
			
			LCD_Clear();
			LCD_String_xy(0, 5, "Game  over");
			LCD_String_xy(1, 6, "Bye  Bye");
			
			break;
		}
	
		else if (key == 50)
		{
			sprintf(str,"Win:%d, Loss:%d", wins, losts);
			LCD_Clear();
			LCD_String_xy(0, 0, str);
			LCD_String_xy(1, 0, "Press ON/C to back");
		
			while(key != 'C')
			{
				key = keypress();
			}
		}
	}

	eeprom_write_byte((uint8_t*)24, wins);
	eeprom_write_byte((uint8_t*)25, losts);
	eeprom_write_byte((uint8_t*)26, point);

	while(1);
}