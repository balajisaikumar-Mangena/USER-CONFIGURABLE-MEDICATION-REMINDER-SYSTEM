#include <lpc21xx.h>
#include "delay.h"
#include "lcd.h"

#define ROWS (0X0F<<16)
#define COLS (0xF0<<16)

unsigned char KeyPad[4][4]={
	{'7', '8', '9', '/'},
	{'4', '5', '6', '*'},
	{'1', '2', '3', '-'},
	{'C', '0', '=', '+'}
};

void init_KPM(){
	IODIR1 |= ROWS;     // Set row pins (P1.16-P1.19) as OUTPUT
	IODIR1 &= ~COLS;    // Set column pins (P1.20-P1.23) as INPUT
}

unsigned char Keypad_Scan(){
	unsigned int row, col;   // Variables for row and column scanning
	
	for(row=0; row<4; row++){                 // Scan each row
		IOSET1 = ROWS;                          // Set all rows HIGH
		IOCLR1 = 1<<(row+16);                   // Make current row LOW
		delay_us(10);
		
		for(col=0; col<4; col++){             	// Scan columns
			if(!(IOPIN1 & (1<<(col+20)))){        // Check if column becomes LOW (key pressed)
				while(!(IOPIN1 & (1<<(col+20))));   // Wait until key is released (debouncing)
				return KeyPad[row][col];            // Return corresponding key value
			}
		}
	}
	return 0;                                 // If no key pressed return 0
}

char last_key;
