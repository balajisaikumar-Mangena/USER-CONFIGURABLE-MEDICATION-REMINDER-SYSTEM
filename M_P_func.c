#include <lpc21xx.h>
#include "delay.h"
#include "lcd.h"
#include "KPM.h"
#include "RTC.h"
#include "EINT.h"
#include <cstdlib>

#define BUZZ (1<<16)

extern unsigned int flag;
unsigned int med4=0;
unsigned int med5=0;
unsigned int cnt=0;

int hour,min,sec;
int date,month,year;
int day;

unsigned int M_H1 = 06;
unsigned int M_min1 = 30;

unsigned int M_H2 = 12;
unsigned int M_min2 = 30;

unsigned int M_H3 = 20;
unsigned int M_min3 = 30;

unsigned int M_H4;
unsigned int M_min4;

unsigned int M_H5;
unsigned int M_min5;

void display_LCD(unsigned int num);

/* CGRAM Initialisation and Display Arrows */

void LCD_Control_UI()
{
    unsigned char i;

    unsigned char arrow[4][8] =
    {
        {0x04,0x08,0x1F,0x08,0x04,0,0,0},       
        {0x04,0x02,0x1F,0x02,0x04,0,0,0},       
        {0x04,0x0E,0x15,0x04,0x04,0x04,0x04,0},
        {0x04,0x04,0x04,0x04,0x15,0x0E,0x04,0} 
    };

    cmd_LCD(0x40);   

    for(i=0;i<32;i++) char_LCD(((unsigned char*)arrow)[i]);

    cmd_LCD(0x01);

    cmd_LCD(0x80);
    str_LCD("4 "); char_LCD(0);
    str_LCD(" 6 "); char_LCD(1);
    str_LCD("  2 "); char_LCD(2);
    str_LCD(" 8 "); char_LCD(3);

    cmd_LCD(0xC0);
    str_LCD("= OK/SAVE C EXIT");

    delay_ms(1000);
    cmd_LCD(0x01);
}

void Arrow_Init()
{
    cmd_LCD(0x40);                                                             // CGRAM start
	
    char_LCD(0x04); char_LCD(0x06); char_LCD(0x1F); char_LCD(0x06);            // Left Arrow
    char_LCD(0x04); char_LCD(0x00); char_LCD(0x00); char_LCD(0x00);

    char_LCD(0x04); char_LCD(0x0C); char_LCD(0x1F); char_LCD(0x0C);            // Right Arrow
    char_LCD(0x04); char_LCD(0x00); char_LCD(0x00); char_LCD(0x00);
}

void Arrow_Init1()
{
    cmd_LCD(0x40);  																												   // CGRAM start

    char_LCD(0x04); char_LCD(0x0E); char_LCD(0x15); char_LCD(0x04); 					 // Up Arrow
    char_LCD(0x04); char_LCD(0x04); char_LCD(0x00); char_LCD(0x00);

    char_LCD(0x04); char_LCD(0x04); char_LCD(0x04); char_LCD(0x04);            // Down Arrow
    char_LCD(0x15); char_LCD(0x0E); char_LCD(0x04); char_LCD(0x00);
}

void Arrow_UI()
{
		Arrow_Init1();
    cmd_LCD(0xC0);

    char_LCD(0);      
    str_LCD("    ");

    char_LCD(1);
    str_LCD("    ");

    str_LCD("OK");
}

//----------Display MEDICINES TIMES-------------

void DisplayMED1(){
	cmd_LCD(0xc0);
	str_LCD("    ");
	display_LCD(M_H1);
	char_LCD(':');
	display_LCD(M_min1);
	char_LCD(':');
	str_LCD("00    ");
}

void DisplayMED2(){
	cmd_LCD(0xc0);
	str_LCD("    ");
	display_LCD(M_H2);
	char_LCD(':');
	display_LCD(M_min2);
	char_LCD(':');
	str_LCD("00    ");
}

void DisplayMED3(){
	cmd_LCD(0xc0);
	str_LCD("    ");
	display_LCD(M_H3);
	char_LCD(':');
	display_LCD(M_min3);
	char_LCD(':');
	str_LCD("00    ");
}

void DisplayMED4(){
	cmd_LCD(0xc0);
	str_LCD("    ");
	display_LCD(M_H4);
	char_LCD(':');
	display_LCD(M_min4);
	char_LCD(':');
	str_LCD("00    ");
}

void DisplayMED5(){
	cmd_LCD(0xc0);
	str_LCD("    ");
	display_LCD(M_H5);
	char_LCD(':');
	display_LCD(M_min5);
	char_LCD(':');
	str_LCD("00    ");
}

//------------Display Project Menu-------------

void MENU(){
	cmd_LCD(0x01);
	str_LCD(" * EDIT MODE * ");
	cmd_LCD(0xc0);
	str_LCD(" RTC  MED  EXIT ");
	delay_s(1);
}

void MRS(){
	cmd_LCD(0x01);
	str_LCD("MEDICICATION SYS");
	cmd_LCD(0xc0);
	str_LCD("REMINDER   READY");
	delay_s(1);
	cmd_LCD(0x01);
	str_LCD("  PRESS 5  FOR  ");
	cmd_LCD(0xc0);
	str_LCD("  CONTROL KEYS  ");
	delay_s(1);
	cmd_LCD(0x01);
}

