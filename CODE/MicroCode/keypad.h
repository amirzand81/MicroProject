

#ifndef KEYPAD_H_
#define KEYPAD_H_

struct bits {
	uint8_t b0:1;
	uint8_t b1:1;
	uint8_t b2:1;
	uint8_t b3:1;
	uint8_t b4:1;
	uint8_t b5:1;
	uint8_t b6:1;
	uint8_t b7:1;
} __attribute__((__packed__));


#define SBIT(port,pin) ((*(volatile struct bits*)&port).b##pin)
#define R1 SBIT(PORTC, 0)
#define R2 SBIT(PORTC, 1)
#define R3 SBIT(PORTC, 2)
#define R4 SBIT(PORTC, 3)
#define c1 SBIT(PINC, 4)
#define c2 SBIT(PINC, 5)
#define c3 SBIT(PINC, 6)
#define c4 SBIT(PINC, 7)

unsigned char keypress(void)
{
	DDRC=0b00001111;
	PORTC=0b11110000;

	R1=0;     R2=1;     R3=1;     R4=1;
	_delay_us(50);
	if (c1==0){while (c1==0);return ('7');}
	if(c2==0){while (c2==0); return ('8');}
	if (c3==0){while (c3==0); return ('9');}
	if(c4==0){while (c4==0);return ('/');}
	
	R1=1;     R2=0;     R3=1;     R4=1;
	_delay_us(50);
	if (c1==0){while (c1==0); return ('4');}
	if(c2==0){while (c2==0); return ('5');}
	if(c3==0){while (c3==0); return ('6');}
	if(c4==0){while (c4==0); return ('x');}
	
	R1=1;     R2=1;     R3=0;     R4=1;
	_delay_us(50);
	if (c1==0){while (c1==0); return ('1');}
	if (c2==0){while (c2==0); return ('2');}
	if (c3==0){while (c3==0); return ('3');}
	if (c4==0){while (c4==0); return ('-');}
	
	R1=1;     R2=1;     R3=1;     R4=0;
	_delay_us(50);
	if (c1==0){while (c1==0); return ('C');}
	if (c2==0){while (c2==0); return ('0');}
	if (c3==0){while (c3==0); return ('=');}
	if (c4==0){while (c4==0); return ('+');}
	
	return 0;
}

#endif /* KEYPAD_H_ */