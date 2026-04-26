/*====================================================================
 *                  KEYPAD (KPM) CONFIGURATION
 *====================================================================*/

#include <lpc21xx.h>          // LPC21xx register definitions
#include "delay.h"            // Delay functions
#include "lcd.h"              // LCD functions (used with keypad UI)

#define ROWS (0X0F<<16)       // Row pins ? P1.16 to P1.19
#define COLS (0xF0<<16)       // Column pins ? P1.20 to P1.23


/*====================================================================
 *                  KEYPAD LAYOUT
 *====================================================================*/
unsigned char KeyPad[4][4]={  // 4x4 keypad mapping
    {'7', '8', '9', '/'},     // Row 0
    {'4', '5', '6', '*'},     // Row 1
    {'1', '2', '3', '-'},     // Row 2
    {'C', '0', '=', '+'}      // Row 3
};


/*====================================================================
 *                  KEYPAD INITIALIZATION
 *====================================================================*/
void init_KPM()                        // Initialize keypad pins
{
    IODIR1 |= ROWS;                   // Set rows (P1.16–P1.19) as OUTPUT
    IODIR1 &= ~COLS;                  // Set columns (P1.20–P1.23) as INPUT
}


/*====================================================================
 *                  KEYPAD SCAN FUNCTION
 *====================================================================*/
unsigned char Keypad_Scan()           // Scan keypad and return pressed key
{
    unsigned int row, col;            // Variables for row & column

    for(row=0; row<4; row++)          // Loop through each row
    {
        IOSET1 = ROWS;                // Set all rows HIGH
        IOCLR1 = 1<<(row+16);         // Make current row LOW (activate row)
        delay_us(10);                 // Small delay for stability

        for(col=0; col<4; col++)      // Check each column
        {
            if(!(IOPIN1 & (1<<(col+20)))) // If column pin LOW ? key pressed
            {
                while(!(IOPIN1 & (1<<(col+20)))); // Wait until key release (debounce)
                return KeyPad[row][col];          // Return key value from array
            }
        }
    }

    return 0;                         // No key pressed
}