void display_LCD(unsigned int num){                                         // Display Number
    char arr[5];

    arr[0] = (num/10) + '0';
    arr[1] = (num%10) + '0';
    arr[2] = '\0';

    str_LCD(arr);
}

void Increment_Value(unsigned int *value, unsigned int max){                // Increment
    (*value)++;
    if(*value > max) *value = 0;
}

void Decrement_Value(unsigned int *value, unsigned int max){                // Decrement
    if(*value == 0) *value = max;
    else (*value)--;
}

// ---------------- HOUR EDIT ----------------

void Hour(){
    unsigned char key;
    unsigned int temp = HOUR;

    cmd_LCD(0x01);
		str_LCD("SET HOUR :      ");
    while(1)
    {
				if(flag) return;
        cmd_LCD(0x80);
				str_LCD("SET HOUR : ");
        display_LCD(temp);
				cmd_LCD(0xc0);
				Arrow_UI();

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();
 
        if(key=='2') Increment_Value(&temp,23);
        if(key=='8') Decrement_Value(&temp,23);
			
				if(key=='C'){ flag=1; if(flag ||(key=='C')) return; }
        if(key=='='){ HOUR = temp; cmd_LCD(0x01); return; }

        delay_ms(150);
    }
}

// ---------------- MIN EDIT ----------------

void Min(){
    unsigned char key;
    unsigned int temp = MIN;

    cmd_LCD(0x01);
		str_LCD("SET MIN :       ");
    while(1)
    {
				if(flag) return;
        cmd_LCD(0x80);
				str_LCD("SET MIN : ");
        display_LCD(temp);
				cmd_LCD(0xc0);
				Arrow_UI();

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='2') Increment_Value(&temp,59);
        if(key=='8') Decrement_Value(&temp,59);
		
				if(key=='C'){ flag=1; if(flag ||(key=='C')) return; }
        if(key=='='){ MIN = temp; cmd_LCD(0x01); return; }

        delay_ms(150);
    }
}

// ---------------- SEC EDIT ----------------

void Sec(){
    unsigned char key;
    unsigned int temp = SEC;

    cmd_LCD(0x01);
		str_LCD("SET SEC :       ");
    while(1)
    {
				if(flag) return;
        cmd_LCD(0x80);
				str_LCD("SET SEC : ");
        display_LCD(temp);
				cmd_LCD(0xc0);
				Arrow_UI();

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='2') Increment_Value(&temp,59);
        if(key=='8') Decrement_Value(&temp,59);

				if(key=='C'){ flag=1; if(flag ||(key=='C')) return; }
        if(key=='='){ SEC = temp; cmd_LCD(0x01); return; }

        delay_ms(150);
    }
}

// ---------------- DATE EDIT ----------------

void Day_OM(){
    unsigned char key;
    unsigned int temp = DOM;

    cmd_LCD(0x01);
		str_LCD("SET DOM :       ");
    while(1)
    {
				if(flag) return;
        cmd_LCD(0x80);
				str_LCD("SET DOM : ");
        display_LCD(temp);
				cmd_LCD(0xc0);
				Arrow_UI();

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='2') Increment_Value(&temp,31);
        if(key=='8') Decrement_Value(&temp,31);
	
				if(key=='C'){ flag=1; if(flag ||(key=='C')) return; }
        if(key=='='){ DOM = temp; cmd_LCD(0x01); return; }

        delay_ms(150);
    }
}

// ---------------- MONTH EDIT ----------------

void Month(){
    unsigned char key;
    unsigned int temp = MONTH;

    cmd_LCD(0x01);
		str_LCD("SET MONTH :     ");
    while(1)
    {
				if(flag) return;
        cmd_LCD(0x80);
				str_LCD("SET MONTH : ");
        display_LCD(temp);
				cmd_LCD(0xc0);
				Arrow_UI();

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='2') Increment_Value(&temp,12);
        if(key=='8') Decrement_Value(&temp,12);

				if(key=='C'){ flag=1;	if(flag ||(key=='C')) return;	}
				if(key=='=') { MONTH = temp; cmd_LCD(0x01); return; }

        delay_ms(150);
    }
}

// ---------------- YEAR EDIT ----------------

void Year(){
    unsigned char key;
    unsigned int temp = YEAR;

    cmd_LCD(0x01);
		str_LCD("SET YEAR :      ");
    while(1)
    {
				if(flag) return;
        cmd_LCD(0x80);
				str_LCD("SET YEAR : ");
				
        char_LCD((temp/1000)%10 + 48);
				char_LCD((temp/100)%10 + 48);
				char_LCD((temp/10)%10 + 48);
				char_LCD((temp%10) + 48);
			
				cmd_LCD(0xc0);
				Arrow_UI();

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='2') Increment_Value(&temp,2099);
        if(key=='8') Decrement_Value(&temp,2000);
	
				if(key=='C'){ flag=1; if(flag ||(key=='C')) return; }
        if(key=='='){ YEAR = temp; cmd_LCD(0x01); return; }

        delay_ms(150);
    }
}

// ---------------- DAY EDIT ----------------

