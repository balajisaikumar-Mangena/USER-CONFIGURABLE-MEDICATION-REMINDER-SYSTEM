#include <lpc21xx.h>
#include "delay.h"
#include "lcd.h"
#include "KPM.h"
#include "RTC.h"
#include "EINT.h"
#include "M_P_func.h"

volatile unsigned char fl = 0;
volatile unsigned char f2 = 0;

unsigned int flag = 0;

int main()
{
	init_LCD();              // Initialization of LCD
	init_KPM();              // Initialization of KPM
	RTC_Init();              // Initialization of RTC
	Interrupt_Init();        // Initialization of Interrupts

	MRS();                   // Display Project name

	SetRTCTimeInfo(06,29,55);
	SetRTCDateInfo(16,03,2026);
	SetRTCDay(1);
	
	while(1){
		if(fl){                 // Interrupt Occurs
			cmd_LCD(0x01);        // Clear LCD
			MENU();								// Edit Mode Menu
			Edit_menu();          // Edit RTC Time Date and MED Time Menu 
			fl=0;									// Exit From Edit Menu 	
			cmd_LCD(0x01);
		}
		Display_Menu();         // Display RTC time date and Med Time 
		Alert();                // Buzzer on when MED Time Matches 
	}
}
