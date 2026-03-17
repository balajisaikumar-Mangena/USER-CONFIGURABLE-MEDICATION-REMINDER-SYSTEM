#include <lpc21xx.h>
#include "delay.h"

#define DATA (0xFF<<8)
#define RS (1<<5)
#define RW (1<<6)
#define EN (1<<7)

void cmd_LCD(unsigned char cmd){
	IOCLR0=RS;
	IOCLR0=RW;
	
	IOCLR0=DATA;
	IOSET0=(cmd<<8)& DATA;
	
	IOSET0=EN;
	delay_ms(1);
	IOCLR0=EN;
	delay_ms(1);
}

void char_LCD(unsigned char ch){
	IOSET0=RS;
	IOCLR0=RW;
	
	IOCLR0=DATA;
	IOSET0=(ch<<8) & DATA;
	
	IOSET0=EN;
	delay_ms(1);
	IOCLR0=EN;
	delay_ms(1);
}

void init_LCD(void){
	IODIR0|=DATA | RS | RW | EN;
	
	delay_ms(15);
	cmd_LCD(0x30);
	delay_ms(5);
	cmd_LCD(0x30);
	delay_us(100);
	cmd_LCD(0x30);
	cmd_LCD(0x38);         // 8 bit, 2 line
	cmd_LCD(0x0C);         // Display ON
	
	cmd_LCD(0x01);         // Clear LCD
	cmd_LCD(0x06);         // Entry Mode
}

void str_LCD(char *str){
	while(*str){
		char_LCD(*str++);
	}
}