void Day(){
    unsigned char key;
    unsigned int temp = DOW;

    cmd_LCD(0x01);
		str_LCD("SET DAY :       ");
    while(1)
    {
				if(flag) return;
        cmd_LCD(0x80);
				str_LCD("SET DAY : ");
			
				if(temp == 00) str_LCD("SUN");
				if(temp == 01) str_LCD("MON");
				if(temp == 02) str_LCD("TUE");
				if(temp == 03) str_LCD("WED");
				if(temp == 04) str_LCD("THU");
				if(temp == 05) str_LCD("FRI");
				if(temp == 06) str_LCD("SAT");
			
				cmd_LCD(0xc0);
				Arrow_UI();
			
        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='2') Increment_Value(&temp,6);
        if(key=='8') Decrement_Value(&temp,6);
				
				if(key=='C'){ flag=1; if(flag ||(key=='C')) return; }
        if(key=='='){ DOW = temp; cmd_LCD(0x01); return; }

        delay_ms(150);
    }
}

// -------- MEDICINE 1 TIME EDIT --------

void Med1_Hour(){
    unsigned char key;
    unsigned int temp = M_H1;

    cmd_LCD(0x01);
		str_LCD("SET HOUR :      ");
    while(1)
    {
				if(flag) return;
        cmd_LCD(0x80);
				str_LCD("SET HOUR : ");
        display_LCD(temp);
				cmd_LCD(0xc0);
				Arrow_UI();

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='2') Increment_Value(&temp,23);
        if(key=='8') Decrement_Value(&temp,23);
				
				if(key=='C'){ flag=1; if(flag ||(key=='C')) return; }
        if(key=='='){ M_H1 = temp; cmd_LCD(0x01); return; }

        delay_ms(150);
    }
}

void Med1_Min(){
    unsigned char key;
    unsigned int temp = M_min1;

    cmd_LCD(0x01);
		str_LCD("SET MIN :       ");
    while(1)
    {
				if(flag) return;
        cmd_LCD(0x80);
				str_LCD("SET MIN : ");
        display_LCD(temp);
				cmd_LCD(0xc0);
				Arrow_UI();

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='2') Increment_Value(&temp,59);
        if(key=='8') Decrement_Value(&temp,59);
				
				if(key=='C'){ flag=1; if(flag ||(key=='C')) return; }
        if(key=='='){ M_min1 = temp; cmd_LCD(0x01); return; }

        delay_ms(150);
    }
}

// -------- MEDICINE 2 TIME EDIT --------

void Med2_Hour(){
    unsigned char key;
    unsigned int temp = M_H2;

    cmd_LCD(0x01);
		str_LCD("SET HOUR :      ");
    while(1)
    {
				if(flag) return;
        cmd_LCD(0x80);
				str_LCD("SET HOUR : ");
        display_LCD(temp);
				cmd_LCD(0xc0);
				Arrow_UI();

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='2') Increment_Value(&temp,23);
        if(key=='8') Decrement_Value(&temp,23);
			
				if(key=='C'){ flag=1; if(flag ||(key=='C')) return; }
        if(key=='='){ M_H2 = temp; cmd_LCD(0x01); return; }

        delay_ms(150);
    }
}

void Med2_Min(){
    unsigned char key;
    unsigned int temp = M_min2;

    cmd_LCD(0x01);
		str_LCD("SET MIN :       ");
    while(1)
    {
			  if(flag) return;
        cmd_LCD(0x80);
				str_LCD("SET MIN : ");
        display_LCD(temp);
				cmd_LCD(0xc0);
				Arrow_UI();

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='2') Increment_Value(&temp,59);
        if(key=='8') Decrement_Value(&temp,59);
	
			
				if(key=='C'){ flag=1; if(flag ||(key=='C')) return; }
        if(key=='='){ M_min2 = temp; cmd_LCD(0x01); return; }

        delay_ms(150);
    }
}

// -------- MEDICINE 3 TIME EDIT --------

void Med3_Hour(){
    unsigned char key;
    unsigned int temp = M_H3;

    cmd_LCD(0x01);
		str_LCD("SET HOUR :      ");
    while(1)
    {
				if(flag) return;
        cmd_LCD(0x80);
				str_LCD("SET HOUR : ");
        display_LCD(temp);
				cmd_LCD(0xc0);
				Arrow_UI();

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='2') Increment_Value(&temp,23);
        if(key=='8') Decrement_Value(&temp,23);
				
				if(key=='C'){ flag=1; if(flag ||(key=='C')) return; }
        if(key=='='){ M_H3 = temp; cmd_LCD(0x01); return; }

        delay_ms(150);
    }
}

void Med3_Min(){
    unsigned char key;
    unsigned int temp = M_min3;

    cmd_LCD(0x01);
		str_LCD("SET MIN :       ");
    while(1)
    {
				if(flag) return;
        cmd_LCD(0x80);
				str_LCD("SET MIN : ");
        display_LCD(temp);
				cmd_LCD(0xc0);
				Arrow_UI();

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='2') Increment_Value(&temp,59);
        if(key=='8') Decrement_Value(&temp,59);

				if(key=='C'){ flag=1; if(flag ||(key=='C')) return; }
        if(key=='='){ M_min3 = temp; cmd_LCD(0x01); return; }

        delay_ms(150);
    }
}

