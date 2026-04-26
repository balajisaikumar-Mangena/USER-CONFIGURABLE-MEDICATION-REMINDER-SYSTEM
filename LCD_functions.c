/*====================================================================
 *                  LCD HEADER & DEFINITIONS
 *====================================================================*/

#include <lpc21xx.h>          // Include LPC21xx register definitions
#include "delay.h"            // Include delay functions

#define DATA (0xFF<<8)        // Data lines connected to P0.8–P0.15
#define RS   (1<<5)           // Register Select pin (P0.5)
#define RW   (1<<6)           // Read/Write pin (P0.6)
#define EN   (1<<7)           // Enable pin (P0.7)


/*====================================================================
 *                  LCD COMMAND FUNCTION
 *====================================================================*/
void cmd_LCD(unsigned char cmd)      // Function to send command to LCD
{
    IOCLR0 = RS;                     // RS = 0 ? Command mode
    IOCLR0 = RW;                     // RW = 0 ? Write operation
    
    IOCLR0 = DATA;                   // Clear previous data on bus
    IOSET0 = (cmd<<8) & DATA;        // Place command on data pins (shift to P0.8–P0.15)
    
    IOSET0 = EN;                     // Enable = HIGH (start command latch)
    delay_ms(1);                     // Small delay
    IOCLR0 = EN;                     // Enable = LOW (latch command)
    delay_ms(1);                     // Wait for LCD to process
}


/*====================================================================
 *                  LCD CHARACTER FUNCTION
 *====================================================================*/
void char_LCD(unsigned char ch)      // Function to send character to LCD
{
    IOSET0 = RS;                     // RS = 1 ? Data mode
    IOCLR0 = RW;                     // RW = 0 ? Write operation
    
    IOCLR0 = DATA;                   // Clear previous data
    IOSET0 = (ch<<8) & DATA;         // Put character on data pins
    
    IOSET0 = EN;                     // Enable HIGH
    delay_ms(1);                     // Delay
    IOCLR0 = EN;                     // Enable LOW
    delay_ms(1);                     // Wait for execution
}


/*====================================================================
 *                  LCD INITIALIZATION (8-BIT MODE)
 *====================================================================*/
void init_LCD(void)                  // Initialize LCD
{
    IODIR0 |= DATA | RS | RW | EN;   // Set all LCD pins as output
    
    delay_ms(15);                    // Wait after power ON

    cmd_LCD(0x30);                   // Wake-up command
    delay_ms(5);                     // Delay

    cmd_LCD(0x30);                   // Repeat initialization
    delay_us(100);                   // Short delay

    cmd_LCD(0x30);                   // Final wake-up

    cmd_LCD(0x38);                   // Function set: 8-bit, 2-line
    cmd_LCD(0x0C);                   // Display ON, cursor OFF
    
    cmd_LCD(0x01);                   // Clear display
    cmd_LCD(0x06);                   // Entry mode (cursor increment)
}


/*====================================================================
 *                  LCD INITIALIZATION (ALT VERSION)
 *====================================================================*/
void init_LCD_4(void)                // Alternate initialization (same config)
{
    IODIR0 |= DATA | RS | RW | EN;   // Set pins as output

    delay_ms(15);                    // Power delay

    cmd_LCD(0x30);                   // Wake-up
    delay_ms(5);

    cmd_LCD(0x30);                   // Repeat
    delay_us(100);

    cmd_LCD(0x30);                   // Repeat

    cmd_LCD(0x38);                   // 8-bit, multi-line mode
    cmd_LCD(0x0C);                   // Display ON
    cmd_LCD(0x01);                   // Clear LCD
    cmd_LCD(0x06);                   // Cursor increment
}


/*====================================================================
 *                  STRING DISPLAY FUNCTION
 *====================================================================*/
void str_LCD(char *str)              // Function to display string
{
    while(*str)                      // Loop until null character
    {
        char_LCD(*str++);            // Send each character and move pointer
    }
}


/*====================================================================
 *                  INTEGER DISPLAY FUNCTION
 *====================================================================*/
void num_LCD(int n)                  // Function to display integer
{
    unsigned char ch[10];            // Array to store digits
    int i = 0, j;                    // Loop variables
    int flag = 0;                    // Negative number flag

    if(n < 0)                        // Check if number is negative
    {
        flag = 1;                    // Set flag
        n = -n;                      // Convert to positive
    }

    if(n == 0)                       // If number is zero
    {
        char_LCD('0');               // Display '0'
        return;                      // Exit function
    }

    while(n)                         // Extract digits
    {
        ch[i++] = (n % 10) + '0';    // Convert digit to ASCII and store
        n /= 10;                     // Remove last digit
    }

    if(flag)                         // If number was negative
    {
        ch[i++] = '-';               // Add minus sign
    }

    for(j = i - 1; j >= 0; j--)      // Print digits in correct order
    {
        char_LCD(ch[j]);             // Display each digit
    }
}