// -------- MEDICINE 4 TIME EDIT --------

void Med4_Hour(){
    unsigned char key;
    unsigned int temp = M_H4;

    cmd_LCD(0x01);
		str_LCD("SET HOUR :      ");
    while(1)
    {
				if(flag) return;
        cmd_LCD(0x80);
				str_LCD("SET HOUR : ");
        display_LCD(temp);
				cmd_LCD(0xc0);
				Arrow_UI();

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='2') Increment_Value(&temp,23);
        if(key=='8') Decrement_Value(&temp,23);
				
				if(key=='C'){ flag=1; if(flag ||(key=='C')) return; }
        if(key=='='){ M_H4 = temp; cmd_LCD(0x01); return; }

        delay_ms(150);
    }
}

void Med4_Min(){
    unsigned char key;
    unsigned int temp = M_min4;

    cmd_LCD(0x01);
		str_LCD("SET MIN :       ");
    while(1)
    {
				if(flag) return;
        cmd_LCD(0x80);
				str_LCD("SET MIN : ");
        display_LCD(temp);
				cmd_LCD(0xc0);
				Arrow_UI();

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='2') Increment_Value(&temp,59);
        if(key=='8') Decrement_Value(&temp,59);
				
				if(key=='C'){ flag=1; if(flag ||(key=='C')) return; }
        if(key=='='){ M_min4 = temp; cmd_LCD(0x01); return; }

        delay_ms(150);
    }
}

// -------- MEDICINE 5 TIME EDIT --------

void Med5_Hour(){
    unsigned char key;
    unsigned int temp = M_H5;

    cmd_LCD(0x01);
		str_LCD("SET HOUR :      ");
    while(1)
    {
				if(flag) return;
        cmd_LCD(0x80);
				str_LCD("SET HOUR : ");
        display_LCD(temp);
				cmd_LCD(0xc0);
				Arrow_UI();

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='2') Increment_Value(&temp,23);
        if(key=='8') Decrement_Value(&temp,23);
				
				if(key=='C'){ flag=1; if(flag ||(key=='C')) return; }
        if(key=='='){ M_H5 = temp; cmd_LCD(0x01); return; }

        delay_ms(150);
    }
}

void Med5_Min(){
    unsigned char key;
    unsigned int temp = M_min5;

    cmd_LCD(0x01);
		str_LCD("SET MIN :       ");
    while(1)
    {
				if(flag) return;
        cmd_LCD(0x80);
				str_LCD("SET MIN : ");
        display_LCD(temp);
				cmd_LCD(0xc0);
				Arrow_UI();

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='2') Increment_Value(&temp,59);
        if(key=='8') Decrement_Value(&temp,59);
				
				if(key=='C'){ flag=1; if(flag ||(key=='C')) return; }
        if(key=='='){ M_min5 = temp; cmd_LCD(0x01); return; }

        delay_ms(150);
    }
}

// ---------------- RTC TIME MENU ----------------

void Edit_RTC_Time(){
    unsigned char key;
    unsigned int menu = 0;
		cmd_LCD(0x01);
		str_LCD("  EDIT RTC TIME ");
    while(1)
    {
				if(flag) return;
				Arrow_Init();
				cmd_LCD(0x80);
				str_LCD("  EDIT RTC TIME ");
        if(menu==0) {cmd_LCD(0xc0);str_LCD("      HOUR     ");char_LCD(0);}
        if(menu==1) {cmd_LCD(0xc0);char_LCD(1);str_LCD("      MIN     ");char_LCD(0);}
        if(menu==2) {cmd_LCD(0xc0);char_LCD(1);str_LCD("      SEC     ");char_LCD(0);}
				if(menu==3) {cmd_LCD(0xC0);char_LCD(1);str_LCD("     BACK     ");char_LCD(0);}
				
        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='6') menu++;
        if(key=='4') menu--;

        if(menu>3) menu=0;

        if(key=='='){
            if(menu==0) Hour();
            if(menu==1) Min();
						if(menu==2) Sec();
						if(menu==3) return;
        }
				if(key=='C'){ flag=1; if(flag ||(key=='C' && menu==3)) return; }
    }
}

// ---------------- RTC DATE MENU ----------------

void Edit_RTC_Date(){
    unsigned char key;
    unsigned int menu = 0;
    cmd_LCD(0x01);
    str_LCD("  EDIT RTC DATE ");
    while(1)
    {
				if(flag) return;
				Arrow_Init();
				cmd_LCD(0x80);
				str_LCD("  EDIT RTC DATE ");

        if(menu==0) {cmd_LCD(0xC0);str_LCD("       DOM     ");char_LCD(0);}
        if(menu==1) {cmd_LCD(0xC0);char_LCD(1);str_LCD("     MONTH    ");char_LCD(0);}
        if(menu==2) {cmd_LCD(0xC0);char_LCD(1);str_LCD("     YEAR     ");char_LCD(0);}
        if(menu==3) {cmd_LCD(0xC0);char_LCD(1);str_LCD("     BACK     ");char_LCD(0);}

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='6') menu++;
        if(key=='4') menu--;

        if(menu>3) menu=0;

        if(key=='='){
            if(menu==0) Day_OM();
            if(menu==1) Month();
            if(menu==2) Year();
						if(menu==3) return;
        }
				if(key=='C'){ flag=1; if(flag ||(key=='C' && menu==3)) return; }
    }
}

// ---------------- RTC DAY MENU ----------------

void Edit_RTC_Day(){
    unsigned char key;
    unsigned int menu = 0;
    cmd_LCD(0x01);
    str_LCD("  EDIT RTC DAY ");
    while(1)
    {
				if(flag) return;
				cmd_LCD(0x80);
				str_LCD("  EDIT RTC DAY ");
        cmd_LCD(0xC0);
				Arrow_Init();

        if(menu==0) {cmd_LCD(0xC0);str_LCD("       DAY     ");char_LCD(0);}
        if(menu==1) {cmd_LCD(0xC0);char_LCD(1);str_LCD("     BACK     ");char_LCD(0);}

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='6') menu++;
        if(key=='4') menu--;

        if(menu>1) menu=0;

        if(key=='='){ if(menu==0) Day(); if(menu==1) return;}
				if(key=='C'){ flag=1; if(flag ||(key=='C' && menu==1)) return; }
    }
}

// -------- MEDICINE 1 TIME MENU --------

void Edit_Med1_Time(){
    unsigned char key;
    unsigned int menu = 0;
    cmd_LCD(0x01);
    str_LCD(" EDIT MED 1 TIME");
	
    while(1)
    {
				if(flag) return;
				cmd_LCD(0x80);
				str_LCD(" EDIT MED 1 TIME");
        cmd_LCD(0xC0);
			  Arrow_Init();

        if(menu==0) {cmd_LCD(0xc0);str_LCD("      HOUR     ");char_LCD(0);}
        if(menu==1) {cmd_LCD(0xc0);char_LCD(1);str_LCD("      MIN     ");char_LCD(0);}
				if(menu==2) {cmd_LCD(0xC0);char_LCD(1);str_LCD("     BACK     ");char_LCD(0);}

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='6') menu++;
        if(key=='4') menu--;

        if(menu>2) menu=0;

        if(key=='='){
            if(menu==0) Med1_Hour();
            if(menu==1) Med1_Min();
						if(menu==2) return;
        }
				if(key=='C'){ flag=1; if(flag ||(key=='C' && menu==2)) return; }
    }
}

// -------- MEDICINE 2 TIME MENU --------

void Edit_Med2_Time(){
    unsigned char key;
    unsigned int menu = 0;
    cmd_LCD(0x01);
    str_LCD(" EDIT MED 2 TIME");
	
    while(1)
    {
				if(flag) return;
				cmd_LCD(0x80);
				str_LCD(" EDIT MED 2 TIME");
        cmd_LCD(0xC0);
			  Arrow_Init();

        if(menu==0) {cmd_LCD(0xc0);str_LCD("      HOUR     ");char_LCD(0);}
        if(menu==1) {cmd_LCD(0xc0);char_LCD(1);str_LCD("      MIN     ");char_LCD(0);}
				if(menu==2) {cmd_LCD(0xC0);char_LCD(1);str_LCD("     BACK     ");char_LCD(0);}

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='6') menu++;
        if(key=='4') menu--;

        if(menu>2) menu=0;

        if(key=='='){
            if(menu==0) Med2_Hour();
            if(menu==1) Med2_Min();
						if(menu==2) return;
        }
				if(key=='C'){ flag=1; if(flag ||(key=='C' && menu==2)) return; }
    }
}

// -------- MEDICINE 3 TIME MENU --------

void Edit_Med3_Time(){
    unsigned char key;
    unsigned int menu = 0;
    cmd_LCD(0x01);
    str_LCD(" EDIT MED 3 TIME");
	
    while(1)
    {
				if(flag) return;
				cmd_LCD(0x80);
				str_LCD(" EDIT MED 3 TIME");
        cmd_LCD(0xC0);
			  Arrow_Init();

        if(menu==0) {cmd_LCD(0xc0);str_LCD("      HOUR     ");char_LCD(0);}
        if(menu==1) {cmd_LCD(0xc0);char_LCD(1);str_LCD("      MIN     ");char_LCD(0);}
				if(menu==2) {cmd_LCD(0xC0);char_LCD(1);str_LCD("     BACK     ");char_LCD(0);}

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='6') menu++;
        if(key=='4') menu--;

        if(menu>2) menu=0;

        if(key=='='){
            if(menu==0) Med3_Hour();
            if(menu==1) Med3_Min();
						if(menu==2) return;
        }
				if(key=='C'){ flag=1; if(flag ||(key=='C' && menu==2)) return; }
    }
}

// -------- MEDICINE 4 TIME MENU --------

void Edit_Med4_Time(){
    unsigned char key;
    unsigned int menu = 0;
    cmd_LCD(0x01);
    str_LCD(" EDIT MED 4 TIME");
	
    while(1)
    {
				if(flag) return;
				cmd_LCD(0x80);
				str_LCD(" EDIT MED 4 TIME");
        cmd_LCD(0xC0);
			  Arrow_Init();

        if(menu==0) {cmd_LCD(0xc0);str_LCD("      HOUR     ");char_LCD(0);}
        if(menu==1) {cmd_LCD(0xc0);char_LCD(1);str_LCD("      MIN     ");char_LCD(0);}
				if(menu==2) {cmd_LCD(0xC0);char_LCD(1);str_LCD("     BACK     ");char_LCD(0);}

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='6') menu++;
        if(key=='4') menu--;

        if(menu>2) menu=0;

        if(key=='='){
            if(menu==0) Med4_Hour();
            if(menu==1) Med4_Min();
					  if(menu==2) return;
        }
				if(key=='C'){ flag=1; if(flag ||(key=='C' && menu==2)) return; }
    }
}

// -------- MEDICINE 5 TIME MENU --------

void Edit_Med5_Time(){
    unsigned char key;
    unsigned int menu = 0;
    cmd_LCD(0x01);
    str_LCD(" EDIT MED 5 TIME");
	
    while(1)
    {
				if(flag) return;
				cmd_LCD(0x80);
				str_LCD(" EDIT MED 5 TIME");
        cmd_LCD(0xC0);
			  Arrow_Init();

        if(menu==0) {cmd_LCD(0xc0);str_LCD("      HOUR     ");char_LCD(0);}
        if(menu==1) {cmd_LCD(0xc0);char_LCD(1);str_LCD("      MIN     ");char_LCD(0);}
				if(menu==2) {cmd_LCD(0xC0);char_LCD(1);str_LCD("     BACK     ");char_LCD(0);}

        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();

        if(key=='6') menu++;
        if(key=='4') menu--;

        if(menu>2) menu=0;

        if(key=='='){
            if(menu==0) Med5_Hour();
            if(menu==1) Med5_Min();
						if(menu==2) return;
        }
				if(key=='C'){ flag=1; if(flag ||(key=='C' && menu==2)) return; }
    }
}

//-----------------Displays Edit Menu---------------

void Edit_menu(){
    unsigned char key;
    int menu = 0;
		cmd_LCD(0x01);
		
    while(1)
    {	
				if(flag) {flag = 0; return;}
				Arrow_Init();
        if(menu==0) {cmd_LCD(0x80);str_LCD("    RTC TIME   ");char_LCD(0);
										 cmd_LCD(0xc0);str_LCD("    ");DisplayRTCTime(hour,min,sec);str_LCD("    ");}
        if(menu==1) {cmd_LCD(0x80);char_LCD(1);str_LCD("   RTC DATE   ");char_LCD(0);
										 cmd_LCD(0xc0);str_LCD("   ");DisplayRTCDate(date,month,year);str_LCD("   ");}
        if(menu==2) {cmd_LCD(0x80);char_LCD(1);str_LCD("   RTC DAY    ");char_LCD(0);
										 cmd_LCD(0xc0);str_LCD("       ");DisplayRTCDay(day);str_LCD("      ");}
        if(menu==3) {cmd_LCD(0x80);char_LCD(1);str_LCD("  MED 1 TIME  ");char_LCD(0);DisplayMED1();}
				if(menu==4) {cmd_LCD(0x80);char_LCD(1);str_LCD("  MED 2 TIME  ");char_LCD(0);DisplayMED2();}
				if(menu==5) {cmd_LCD(0x80);char_LCD(1);str_LCD("  MED 3 TIME  ");char_LCD(0);DisplayMED3();}
				if(menu==6) {cmd_LCD(0x80);char_LCD(1);str_LCD("  MED 4 TIME  ");char_LCD(0);DisplayMED4();}
				if(menu==7) {cmd_LCD(0x80);char_LCD(1);str_LCD("  MED 5 TIME  ");char_LCD(0);DisplayMED5();}
				if(menu==8) { cmd_LCD(0x80); char_LCD(1); str_LCD(" ADD MED SLOT "); char_LCD(0);
											if(cnt<2) { cmd_LCD(0xC0); str_LCD(" PRESS = TO ADD  "); }
												key = Keypad_Scan();
												if(key=='5') LCD_Control_UI();
												if(key=='=' && cnt < 2) {                                    // ADD SLOT
														cnt++;
														cmd_LCD(0x01);
														cmd_LCD(0x80);
														str_LCD(" SLOT ADDED ");
														cmd_LCD(0xC0);
														str_LCD(" SUCCESSFULLY ");
														delay_ms(1000);

														if(cnt==1) { med4 = 1; menu = 6; Edit_Med4_Time(); }
														else if(cnt==2) { med5 = 1; menu = 7;
																if(cnt >= 2){ cmd_LCD(0x80); str_LCD("  ALL MEDICINE  "); cmd_LCD(0xC0); str_LCD(" SLOTS ARE FULL "); delay_s(1); }
																Edit_Med5_Time();
														}
												}
												if(key=='6') { menu++;                                      // MENU INCREMENT
														if(med4 && med5 && menu==8) menu=9;
														if(menu>9) menu=0;
												}
												if(key=='4') { if(menu==0) menu=9; else menu--;             // MENU DECREMENT
														if(!med4 && !med5 && (menu==6 || menu==7)) menu=5;
														if(med4 && !med5 && menu==7) menu=6;
														if(med4 && med5 && menu==8) menu=7;
												}
										}
				if(menu==9) {cmd_LCD(0x80);char_LCD(1);str_LCD("     EXIT     ");char_LCD(0);
										 cmd_LCD(0xc0);str_LCD("  PRESS C EXIT  ");}
			
        key = Keypad_Scan();
				if(key=='5') LCD_Control_UI();
        if(key=='6') { menu++;
						if(!med4 && !med5) { if(menu==6 || menu==7) menu=8; }                    // Only MED1,2,3 active
						if(med4 && !med5) { if(menu==7) menu=8; }                                // MED4 active only
						if(med4 && med5) { if(menu==8) menu=9; }                                 // MED4 and MED5 both active ? skip ADD SLOT
						if(menu>9) menu=0;
				}
        if(key=='4') { menu--;
						if(menu < 0 ) menu = 9;
						if(!med4 && !med5) { if(menu==7 || menu==6) menu=5; }                    // Only MED1,2,3 active
						if(med4 && !med5) { if(menu==7) menu=6; }                                // MED4 active only
						if(med4 && med5) { if(menu==8) menu=7; }                                 // MED4 and MED5 both active ? skip ADD SLOT
				}
				
        if(key=='='){
            if(menu==0) Edit_RTC_Time();
            if(menu==1) Edit_RTC_Date();
            if(menu==2) Edit_RTC_Day();
            if(menu==3) Edit_Med1_Time();
						if(menu==4) Edit_Med2_Time();
						if(menu==5) Edit_Med3_Time();
						if(menu==6) Edit_Med4_Time();
						if(menu==7) Edit_Med5_Time();
        }
				if(key=='C'){ flag=1; if(flag ||(key=='C' && menu==9)){ flag = 0; return;} }
    }
}

void Show_Next_Med()
{
    int cur = HOUR*60 + MIN;
    int t[5], diff[5];
    int i, min_index=-1, min_val=10000;

    t[0] = M_H1*60 + M_min1;
    t[1] = M_H2*60 + M_min2;
    t[2] = M_H3*60 + M_min3;
    t[3] = med4 ? (M_H4*60 + M_min4) : 10000;
    t[4] = med5 ? (M_H5*60 + M_min5) : 10000;

    for(i=0;i<5;i++) {                                                    // Skip current medicine time
        if(t[i] == cur) diff[i] = 10000;
        else if(t[i] > cur) diff[i] = t[i] - cur;
        else diff[i] = (1440 - cur) + t[i];                                // next day
    }
    for(i=0;i<5;i++) {
        if(diff[i] < min_val)  {
            min_val = diff[i];
            min_index = i;
        }
    }

    cmd_LCD(0xC0);
    str_LCD(" NEXT MED ");

    switch(min_index)
    {
        case 0: display_LCD(M_H1); char_LCD(':'); display_LCD(M_min1); break;
        case 1: display_LCD(M_H2); char_LCD(':'); display_LCD(M_min2); break;
        case 2: display_LCD(M_H3); char_LCD(':'); display_LCD(M_min3); break;
        case 3: display_LCD(M_H4); char_LCD(':'); display_LCD(M_min4); break;
        case 4: display_LCD(M_H5); char_LCD(':'); display_LCD(M_min5); break;
    }
}


void Alert()
{
    IODIR0 |= BUZZ;

    // -------- MED1 --------
    if(M_H1==HOUR && M_min1==MIN && SEC<=1)
    {
        int i;

        cmd_LCD(0x01);
        str_LCD(" TAKE MEDICINE 1");
        cmd_LCD(0xC0);
        str_LCD(" PRESS SW2 STOP");

        IOSET0 = BUZZ;

        for(i=0;i<6000 && !f2;i++) delay_ms(100);

        IOCLR0 = BUZZ;
        f2 = 0;
				if(i==6000) { cmd_LCD(0x01); str_LCD("MEDICINE MISSED!"); cmd_LCD(0xc0); Show_Next_Med(); delay_ms(1500); }
				else {
					cmd_LCD(0x01);
					str_LCD("MEDICINE 1 TAKEN");

					Show_Next_Med();

					delay_ms(1500);
					cmd_LCD(0x01);
				}
    }

    // -------- MED2 --------
    if(M_H2==HOUR && M_min2==MIN && SEC<=1)
    {
        int i;

        cmd_LCD(0x01);
        str_LCD(" TAKE MEDICINE 2");
        cmd_LCD(0xC0);
        str_LCD(" PRESS SW2 STOP");

        IOSET0 = BUZZ;

        for(i=0;i<6000 && !f2;i++) delay_ms(100);

        IOCLR0 = BUZZ;
        f2 = 0;
				if(i==6000) { cmd_LCD(0x01); str_LCD("MEDICINE MISSED!"); cmd_LCD(0xc0); Show_Next_Med(); delay_ms(1500); }
				else {
					cmd_LCD(0x01);
					str_LCD("MEDICINE 2 TAKEN");

					Show_Next_Med();

					delay_ms(1500);
					cmd_LCD(0x01);
				}
    }

    // -------- MED3 --------
    if(M_H3==HOUR && M_min3==MIN && SEC<=1)
    {
        int i;

        cmd_LCD(0x01);
        str_LCD(" TAKE MEDICINE 3");
        cmd_LCD(0xC0);
        str_LCD(" PRESS SW2 STOP");

        IOSET0 = BUZZ;

        for(i=0;i<6000 && !f2;i++){ 
						delay_ms(100);
						IOCLR0 = BUZZ;
						f2 = 0;
				}
				
				if(i==6000) { cmd_LCD(0x01); str_LCD("MEDICINE MISSED!"); cmd_LCD(0xc0); Show_Next_Med(); delay_ms(1500); }
				else {
					cmd_LCD(0x01);
					str_LCD("MEDICINE 3 TAKEN");

					Show_Next_Med();

					delay_ms(1500);
					cmd_LCD(0x01);
				}
    }

    // -------- MED4 --------
    if(med4 && M_H4==HOUR && M_min4==MIN && SEC<=1)
    {
        int i;

        cmd_LCD(0x01);
        str_LCD(" TAKE MEDICINE 4");
        cmd_LCD(0xC0);
        str_LCD(" PRESS SW2 STOP");

        IOSET0 = BUZZ;

        for(i=0;i<6000 && !f2;i++) delay_ms(100);

        IOCLR0 = BUZZ;
        f2 = 0;
				if(i==6000) { cmd_LCD(0x01); str_LCD("MEDICINE MISSED!"); cmd_LCD(0xc0); Show_Next_Med(); delay_ms(1500); }
				else {
					cmd_LCD(0x01);
					str_LCD("MEDICINE 4 TAKEN");

					Show_Next_Med();

					delay_ms(1500);
					cmd_LCD(0x01);
				}
    }

    // -------- MED5 --------
    if(med5 && M_H5==HOUR && M_min5==MIN && SEC<=1)
    {
        int i;

        cmd_LCD(0x01);
        str_LCD(" TAKE MEDICINE 5");
        cmd_LCD(0xC0);
        str_LCD(" PRESS SW2 STOP");

        IOSET0 = BUZZ;

        for(i=0;i<6000 && !f2;i++) delay_ms(100);

        IOCLR0 = BUZZ;
        f2 = 0;
				if(i==6000) { cmd_LCD(0x01); str_LCD("MEDICINE MISSED!"); cmd_LCD(0xc0); Show_Next_Med(); delay_ms(1500); }
				else {
					cmd_LCD(0x01);
					str_LCD("MEDICINE 5 TAKEN");

					Show_Next_Med();

					delay_ms(1500);
					cmd_LCD(0x01);
				}
    }
}

// ---------------Displays Medicines Time---------------

void Display_MED1(){
		Arrow_Init();
    cmd_LCD(0x80);
    str_LCD(" MEDICINE 1 TIME");

    cmd_LCD(0xC0);
		char_LCD(1);
		str_LCD("   ");
    display_LCD(M_H1);
		char_LCD(':');
    display_LCD(M_min1);
		char_LCD(':');
		str_LCD("00   ");
		char_LCD(0);
}

void Display_MED2(){
		Arrow_Init();
    cmd_LCD(0x80);
    str_LCD(" MEDICINE 2 TIME");

    cmd_LCD(0xC0);
		char_LCD(1);
		str_LCD("   ");
    display_LCD(M_H2);
		char_LCD(':');
    display_LCD(M_min2);
		char_LCD(':');
		str_LCD("00   ");
		char_LCD(0);
}

void Display_MED3(){
		Arrow_Init();
    cmd_LCD(0x80);
    str_LCD(" MEDICINE 3 TIME");

    cmd_LCD(0xC0);
		char_LCD(1);
		str_LCD("   ");
    display_LCD(M_H3);
		char_LCD(':');
    display_LCD(M_min3);
		char_LCD(':');
		str_LCD("00   ");
		char_LCD(0);
}

void Display_MED4(){
		Arrow_Init();
    cmd_LCD(0x80);
    str_LCD(" MEDICINE 4 TIME");

    cmd_LCD(0xC0);
		char_LCD(1);
		str_LCD("   ");
    display_LCD(M_H4);
		char_LCD(':');
    display_LCD(M_min4);
		char_LCD(':');
		str_LCD("00   ");
		char_LCD(0);
}

void Display_MED5(){
		Arrow_Init();
    cmd_LCD(0x80);
    str_LCD(" MEDICINE 5 TIME");

    cmd_LCD(0xC0);
		char_LCD(1);
		str_LCD("   ");
    display_LCD(M_H5);
		char_LCD(':');
    display_LCD(M_min5);
		char_LCD(':');
		str_LCD("00   ");
		char_LCD(0);
}

// -----------Displays RTC and Medicines Times-----------

void Display_Menu()
{
    static int menu = 0;
    unsigned char key;
    int max_menu = 3;

    // Decide max menu based on slots
    if(med4 && med5) max_menu = 5;
    else if(med4) max_menu = 4;
    else max_menu = 3;

    // Display according to menu
    if(menu==0) DisplayRTC();
    if(menu==1) Display_MED1();
    if(menu==2) Display_MED2();
    if(menu==3) Display_MED3();
    if(menu==4 && med4) Display_MED4();
    if(menu==5 && med5) Display_MED5();

    key = Keypad_Scan();

    if(key=='5') LCD_Control_UI();
    // Increment
    if(key=='6') { menu++;
        if(menu > max_menu) menu = 0;
    }
    // Decrement
    if(key=='4') {
        if(menu == 0) menu = max_menu;
        else menu--;
    }
}
