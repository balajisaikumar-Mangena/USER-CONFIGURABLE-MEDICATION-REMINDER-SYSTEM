#include <lpc21xx.h>              // LPC21xx registers
#include "delay.h"                // Delay functions
#include "lcd.h"                  // LCD functions
#include "KPM.h"                  // Keypad module
#include "RTC.h"                  // RTC functions
#include "EINT.h"                 // External interrupts
#include <cstdlib>                // Standard library


/*====================================================================
 *                      MACRO DEFINITIONS
 *====================================================================*/
#define BUZZ (1<<16)              // Buzzer pin (P0.16)


/*====================================================================
 *                      GLOBAL VARIABLES
 *====================================================================*/
extern unsigned int flag;         // External flag

unsigned int med4 = 0;            // Medicine 4 status
unsigned int med5 = 0;            // Medicine 5 status
unsigned int cnt  = 0;            // Counter


/*====================================================================
 *                      TIME & DATE VARIABLES
 *====================================================================*/
int hour, min, sec;               // Time variables
int date, month, year;           // Date variables
int day;                          // Day variable


/*====================================================================
 *                      MEDICINE TIME SETTINGS
 *====================================================================*/
// Fixed medicine timings
unsigned int M_H1   = 6;          // Medicine 1 hour
unsigned int M_min1 = 30;         // Medicine 1 minute

unsigned int M_H2   = 12;         // Medicine 2 hour
unsigned int M_min2 = 30;         // Medicine 2 minute

unsigned int M_H3   = 20;         // Medicine 3 hour
unsigned int M_min3 = 30;         // Medicine 3 minute

// User-defined timings
unsigned int M_H4;                // Medicine 4 hour
unsigned int M_min4;              // Medicine 4 minute

unsigned int M_H5;                // Medicine 5 hour
unsigned int M_min5;              // Medicine 5 minute


/*====================================================================
 *                      FUNCTION DECLARATIONS
 *====================================================================*/
 
void display_LCD(unsigned int num);   // Display number on LCD

/*====================================================================
 *              CGRAM INITIALIZATION & ARROW DISPLAY UI
 *====================================================================*/
void LCD_Control_UI()
{
    unsigned char i;                           // Loop variable

    unsigned char arrow[4][8] =                // Arrow patterns
    {
        {0x04,0x08,0x1F,0x08,0x04,0,0,0},      // Left arrow
        {0x04,0x02,0x1F,0x02,0x04,0,0,0},      // Right arrow
        {0x04,0x0E,0x15,0x04,0x04,0x04,0x04,0},// Up arrow
        {0x04,0x04,0x04,0x04,0x15,0x0E,0x04,0} // Down arrow
    };

    cmd_LCD(0x40);                             // CGRAM address set

    for(i=0;i<32;i++)                          // Load patterns
        char_LCD(((unsigned char*)arrow)[i]);  // Write to CGRAM

    cmd_LCD(0x01);                             // Clear LCD

    cmd_LCD(0x80);                             // First line
    str_LCD("4 ");   char_LCD(0);               // 4 ? Left
    str_LCD(" 6 ");  char_LCD(1);               // 6 ? Right
    str_LCD("  2 "); char_LCD(2);               // 2 ? Up
    str_LCD(" 8 ");  char_LCD(3);               // 8 ? Down

    cmd_LCD(0xC0);                             // Second line
    str_LCD("= OK/SAVE C EXIT");               // Instructions

    delay_ms(1000);                            // Wait
    cmd_LCD(0x01);                             // Clear LCD
}


/*====================================================================
 *                      LEFT & RIGHT ARROW INIT
 *====================================================================*/
void Arrow_Init()
{
    cmd_LCD(0x40);                                                  // CGRAM start
    
    char_LCD(0x04); char_LCD(0x06); char_LCD(0x1F); char_LCD(0x06); // Left arrow
    char_LCD(0x04); char_LCD(0x00); char_LCD(0x00); char_LCD(0x00);

    char_LCD(0x04); char_LCD(0x0C); char_LCD(0x1F); char_LCD(0x0C); // Right arrow
    char_LCD(0x04); char_LCD(0x00); char_LCD(0x00); char_LCD(0x00);
}


/*====================================================================
 *                      UP & DOWN ARROW INIT
 *====================================================================*/
void Arrow_Init1()
{
    cmd_LCD(0x40);                                                  // CGRAM start

    char_LCD(0x04); char_LCD(0x0E); char_LCD(0x15); char_LCD(0x04); // Up arrow
    char_LCD(0x04); char_LCD(0x04); char_LCD(0x00); char_LCD(0x00);

    char_LCD(0x04); char_LCD(0x04); char_LCD(0x04); char_LCD(0x04); // Down arrow
    char_LCD(0x15); char_LCD(0x0E); char_LCD(0x04); char_LCD(0x00);
}


/*====================================================================
 *                      ARROW DISPLAY UI
 *====================================================================*/
void Arrow_UI()
{
    Arrow_Init1();                             // Load arrows

    cmd_LCD(0xC0);                             // Second line

    char_LCD(0);   str_LCD("    ");            // Up arrow
    char_LCD(1);   str_LCD("    ");            // Down arrow

    str_LCD("OK");                             // OK option
}

/*====================================================================
 *                  DISPLAY MENU & START SCREEN
 *====================================================================*/
void MENU()
{
    cmd_LCD(0x01);                              // Clear LCD
    str_LCD(" * EDIT MODE * ");                 // Title
    cmd_LCD(0xc0);                              // Second line
    str_LCD(" RTC  MED  EXIT ");                // Options
    delay_s(1);                                 // Wait
}

void MRS()
{
    cmd_LCD(0x01);                              // Clear
    str_LCD("MEDICICATION SYS");                // Title
    cmd_LCD(0xc0);                              // Second line
    str_LCD("REMINDER   READY");                // Status
    delay_s(1);                                 // Wait

    cmd_LCD(0x01);                              // Clear
    str_LCD("  PRESS 5  FOR  ");                // Instruction
    cmd_LCD(0xc0);                              // Second line
    str_LCD("  CONTROL KEYS  ");                // Info
    delay_s(1);                                 // Wait

    cmd_LCD(0x01);                              // Clear
}


/*====================================================================
 *                  DISPLAY MEDICINE TIMES
 *====================================================================*/
void DisplayMED1()
{
    cmd_LCD(0xc0);                              // Second line
    str_LCD("    ");                            // Space
    display_LCD(M_H1);                          // Hour
    char_LCD(':');                              // Separator
    display_LCD(M_min1);                        // Minute
    char_LCD(':');                              // Separator
    str_LCD("00    ");                          // Seconds
}

void DisplayMED2()
{
    cmd_LCD(0xc0);                              // Second line
    str_LCD("    ");                            // Space
    display_LCD(M_H2);                          // Hour
    char_LCD(':');                              // Separator
    display_LCD(M_min2);                        // Minute
    char_LCD(':');                              // Separator
    str_LCD("00    ");                          // Seconds
}

void DisplayMED3()
{
    cmd_LCD(0xc0);                              // Second line
    str_LCD("    ");                            // Space
    display_LCD(M_H3);                          // Hour
    char_LCD(':');                              // Separator
    display_LCD(M_min3);                        // Minute
    char_LCD(':');                              // Separator
    str_LCD("00    ");                          // Seconds
}

void DisplayMED4()
{
    cmd_LCD(0xc0);                              // Second line
    str_LCD("    ");                            // Space
    display_LCD(M_H4);                          // Hour
    char_LCD(':');                              // Separator
    display_LCD(M_min4);                        // Minute
    char_LCD(':');                              // Separator
    str_LCD("00    ");                          // Seconds
}

void DisplayMED5()
{
    cmd_LCD(0xc0);                              // Second line
    str_LCD("    ");                            // Space
    display_LCD(M_H5);                          // Hour
    char_LCD(':');                              // Separator
    display_LCD(M_min5);                        // Minute
    char_LCD(':');                              // Separator
    str_LCD("00    ");                          // Seconds
}


/*====================================================================
 *                  DISPLAY NUMBER FUNCTION
 *====================================================================*/
void display_LCD(unsigned int num)
{
    char arr[5];                                // Buffer

    arr[0] = (num/10) + '0';                    // Tens digit
    arr[1] = (num%10) + '0';                    // Units digit
    arr[2] = '\0';                              // String end

    str_LCD(arr);                               // Display number
}


/*====================================================================
 *                  VALUE CONTROL FUNCTIONS
 *====================================================================*/
void Increment_Value(unsigned int *value, unsigned int max)
{
    (*value)++;                                 // Increment
    if(*value > max) *value = 0;                // Reset if overflow
}

void Decrement_Value(unsigned int *value, unsigned int max)
{
    if(*value == 0) *value = max;               // Wrap to max
    else (*value)--;                            // Decrement
}

/*====================================================================
 *                      HOUR EDIT FUNCTION
 *====================================================================*/
void Hour()                                  // Edit hour using keypad
{
    unsigned char key;                        // Store keypad input
    unsigned int temp = HOUR;                 // Copy current hour value

    cmd_LCD(0x01);                            // Clear LCD display
    str_LCD("SET HOUR :      ");              // Show heading

    while(1)                                  // Loop until save/exit
    {
        if(flag) return;                      // Exit if flag set

        cmd_LCD(0x80);                        // Move cursor to 1st line
        str_LCD("SET HOUR : ");              // Display label
        display_LCD(temp);                    // Show current hour

        cmd_LCD(0xc0);                        // Move cursor to 2nd line
        Arrow_UI();                           // Show arrow controls

        key = Keypad_Scan();                  // Read key from keypad

        if(key=='5') LCD_Control_UI();        // Show control help UI

        if(key=='2') Increment_Value(&temp,23); // Increase (0–23)
        if(key=='8') Decrement_Value(&temp,23); // Decrease

        if(key=='C')                          // Cancel key pressed
        {
            flag = 1;                         // Set exit flag
            return;                           // Exit without saving
        }

        if(key=='=')                          // Save key pressed
        {
            HOUR = temp;                      // Store value to RTC
            cmd_LCD(0x01);                    // Clear LCD
            return;                           // Exit after saving
        }

        delay_ms(150);                        // Delay for key debounce
    }
}

/*====================================================================
 *                      MINUTE EDIT FUNCTION
 *====================================================================*/
void Min()                                  // Edit minutes using keypad
{
    unsigned char key;                        // Store keypad input
    unsigned int temp = MIN;                  // Copy current minute

    cmd_LCD(0x01);                            // Clear LCD
    str_LCD("SET MIN :       ");              // Show heading

    while(1)                                  // Loop until save/exit
    {
        if(flag) return;                      // Exit if flag set

        cmd_LCD(0x80);                        // Move cursor to 1st line
        str_LCD("SET MIN : ");               // Label
        display_LCD(temp);                    // Show minute value

        cmd_LCD(0xc0);                        // Move cursor to 2nd line
        Arrow_UI();                           // Show arrow controls

        key = Keypad_Scan();                  // Read keypad input

        if(key=='5') LCD_Control_UI();        // Show help UI

        if(key=='2') Increment_Value(&temp,59); // Increase (0–59)
        if(key=='8') Decrement_Value(&temp,59); // Decrease

        if(key=='C'){ flag=1; return; }       // Cancel and exit
        if(key=='='){ MIN=temp; cmd_LCD(0x01); return; } // Save

        delay_ms(150);                        // Delay (debounce)
    }
}


/*====================================================================
 *                      SECOND EDIT FUNCTION
 *====================================================================*/
void Sec()                                  // Edit seconds using keypad
{
    unsigned char key;                        // Store keypad input
    unsigned int temp = SEC;                  // Copy current seconds

    cmd_LCD(0x01);                            // Clear LCD
    str_LCD("SET SEC :       ");              // Show heading

    while(1)                                  // Loop until save/exit
    {
        if(flag) return;                      // Exit if flag set

        cmd_LCD(0x80);                        // Move cursor to 1st line
        str_LCD("SET SEC : ");               // Label
        display_LCD(temp);                    // Show seconds value

        cmd_LCD(0xc0);                        // Move cursor to 2nd line
        Arrow_UI();                           // Show controls

        key = Keypad_Scan();                  // Read keypad input

        if(key=='5') LCD_Control_UI();        // Help UI

        if(key=='2') Increment_Value(&temp,59); // Increase (0–59)
        if(key=='8') Decrement_Value(&temp,59); // Decrease

        if(key=='C'){ flag=1; return; }       // Cancel
        if(key=='='){ SEC=temp; cmd_LCD(0x01); return; } // Save

        delay_ms(150);                        // Delay
    }
}


/*====================================================================
 *                      DATE (DOM) EDIT FUNCTION
 *====================================================================*/
void Day_OM()                               // Edit date (day of month)
{
    unsigned char key;                        // Store keypad input
    unsigned int temp = DOM;                  // Copy current date

    cmd_LCD(0x01);                            // Clear LCD
    str_LCD("SET DOM :       ");              // Show heading

    while(1)                                  // Loop until save/exit
    {
        if(flag) return;                      // Exit if flag set

        cmd_LCD(0x80);                        // Move cursor to 1st line
        str_LCD("SET DOM : ");               // Label
        display_LCD(temp);                    // Show date value

        cmd_LCD(0xc0);                        // Move cursor to 2nd line
        Arrow_UI();                           // Show controls

        key = Keypad_Scan();                  // Read keypad input

        if(key=='5') LCD_Control_UI();        // Help UI

        if(key=='2') Increment_Value(&temp,31); // Increase (1–31)
        if(key=='8') Decrement_Value(&temp,31); // Decrease

        if(key=='C'){ flag=1; return; }       // Cancel
        if(key=='='){ DOM=temp; cmd_LCD(0x01); return; } // Save

        delay_ms(150);                        // Delay
    }
}


/*====================================================================
 *                      MONTH EDIT FUNCTION
 *====================================================================*/
void Month()                                  // Edit month using keypad
{
    unsigned char key;                        // Store keypad input
    unsigned int temp = MONTH;                // Copy current month

    cmd_LCD(0x01);                            // Clear LCD
    str_LCD("SET MONTH :     ");              // Show heading

    while(1)                                  // Loop until save/exit
    {
        if(flag) return;                      // Exit if flag set

        cmd_LCD(0x80);                        // Move cursor to 1st line
        str_LCD("SET MONTH : ");             // Label
        display_LCD(temp);                    // Show month value

        cmd_LCD(0xc0);                        // Move cursor to 2nd line
        Arrow_UI();                           // Show arrow controls

        key = Keypad_Scan();                  // Read keypad input

        if(key=='5') LCD_Control_UI();        // Show help UI

        if(key=='2') Increment_Value(&temp,12); // Increase (1–12)
        if(key=='8') Decrement_Value(&temp,12); // Decrease

        if(key=='C'){ flag=1; return; }       // Cancel and exit
        if(key=='='){ MONTH=temp; cmd_LCD(0x01); return; } // Save

        delay_ms(150);                        // Delay (debounce)
    }
}


/*====================================================================
 *                      YEAR EDIT FUNCTION
 *====================================================================*/
void Year()                                   // Edit year using keypad
{
    unsigned char key;                        // Store keypad input
    unsigned int temp = YEAR;                 // Copy current year

    cmd_LCD(0x01);                            // Clear LCD
    str_LCD("SET YEAR :      ");              // Show heading

    while(1)                                  // Loop until save/exit
    {
        if(flag) return;                      // Exit if flag set

        cmd_LCD(0x80);                        // Move cursor to 1st line
        str_LCD("SET YEAR : ");              // Label

        char_LCD((temp/1000)%10 + 48);        // Display 1000s digit
        char_LCD((temp/100)%10 + 48);         // Display 100s digit
        char_LCD((temp/10)%10 + 48);          // Display 10s digit
        char_LCD((temp%10) + 48);             // Display units digit

        cmd_LCD(0xc0);                        // Move cursor to 2nd line
        Arrow_UI();                           // Show controls

        key = Keypad_Scan();                  // Read keypad input

        if(key=='5') LCD_Control_UI();        // Show help UI

        if(key=='2') Increment_Value(&temp,2099); // Increase (max limit)
        if(key=='8') Decrement_Value(&temp,2000); // Decrease (min limit)

        if(key=='C'){ flag=1; return; }       // Cancel
        if(key=='='){ YEAR=temp; cmd_LCD(0x01); return; } // Save

        delay_ms(150);                        // Delay
    }
}


/*====================================================================
 *                      DAY (DOW) EDIT FUNCTION
 *====================================================================*/
void Day()                                    // Edit day using keypad
{
    unsigned char key;                        // Store keypad input
    unsigned int temp = DOW;                  // Copy current day

    cmd_LCD(0x01);                            // Clear LCD
    str_LCD("SET DAY :       ");              // Show heading

    while(1)                                  // Loop until save/exit
    {
        if(flag) return;                      // Exit if flag set

        cmd_LCD(0x80);                        // Move cursor to 1st line
        str_LCD("SET DAY : ");               // Label

        if(temp==0) str_LCD("SUN");           // Sunday
        if(temp==1) str_LCD("MON");           // Monday
        if(temp==2) str_LCD("TUE");           // Tuesday
        if(temp==3) str_LCD("WED");           // Wednesday
        if(temp==4) str_LCD("THU");           // Thursday
        if(temp==5) str_LCD("FRI");           // Friday
        if(temp==6) str_LCD("SAT");           // Saturday

        cmd_LCD(0xc0);                        // Move cursor to 2nd line
        Arrow_UI();                           // Show controls

        key = Keypad_Scan();                  // Read keypad input

        if(key=='5') LCD_Control_UI();        // Show help UI

        if(key=='2') Increment_Value(&temp,6); // Increase (0–6)
        if(key=='8') Decrement_Value(&temp,6); // Decrease

        if(key=='C'){ flag=1; return; }       // Cancel
        if(key=='='){ DOW=temp; cmd_LCD(0x01); return; } // Save

        delay_ms(150);                        // Delay
    }
}

/*====================================================================
 *                  MEDICINE 1 HOUR EDIT
 *====================================================================*/
void Med1_Hour()                              // Function to edit Medicine 1 hour
{
    unsigned char key;                        // Variable to store keypad input
    unsigned int temp = M_H1;                 // Copy current hour into temp

    cmd_LCD(0x01);                            // Clear LCD screen
    str_LCD("SET HOUR :      ");              // Display heading

    while(1)                                  // Infinite loop
    {
        if(flag) return;                      // Exit if flag is set

        cmd_LCD(0x80);                        // Move cursor to 1st line
        str_LCD("SET HOUR : ");              // Display label
        display_LCD(temp);                    // Show current hour value

        cmd_LCD(0xc0);                        // Move cursor to 2nd line
        Arrow_UI();                           // Display arrow controls

        key = Keypad_Scan();                  // Read key from keypad

        if(key=='5') LCD_Control_UI();        // Show help/control screen

        if(key=='2') Increment_Value(&temp,23); // Increase value (max 23)
        if(key=='8') Decrement_Value(&temp,23); // Decrease value

        if(key=='C')                          // If cancel key pressed
        {
            flag = 1;                         // Set exit flag
            return;                           // Exit without saving
        }

        if(key=='=')                          // If save key pressed
        {
            M_H1 = temp;                      // Save value to Medicine 1 hour
            cmd_LCD(0x01);                    // Clear LCD
            return;                           // Exit function
        }

        delay_ms(150);                        // Delay for debounce
    }
}

/*====================================================================
 *                  MEDICINE 1 MINUTE EDIT
 *====================================================================*/
void Med1_Min()                               // Function to edit Medicine 1 minute
{
    unsigned char key;                        // Key input
    unsigned int temp = M_min1;               // Copy current minute

    cmd_LCD(0x01);                            // Clear LCD
    str_LCD("SET MIN :       ");              // Heading

    while(1)
    {
        if(flag) return;                      // Exit if flag set

        cmd_LCD(0x80);                        // First line
        str_LCD("SET MIN : ");
        display_LCD(temp);                    // Show minute value

        cmd_LCD(0xc0);                        // Second line
        Arrow_UI();                           // Show controls

        key = Keypad_Scan();                  // Read key

        if(key=='5') LCD_Control_UI();        // Help screen

        if(key=='2') Increment_Value(&temp,59); // Increase (0–59)
        if(key=='8') Decrement_Value(&temp,59); // Decrease

        if(key=='C'){ flag=1; return; }       // Cancel
        if(key=='='){ M_min1=temp; cmd_LCD(0x01); return; } // Save

        delay_ms(150);                        // Delay
    }
}

/*====================================================================
 *                  MEDICINE 2 HOUR EDIT
 *====================================================================*/
void Med2_Hour()                             // Function to edit Medicine 2 hour
{
    unsigned char key;                       // Variable to store keypad input
    unsigned int temp = M_H2;                // Copy current hour into temp

    cmd_LCD(0x01);                           // Clear LCD screen
    str_LCD("SET HOUR :      ");             // Display heading

    while(1)                                 // Infinite loop
    {
        if(flag) return;                     // Exit if flag is set

        cmd_LCD(0x80);                       // Move cursor to 1st line
        str_LCD("SET HOUR : ");              // Display label
        display_LCD(temp);                   // Show current hour value

        cmd_LCD(0xc0);                       // Move cursor to 2nd line
        Arrow_UI();                          // Display arrow controls

        key = Keypad_Scan();                 // Read key from keypad

        if(key=='5') LCD_Control_UI();       // Show help/control UI

        if(key=='2') Increment_Value(&temp,23); // Increase hour (0–23)
        if(key=='8') Decrement_Value(&temp,23); // Decrease hour

        if(key=='C')                         // If cancel key pressed
        {
            flag = 1;                        // Set exit flag
            return;                          // Exit function
        }

        if(key=='=')                         // If save key pressed
        {
            M_H2 = temp;                     // Save value to Medicine 2 hour
            cmd_LCD(0x01);                   // Clear LCD
            return;                          // Exit function
        }

        delay_ms(150);                       // Delay for debounce
    }
}

/*====================================================================
 *                  MEDICINE 2 MINUTE EDIT
 *====================================================================*/
void Med2_Min()                              // Function to edit Medicine 2 minute
{
    unsigned char key;                       // Variable to store keypad input
    unsigned int temp = M_min2;              // Copy current minute into temp

    cmd_LCD(0x01);                           // Clear LCD screen
    str_LCD("SET MIN :       ");             // Display heading

    while(1)                                 // Infinite loop
    {
        if(flag) return;                     // Exit if flag is set

        cmd_LCD(0x80);                       // Move cursor to 1st line
        str_LCD("SET MIN : ");               // Display label
        display_LCD(temp);                   // Show current minute value

        cmd_LCD(0xc0);                       // Move cursor to 2nd line
        Arrow_UI();                          // Display arrow controls

        key = Keypad_Scan();                 // Read key from keypad

        if(key=='5') LCD_Control_UI();       // Show help/control UI

        if(key=='2') Increment_Value(&temp,59); // Increase minute (0–59)
        if(key=='8') Decrement_Value(&temp,59); // Decrease minute

        if(key=='C')                         // If cancel key pressed
        {
            flag = 1;                        // Set exit flag
            return;                          // Exit function
        }

        if(key=='=')                         // If save key pressed
        {
            M_min2 = temp;                   // Save value to Medicine 2 minute
            cmd_LCD(0x01);                   // Clear LCD
            return;                          // Exit function
        }

        delay_ms(150);                       // Delay for debounce
    }
}

/*====================================================================
 *                  MEDICINE 3 HOUR EDIT
 *====================================================================*/
void Med3_Hour()                            // Function to edit Medicine 3 hour
{
    unsigned char key;                      // Variable to store keypad input
    unsigned int temp = M_H3;               // Copy current hour into temp

    cmd_LCD(0x01);                          // Clear LCD screen
    str_LCD("SET HOUR :      ");            // Display heading

    while(1)                                // Infinite loop
    {
        if(flag) return;                    // Exit if flag is set

        cmd_LCD(0x80);                      // Move cursor to 1st line
        str_LCD("SET HOUR : ");             // Display label
        display_LCD(temp);                  // Show current hour value

        cmd_LCD(0xc0);                      // Move cursor to 2nd line
        Arrow_UI();                         // Display arrow controls

        key = Keypad_Scan();                // Read key from keypad

        if(key=='5') LCD_Control_UI();      // Show help/control UI

        if(key=='2') Increment_Value(&temp,23); // Increase hour (0–23)
        if(key=='8') Decrement_Value(&temp,23); // Decrease hour

        if(key=='C')                        // If cancel key pressed
        {
            flag = 1;                       // Set exit flag
            return;                         // Exit function
        }

        if(key=='=')                        // If save key pressed
        {
            M_H3 = temp;                    // Save value to Medicine 3 hour
            cmd_LCD(0x01);                  // Clear LCD
            return;                         // Exit function
        }

        delay_ms(150);                      // Delay for debounce
    }
}

/*====================================================================
 *                  MEDICINE 3 MINUTE EDIT
 *====================================================================*/
void Med3_Min()                             // Function to edit Medicine 3 minute
{
    unsigned char key;                      // Variable to store keypad input
    unsigned int temp = M_min3;             // Copy current minute into temp

    cmd_LCD(0x01);                          // Clear LCD screen
    str_LCD("SET MIN :       ");            // Display heading

    while(1)                                // Infinite loop
    {
        if(flag) return;                    // Exit if flag is set

        cmd_LCD(0x80);                      // Move cursor to 1st line
        str_LCD("SET MIN : ");              // Display label
        display_LCD(temp);                  // Show current minute value

        cmd_LCD(0xc0);                      // Move cursor to 2nd line
        Arrow_UI();                         // Display arrow controls

        key = Keypad_Scan();                // Read key from keypad

        if(key=='5') LCD_Control_UI();      // Show help/control UI

        if(key=='2') Increment_Value(&temp,59); // Increase minute (0–59)
        if(key=='8') Decrement_Value(&temp,59); // Decrease minute

        if(key=='C')                        // If cancel key pressed
        {
            flag = 1;                       // Set exit flag
            return;                         // Exit function
        }

        if(key=='=')                        // If save key pressed
        {
            M_min3 = temp;                  // Save value to Medicine 3 minute
            cmd_LCD(0x01);                  // Clear LCD
            return;                         // Exit function
        }

        delay_ms(150);                      // Delay for debounce
    }
}

/*====================================================================
 *                  MEDICINE 4 HOUR EDIT
 *====================================================================*/
void Med4_Hour()                             // Function to edit Medicine 4 hour
{
    unsigned char key;                       // Variable to store keypad input
    unsigned int temp = M_H4;                // Copy current hour into temp

    cmd_LCD(0x01);                           // Clear LCD screen
    str_LCD("SET HOUR :      ");             // Display heading

    while(1)                                 // Infinite loop
    {
        if(flag) return;                     // Exit if flag is set

        cmd_LCD(0x80);                       // Move cursor to 1st line
        str_LCD("SET HOUR : ");              // Display label
        display_LCD(temp);                   // Show current hour value

        cmd_LCD(0xc0);                       // Move cursor to 2nd line
        Arrow_UI();                          // Display arrow controls

        key = Keypad_Scan();                 // Read key from keypad

        if(key=='5') LCD_Control_UI();       // Show help/control UI

        if(key=='2') Increment_Value(&temp,23); // Increase hour (0–23)
        if(key=='8') Decrement_Value(&temp,23); // Decrease hour

        if(key=='C')                         // If cancel key pressed
        {
            flag = 1;                        // Set exit flag
            return;                          // Exit function
        }

        if(key=='=')                         // If save key pressed
        {
            M_H4 = temp;                     // Save value to Medicine 4 hour
            cmd_LCD(0x01);                   // Clear LCD
            return;                          // Exit function
        }

        delay_ms(150);                       // Delay for debounce
    }
}


/*====================================================================
 *                  MEDICINE 4 MINUTE EDIT
 *====================================================================*/
void Med4_Min()                              // Function to edit Medicine 4 minute
{
    unsigned char key;                       // Variable to store keypad input
    unsigned int temp = M_min4;              // Copy current minute into temp

    cmd_LCD(0x01);                           // Clear LCD screen
    str_LCD("SET MIN :       ");             // Display heading

    while(1)                                 // Infinite loop
    {
        if(flag) return;                     // Exit if flag is set

        cmd_LCD(0x80);                       // Move cursor to 1st line
        str_LCD("SET MIN : ");               // Display label
        display_LCD(temp);                   // Show current minute value

        cmd_LCD(0xc0);                       // Move cursor to 2nd line
        Arrow_UI();                          // Display arrow controls

        key = Keypad_Scan();                 // Read key from keypad

        if(key=='5') LCD_Control_UI();       // Show help/control UI

        if(key=='2') Increment_Value(&temp,59); // Increase minute (0–59)
        if(key=='8') Decrement_Value(&temp,59); // Decrease minute

        if(key=='C')                         // If cancel key pressed
        {
            flag = 1;                        // Set exit flag
            return;                          // Exit function
        }

        if(key=='=')                         // If save key pressed
        {
            M_min4 = temp;                   // Save value to Medicine 4 minute
            cmd_LCD(0x01);                   // Clear LCD
            return;                          // Exit function
        }

        delay_ms(150);                       // Delay for debounce
    }
}

/*====================================================================
 *                  MEDICINE 5 HOUR EDIT
 *====================================================================*/
void Med5_Hour()                             // Function to edit Medicine 5 hour
{
    unsigned char key;                       // Variable to store keypad input
    unsigned int temp = M_H5;                // Copy current hour into temp

    cmd_LCD(0x01);                           // Clear LCD screen
    str_LCD("SET HOUR :      ");             // Display heading

    while(1)                                 // Infinite loop
    {
        if(flag) return;                     // Exit if flag is set

        cmd_LCD(0x80);                       // Move cursor to 1st line
        str_LCD("SET HOUR : ");              // Display label
        display_LCD(temp);                   // Show current hour value

        cmd_LCD(0xc0);                       // Move cursor to 2nd line
        Arrow_UI();                          // Display arrow controls

        key = Keypad_Scan();                 // Read key from keypad

        if(key=='5') LCD_Control_UI();       // Show help/control UI

        if(key=='2') Increment_Value(&temp,23); // Increase hour (0–23)
        if(key=='8') Decrement_Value(&temp,23); // Decrease hour

        if(key=='C')                         // If cancel key pressed
        {
            flag = 1;                        // Set exit flag
            return;                          // Exit function
        }

        if(key=='=')                         // If save key pressed
        {
            M_H5 = temp;                     // Save value to Medicine 5 hour
            cmd_LCD(0x01);                   // Clear LCD
            return;                          // Exit function
        }

        delay_ms(150);                       // Delay for debounce
    }
}


/*====================================================================
 *                  MEDICINE 5 MINUTE EDIT
 *====================================================================*/
void Med5_Min()                              // Function to edit Medicine 5 minute
{
    unsigned char key;                       // Variable to store keypad input
    unsigned int temp = M_min5;              // Copy current minute into temp

    cmd_LCD(0x01);                           // Clear LCD screen
    str_LCD("SET MIN :       ");             // Display heading

    while(1)                                 // Infinite loop
    {
        if(flag) return;                     // Exit if flag is set

        cmd_LCD(0x80);                       // Move cursor to 1st line
        str_LCD("SET MIN : ");               // Display label
        display_LCD(temp);                   // Show current minute value

        cmd_LCD(0xc0);                       // Move cursor to 2nd line
        Arrow_UI();                          // Display arrow controls

        key = Keypad_Scan();                 // Read key from keypad

        if(key=='5') LCD_Control_UI();       // Show help/control UI

        if(key=='2') Increment_Value(&temp,59); // Increase minute (0–59)
        if(key=='8') Decrement_Value(&temp,59); // Decrease minute

        if(key=='C')                         // If cancel key pressed
        {
            flag = 1;                        // Set exit flag
            return;                          // Exit function
        }

        if(key=='=')                         // If save key pressed
        {
            M_min5 = temp;                   // Save value to Medicine 5 minute
            cmd_LCD(0x01);                   // Clear LCD
            return;                          // Exit function
        }

        delay_ms(150);                       // Delay for debounce
    }
}

/*====================================================================
 *                  RTC TIME MENU
 *====================================================================*/
void Edit_RTC_Time()                        // Function to edit RTC time menu
{
    unsigned char key;                      // Store keypad input
    unsigned int menu = 0;                  // Menu index (0–3)

    cmd_LCD(0x01);                          // Clear LCD
    str_LCD("  EDIT RTC TIME ");            // Show heading

    while(1)                                // Infinite loop
    {
        if(flag) return;                    // Exit if flag set

        Arrow_Init();                       // Load arrow symbols

        cmd_LCD(0x80);                      // Move cursor to first line
        str_LCD("  EDIT RTC TIME ");        // Display heading

        if(menu==0){cmd_LCD(0xc0);str_LCD("      HOUR     ");char_LCD(0);}   // Hour option
        if(menu==1){cmd_LCD(0xc0);char_LCD(1);str_LCD("      MIN      ");char_LCD(0);} // Minute option
        if(menu==2){cmd_LCD(0xc0);char_LCD(1);str_LCD("      SEC      ");char_LCD(0);} // Second option
        if(menu==3){cmd_LCD(0xc0);char_LCD(1);str_LCD("     BACK      ");char_LCD(0);} // Back option

        key = Keypad_Scan();                // Read keypad input

        if(key=='5') LCD_Control_UI();      // Show help UI

        if(key=='6') menu++;                // Move down
        if(key=='4') menu--;                // Move up

        if(menu>3) menu=0;                  // Wrap forward

        if(key=='=')                        // Select option
        {
            if(menu==0) Hour();             // Call Hour edit
            if(menu==1) Min();              // Call Minute edit
            if(menu==2) Sec();              // Call Second edit
            if(menu==3) return;             // Exit menu
        }

        if(key=='C')                        // Cancel key
        {
            flag = 1;                       // Set flag
            return;                         // Exit
        }
    }
}


/*====================================================================
 *                  RTC DATE MENU
 *====================================================================*/
void Edit_RTC_Date()                        // Function to edit RTC date menu
{
    unsigned char key;                      // Key input
    unsigned int menu = 0;                  // Menu index

    cmd_LCD(0x01);                          // Clear LCD
    str_LCD("  EDIT RTC DATE ");            // Heading

    while(1)
    {
        if(flag) return;                    // Exit if flag

        Arrow_Init();                       // Load arrows

        cmd_LCD(0x80);
        str_LCD("  EDIT RTC DATE ");

        if(menu==0){cmd_LCD(0xC0);str_LCD("       DOM      ");char_LCD(0);}   // Date
        if(menu==1){cmd_LCD(0xC0);char_LCD(1);str_LCD("     MONTH     ");char_LCD(0);} // Month
        if(menu==2){cmd_LCD(0xC0);char_LCD(1);str_LCD("     YEAR      ");char_LCD(0);} // Year
        if(menu==3){cmd_LCD(0xC0);char_LCD(1);str_LCD("     BACK      ");char_LCD(0);} // Back

        key = Keypad_Scan();                // Read key

        if(key=='5') LCD_Control_UI();      // Help

        if(key=='6') menu++;                // Down
        if(key=='4') menu--;                // Up

        if(menu>3) menu=0;                  // Wrap

        if(key=='=')                        // Select
        {
            if(menu==0) Day_OM();           // Edit date
            if(menu==1) Month();            // Edit month
            if(menu==2) Year();             // Edit year
            if(menu==3) return;             // Back
        }

        if(key=='C')                        // Cancel
        {
            flag = 1;
            return;
        }
    }
}


/*====================================================================
 *                  RTC DAY MENU
 *====================================================================*/
void Edit_RTC_Day()                         // Function to edit RTC day
{
    unsigned char key;                      // Key input
    unsigned int menu = 0;                  // Menu index (0–1)

    cmd_LCD(0x01);                          // Clear LCD
    str_LCD("  EDIT RTC DAY ");             // Heading

    while(1)
    {
        if(flag) return;                    // Exit if flag

        cmd_LCD(0x80);
        str_LCD("  EDIT RTC DAY ");

        Arrow_Init();                       // Load arrows

        if(menu==0){cmd_LCD(0xC0);str_LCD("       DAY      ");char_LCD(0);} // Day
        if(menu==1){cmd_LCD(0xC0);char_LCD(1);str_LCD("     BACK      ");char_LCD(0);} // Back

        key = Keypad_Scan();                // Read key

        if(key=='5') LCD_Control_UI();      // Help

        if(key=='6') menu++;                // Down
        if(key=='4') menu--;                // Up

        if(menu>1) menu=0;                  // Wrap

        if(key=='=')                        // Select
        {
            if(menu==0) Day();              // Edit day
            if(menu==1) return;             // Back
        }

        if(key=='C')                        // Cancel
        {
            flag = 1;
            return;
        }
    }
}

/*====================================================================
 *                  MEDICINE 1 TIME MENU
 *====================================================================*/
void Edit_Med1_Time()                        // Function to edit MED1 time
{
    unsigned char key;                      // Store keypad input
    unsigned int menu = 0;                  // Menu index (0–2)

    cmd_LCD(0x01);                          // Clear LCD
    str_LCD(" EDIT MED 1 TIME");            // Show heading
	
    while(1)                                // Infinite loop
    {
        if(flag) return;                    // Exit if flag set

        cmd_LCD(0x80);                      // Move to first line
        str_LCD(" EDIT MED 1 TIME");        // Display heading

        cmd_LCD(0xC0);                      // Move to second line
        Arrow_Init();                       // Load arrow symbols

        /*---------------- MENU OPTIONS ----------------*/
        if(menu==0){cmd_LCD(0xc0);str_LCD("      HOUR     ");char_LCD(0);} // Hour option
        if(menu==1){cmd_LCD(0xc0);char_LCD(1);str_LCD("      MIN      ");char_LCD(0);} // Minute option
        if(menu==2){cmd_LCD(0xc0);char_LCD(1);str_LCD("     BACK      ");char_LCD(0);} // Back option

        key = Keypad_Scan();                // Read keypad input

        if(key=='5') LCD_Control_UI();      // Show help UI

        if(key=='6') menu++;                // Move forward in menu
        if(key=='4') menu--;                // Move backward

        if(menu>2) menu=0;                  // Wrap menu (0–2)

        /*---------------- SELECT OPTION ----------------*/
        if(key=='=')
        {
            if(menu==0) Med1_Hour();        // Go to MED1 hour edit
            if(menu==1) Med1_Min();         // Go to MED1 minute edit
            if(menu==2) return;             // Back to previous menu
        }

        /*---------------- EXIT CONTROL ----------------*/
        if(key=='C')                        // Cancel key
        {
            flag=1;                         // Set flag
            if(flag || (menu==2)) return;   // Exit if on BACK
        }
    }
}


/*====================================================================
 *                  MEDICINE 2 TIME MENU
 *====================================================================*/
void Edit_Med2_Time()                        // Function to edit MED2 time
{
    unsigned char key;                      // Store keypad input
    unsigned int menu = 0;                  // Menu index (0–2)

    cmd_LCD(0x01);                          // Clear LCD
    str_LCD(" EDIT MED 2 TIME");            // Show heading
	
    while(1)                                // Infinite loop
    {
        if(flag) return;                    // Exit if flag set

        cmd_LCD(0x80);                      // Move to first line
        str_LCD(" EDIT MED 2 TIME");        // Display heading

        cmd_LCD(0xC0);                      // Move to second line
        Arrow_Init();                       // Load arrow symbols

        /*---------------- MENU OPTIONS ----------------*/
        if(menu==0){cmd_LCD(0xc0);str_LCD("      HOUR     ");char_LCD(0);} // Hour option
        if(menu==1){cmd_LCD(0xc0);char_LCD(1);str_LCD("      MIN      ");char_LCD(0);} // Minute option
        if(menu==2){cmd_LCD(0xc0);char_LCD(1);str_LCD("     BACK      ");char_LCD(0);} // Back option

        key = Keypad_Scan();                // Read keypad input

        if(key=='5') LCD_Control_UI();      // Show help UI

        if(key=='6') menu++;                // Move forward
        if(key=='4') menu--;                // Move backward

        if(menu>2) menu=0;                  // Wrap menu

        /*---------------- SELECT OPTION ----------------*/
        if(key=='=')
        {
            if(menu==0) Med2_Hour();        // Edit MED2 hour
            if(menu==1) Med2_Min();         // Edit MED2 minute
            if(menu==2) return;             // Back
        }

        /*---------------- EXIT CONTROL ----------------*/
        if(key=='C')                        // Cancel key
        {
            flag=1;                         // Set flag
            if(flag || (menu==2)) return;   // Exit if on BACK
        }
    }
}

/*====================================================================
 *                  MEDICINE 3 TIME MENU
 *====================================================================*/
void Edit_Med3_Time()                         // Function to edit Medicine 3 time
{
    unsigned char key;                        // Variable to store keypad input
    unsigned int menu = 0;                    // Menu index (0=Hour,1=Min,2=Back)

    cmd_LCD(0x01);                            // Clear LCD screen
    str_LCD(" EDIT MED 3 TIME");              // Display heading
	
    while(1)                                  // Infinite loop until exit
    {
        if(flag) return;                      // Exit if interrupt flag set

        cmd_LCD(0x80);                        // Move cursor to first line
        str_LCD(" EDIT MED 3 TIME");          // Show heading again (refresh)

        cmd_LCD(0xC0);                        // Move cursor to second line
        Arrow_Init();                         // Load arrow symbols for UI

        if(menu==0){cmd_LCD(0xc0);str_LCD("      HOUR     ");char_LCD(0);} // Show HOUR option
        if(menu==1){cmd_LCD(0xc0);char_LCD(1);str_LCD("      MIN      ");char_LCD(0);} // Show MIN option
        if(menu==2){cmd_LCD(0xc0);char_LCD(1);str_LCD("     BACK      ");char_LCD(0);} // Show BACK option

        key = Keypad_Scan();                  // Read keypad input

        if(key=='5') LCD_Control_UI();        // Show help/control screen

        if(key=='6') menu++;                  // Move forward in menu
        if(key=='4') menu--;                  // Move backward in menu

        if(menu>2) menu=0;                    // Wrap menu (cycle back to 0)

        if(key=='=')                          // If select key pressed
        {
            if(menu==0) Med3_Hour();          // Go to hour edit
            if(menu==1) Med3_Min();           // Go to minute edit
            if(menu==2) return;               // Exit (Back)
        }

        if(key=='C')                          // Cancel key
        {
            flag=1;                           // Set exit flag
            if(flag || (menu==2)) return;     // Exit if BACK or cancel
        }
    }
}


/*====================================================================
 *                  MEDICINE 4 TIME MENU
 *====================================================================*/
void Edit_Med4_Time()                         // Function to edit Medicine 4 time
{
    unsigned char key;                        // Store keypad input
    unsigned int menu = 0;                    // Menu index

    cmd_LCD(0x01);                            // Clear LCD
    str_LCD(" EDIT MED 4 TIME");              // Display heading
	
    while(1)                                  // Infinite loop
    {
        if(flag) return;                      // Exit if flag set

        cmd_LCD(0x80);                        // First line
        str_LCD(" EDIT MED 4 TIME");          // Show heading

        cmd_LCD(0xC0);                        // Second line
        Arrow_Init();                         // Load arrows

        if(menu==0){cmd_LCD(0xc0);str_LCD("      HOUR     ");char_LCD(0);} // Hour option
        if(menu==1){cmd_LCD(0xc0);char_LCD(1);str_LCD("      MIN      ");char_LCD(0);} // Minute option
        if(menu==2){cmd_LCD(0xc0);char_LCD(1);str_LCD("     BACK      ");char_LCD(0);} // Back option

        key = Keypad_Scan();                  // Read key

        if(key=='5') LCD_Control_UI();        // Help UI

        if(key=='6') menu++;                  // Next option
        if(key=='4') menu--;                  // Previous option

        if(menu>2) menu=0;                    // Wrap menu

        if(key=='=')                          // Select key
        {
            if(menu==0) Med4_Hour();          // Edit hour
            if(menu==1) Med4_Min();           // Edit minute
            if(menu==2) return;               // Back
        }

        if(key=='C')                          // Cancel key
        {
            flag=1;                           // Set flag
            if(flag || (menu==2)) return;     // Exit
        }
    }
}


/*====================================================================
 *                  MEDICINE 5 TIME MENU
 *====================================================================*/
void Edit_Med5_Time()                         // Function to edit Medicine 5 time
{
    unsigned char key;                        // Store keypad input
    unsigned int menu = 0;                    // Menu index

    cmd_LCD(0x01);                            // Clear LCD
    str_LCD(" EDIT MED 5 TIME");              // Display heading
	
    while(1)                                  // Infinite loop
    {
        if(flag) return;                      // Exit if flag set

        cmd_LCD(0x80);                        // First line
        str_LCD(" EDIT MED 5 TIME");          // Show heading

        cmd_LCD(0xC0);                        // Second line
        Arrow_Init();                         // Load arrows

        if(menu==0){cmd_LCD(0xc0);str_LCD("      HOUR     ");char_LCD(0);} // Hour
        if(menu==1){cmd_LCD(0xc0);char_LCD(1);str_LCD("      MIN      ");char_LCD(0);} // Minute
        if(menu==2){cmd_LCD(0xc0);char_LCD(1);str_LCD("     BACK      ");char_LCD(0);} // Back

        key = Keypad_Scan();                  // Read keypad

        if(key=='5') LCD_Control_UI();        // Help UI

        if(key=='6') menu++;                  // Move next
        if(key=='4') menu--;                  // Move previous

        if(menu>2) menu=0;                    // Wrap menu

        if(key=='=')                          // Select option
        {
            if(menu==0) Med5_Hour();          // Edit hour
            if(menu==1) Med5_Min();           // Edit minute
            if(menu==2) return;               // Back
        }

        if(key=='C')                          // Cancel
        {
            flag=1;                           // Set flag
            if(flag || (menu==2)) return;     // Exit
        }
    }
}

/*====================================================================
 *                      EDIT MENU FUNCTION
 *====================================================================*/
void Edit_menu()                                // Main control menu of system
{
    unsigned char key;                          // Stores keypad input
    int menu = 0;                               // Menu index (0–9 navigation)

    cmd_LCD(0x01);                              // Clear LCD screen before starting

    while(1)                                    // Infinite loop to keep menu active
    {	
        if(flag)                                // If interrupt flag triggered
        {
            flag = 0;                           // Reset flag
            return;                             // Exit menu immediately
        }

        Arrow_Init();                           // Load custom arrow symbols (UI navigation)

        /*==================== RTC TIME DISPLAY ====================*/
        if(menu==0)                             // Menu option 0 ? RTC Time
        {
            cmd_LCD(0x80);                      // Move cursor to first line
            str_LCD("    RTC TIME   ");         // Display title
            char_LCD(0);                        // Show arrow indicator

            cmd_LCD(0xc0);                      // Move to second line
            str_LCD("    ");                    // Padding
            DisplayRTCTime(hour,min,sec);       // Display live RTC time
            str_LCD("    ");                    // Padding
        }

        /*==================== RTC DATE DISPLAY ====================*/
        if(menu==1)                             // Menu option 1 ? RTC Date
        {
            cmd_LCD(0x80);
            char_LCD(1);                        // Left arrow
            str_LCD("   RTC DATE   ");
            char_LCD(0);                        // Right arrow

            cmd_LCD(0xc0);
            str_LCD("   ");
            DisplayRTCDate(date,month,year);    // Show date (DD/MM/YYYY)
            str_LCD("   ");
        }

        /*==================== RTC DAY DISPLAY ====================*/
        if(menu==2)                             // Menu option 2 ? RTC Day
        {
            cmd_LCD(0x80);
            char_LCD(1);
            str_LCD("   RTC DAY    ");
            char_LCD(0);

            cmd_LCD(0xc0);
            str_LCD("       ");
            DisplayRTCDay(day);                 // Show weekday (SUN, MON…)
            str_LCD("      ");
        }

        /*==================== MEDICINE TIME DISPLAY ====================*/
        if(menu==3) {cmd_LCD(0x80);char_LCD(1);str_LCD("  MED 1 TIME  ");char_LCD(0);DisplayMED1();} // MED1 schedule
        if(menu==4) {cmd_LCD(0x80);char_LCD(1);str_LCD("  MED 2 TIME  ");char_LCD(0);DisplayMED2();} // MED2 schedule
        if(menu==5) {cmd_LCD(0x80);char_LCD(1);str_LCD("  MED 3 TIME  ");char_LCD(0);DisplayMED3();} // MED3 schedule
        if(menu==6) {cmd_LCD(0x80);char_LCD(1);str_LCD("  MED 4 TIME  ");char_LCD(0);DisplayMED4();} // MED4 (optional)
        if(menu==7) {cmd_LCD(0x80);char_LCD(1);str_LCD("  MED 5 TIME  ");char_LCD(0);DisplayMED5();} // MED5 (optional)

        /*==================== ADD MEDICINE SLOT ====================*/
        if(menu==8)                             // Option to dynamically add MED4 & MED5
        {
            cmd_LCD(0x80);
            char_LCD(1);
            str_LCD(" ADD MED SLOT ");          // Title
            char_LCD(0);

            if(cnt<2)                           // Only allow 2 extra medicines
            {
                cmd_LCD(0xC0);
                str_LCD(" PRESS = TO ADD  ");   // Prompt user
            }

            key = Keypad_Scan();                // Read input

            if(key=='5') LCD_Control_UI();      // Show help UI

            if(key=='=' && cnt < 2)             // If user confirms adding slot
            {
                cnt++;                          // Increase slot count

                cmd_LCD(0x01);
                cmd_LCD(0x80);
                str_LCD(" SLOT ADDED ");
                cmd_LCD(0xC0);
                str_LCD(" SUCCESSFULLY ");
                delay_ms(1000);                 // Show confirmation

                if(cnt==1)                      // First extra slot ? MED4
                {
                    med4 = 1;                   // Enable MED4
                    menu = 6;                   // Jump to MED4 menu
                    Edit_Med4_Time();           // Open MED4 edit
                }
                else if(cnt==2)                 // Second extra slot ? MED5
                {
                    med5 = 1;                   // Enable MED5
                    menu = 7;

                    if(cnt >= 2)                // Show full message
                    {
                        cmd_LCD(0x80);
                        str_LCD("  ALL MEDICINE  ");
                        cmd_LCD(0xC0);
                        str_LCD(" SLOTS ARE FULL ");
                        delay_s(1);
                    }

                    Edit_Med5_Time();           // Open MED5 edit
                }
            }

            /* Navigation inside ADD SLOT */
            if(key=='6')                       // Next
            {
                menu++;
                if(med4 && med5 && menu==8) menu=9; // Skip if full
                if(menu>9) menu=0;
            }

            if(key=='4')                       // Previous
            {
                if(menu==0) menu=9;
                else menu--;

                // Skip unavailable options dynamically
                if(!med4 && !med5 && (menu==6 || menu==7)) menu=5;
                if(med4 && !med5 && menu==7) menu=6;
                if(med4 && med5 && menu==8) menu=7;
            }
        }

        /*==================== EXIT OPTION ====================*/
        if(menu==9)                             // Final option
        {
            cmd_LCD(0x80);
            char_LCD(1);
            str_LCD("     EXIT     ");
            char_LCD(0);

            cmd_LCD(0xc0);
            str_LCD("  PRESS C EXIT  ");       // Instruction to exit
        }

        /*==================== GLOBAL KEYPAD CONTROL ====================*/
        key = Keypad_Scan();                   // Read keypad

        if(key=='5') LCD_Control_UI();         // Help screen

        if(key=='6')                           // Move forward
        {
            menu++;

            // Skip unavailable menus
            if(!med4 && !med5 && (menu==6 || menu==7)) menu=8;
            if(med4 && !med5 && menu==7) menu=8;
            if(med4 && med5 && menu==8) menu=9;

            if(menu>9) menu=0;
        }

        if(key=='4')                           // Move backward
        {
            menu--;

            if(menu<0) menu=9;

            // Skip unavailable menus
            if(!med4 && !med5 && (menu==7 || menu==6)) menu=5;
            if(med4 && !med5 && menu==7) menu=6;
            if(med4 && med5 && menu==8) menu=7;
        }

        /*==================== SELECT OPTION ====================*/
        if(key=='=')                           // Enter selected menu
        {
            if(menu==0) Edit_RTC_Time();       // Go to RTC Time edit
            if(menu==1) Edit_RTC_Date();       // Go to RTC Date edit
            if(menu==2) Edit_RTC_Day();        // Go to RTC Day edit
            if(menu==3) Edit_Med1_Time();      // MED1 edit
            if(menu==4) Edit_Med2_Time();      // MED2 edit
            if(menu==5) Edit_Med3_Time();      // MED3 edit
            if(menu==6) Edit_Med4_Time();      // MED4 edit
            if(menu==7) Edit_Med5_Time();      // MED5 edit
        }

        /*==================== EXIT CONTROL ====================*/
        if(key=='C')                           // Cancel key
        {
            flag = 1;                          // Set flag

            if(flag || menu==9)                // Exit if on EXIT menu
            {
                flag = 0;                      // Reset flag
                return;                        // Exit function
            }
        }
    }
}

/*====================================================================
 *                  SHOW NEXT MEDICINE FUNCTION
 *====================================================================*/
void Show_Next_Med()                         // Function to display next upcoming medicine time
{
    int cur = HOUR*60 + MIN;                 // Current time in minutes

    int t[5], diff[5];                       // t[] = med times, diff[] = differences
    int i, min_index=-1, min_val=10000;      // Track nearest medicine

    /*-------------------- STORE MED TIMES --------------------*/
    t[0] = M_H1*60 + M_min1;                 // MED1
    t[1] = M_H2*60 + M_min2;                 // MED2
    t[2] = M_H3*60 + M_min3;                 // MED3
    t[3] = med4 ? (M_H4*60 + M_min4) : 10000;// MED4 (if enabled)
    t[4] = med5 ? (M_H5*60 + M_min5) : 10000;// MED5 (if enabled)

    /*-------------------- CALCULATE DIFFERENCE --------------------*/
    for(i=0;i<5;i++)
    {
        if(t[i]==cur) diff[i]=10000;         // Ignore current time
        else if(t[i]>cur) diff[i]=t[i]-cur;  // Same day
        else diff[i]=(1440-cur)+t[i];        // Next day
    }

    /*-------------------- FIND MINIMUM --------------------*/
    for(i=0;i<5;i++)
    {
        if(diff[i]<min_val)
        {
            min_val = diff[i];
            min_index = i;
        }
    }

    /*-------------------- DISPLAY RESULT --------------------*/
    cmd_LCD(0xC0);                           // Move to second line
    str_LCD(" NEXT MED ");                   // Label

    switch(min_index)
    {
        case 0: display_LCD(M_H1); char_LCD(':'); display_LCD(M_min1); break; // MED1
        case 1: display_LCD(M_H2); char_LCD(':'); display_LCD(M_min2); break; // MED2
        case 2: display_LCD(M_H3); char_LCD(':'); display_LCD(M_min3); break; // MED3
        case 3: display_LCD(M_H4); char_LCD(':'); display_LCD(M_min4); break; // MED4
        case 4: display_LCD(M_H5); char_LCD(':'); display_LCD(M_min5); break; // MED5
    }
}


/*====================================================================
 *                      ALERT FUNCTION
 *====================================================================*/
void Alert()                                   // Function to check and trigger medicine alerts
{
    IODIR0 |= BUZZ;                            // Configure buzzer pin as output

    /*-------------------- MEDICINE 1 --------------------*/
    if(M_H1==HOUR && M_min1==MIN && SEC<=1)    // Check if MED1 time matches RTC
    {
        int i;                                 // Loop variable

        cmd_LCD(0x01);                         // Clear LCD
        str_LCD(" TAKE MEDICINE 1");           // Show message
        cmd_LCD(0xC0);                         // Move to second line
        str_LCD(" PRESS SW2 STOP");            // Instruction

        IOSET0 = BUZZ;                         // Turn ON buzzer

        for(i=0;i<600 && !f2;i++)              // Wait until stop button or timeout
            delay_ms(100);                     // Delay 100ms

        IOCLR0 = BUZZ;                         // Turn OFF buzzer
        f2 = 0;                                // Reset stop flag

        if(i==600)                             // If timeout (medicine missed)
        {
            cmd_LCD(0x01);
            str_LCD("MEDICINE MISSED!");
            cmd_LCD(0xc0);
            Show_Next_Med();                   // Show next medicine
            delay_ms(1500);
        }
        else                                   // If user pressed stop (taken)
        {
            cmd_LCD(0x01);
            str_LCD("MEDICINE 1 TAKEN");

            Show_Next_Med();                   // Show next medicine

            delay_ms(1500);
            cmd_LCD(0x01);
        }
    }

    /*-------------------- MEDICINE 2 --------------------*/
    if(M_H2==HOUR && M_min2==MIN && SEC<=1)    // Check MED2 time
    {
        int i;

        cmd_LCD(0x01);
        str_LCD(" TAKE MEDICINE 2");
        cmd_LCD(0xC0);
        str_LCD(" PRESS SW2 STOP");

        IOSET0 = BUZZ;                         // Buzzer ON

        for(i=0;i<600 && !f2;i++)
            delay_ms(100);

        IOCLR0 = BUZZ;                         // Buzzer OFF
        f2 = 0;

        if(i==600)                             // Missed
        {
            cmd_LCD(0x01);
            str_LCD("MEDICINE MISSED!");
            cmd_LCD(0xc0);
            Show_Next_Med();
            delay_ms(1500);
        }
        else                                   // Taken
        {
            cmd_LCD(0x01);
            str_LCD("MEDICINE 2 TAKEN");

            Show_Next_Med();

            delay_ms(1500);
            cmd_LCD(0x01);
        }
    }

    /*-------------------- MEDICINE 3 --------------------*/
    if(M_H3==HOUR && M_min3==MIN && SEC<=1)    // Check MED3 time
    {
        int i;

        cmd_LCD(0x01);
        str_LCD(" TAKE MEDICINE 3");
        cmd_LCD(0xC0);
        str_LCD(" PRESS SW2 STOP");

        IOSET0 = BUZZ;                         // Buzzer ON

        for(i=0;i<600 && !f2;i++)              // Loop with delay
        { 
            delay_ms(100);
            IOCLR0 = BUZZ;                     // Turn OFF buzzer (inside loop)
            f2 = 0;                            // Reset flag
        }

        if(i==600)                             // Missed
        {
            cmd_LCD(0x01);
            str_LCD("MEDICINE MISSED!");
            cmd_LCD(0xc0);
            Show_Next_Med();
            delay_ms(1500);
        }
        else                                   // Taken
        {
            cmd_LCD(0x01);
            str_LCD("MEDICINE 3 TAKEN");

            Show_Next_Med();

            delay_ms(1500);
            cmd_LCD(0x01);
        }
    }

    /*-------------------- MEDICINE 4 --------------------*/
    if(med4 && M_H4==HOUR && M_min4==MIN && SEC<=1) // Check MED4 (enabled + time match)
    {
        int i;

        cmd_LCD(0x01);
        str_LCD(" TAKE MEDICINE 4");
        cmd_LCD(0xC0);
        str_LCD(" PRESS SW2 STOP");

        IOSET0 = BUZZ;

        for(i=0;i<600 && !f2;i++)
            delay_ms(100);

        IOCLR0 = BUZZ;
        f2 = 0;

        if(i==600)                             // Missed
        {
            cmd_LCD(0x01);
            str_LCD("MEDICINE MISSED!");
            cmd_LCD(0xc0);
            Show_Next_Med();
            delay_ms(1500);
        }
        else                                   // Taken
        {
            cmd_LCD(0x01);
            str_LCD("MEDICINE 4 TAKEN");

            Show_Next_Med();

            delay_ms(1500);
            cmd_LCD(0x01);
        }
    }

    /*-------------------- MEDICINE 5 --------------------*/
    if(med5 && M_H5==HOUR && M_min5==MIN && SEC<=1) // Check MED5
    {
        int i;

        cmd_LCD(0x01);
        str_LCD(" TAKE MEDICINE 5");
        cmd_LCD(0xC0);
        str_LCD(" PRESS SW2 STOP");

        IOSET0 = BUZZ;

        for(i=0;i<600 && !f2;i++)
            delay_ms(100);

        IOCLR0 = BUZZ;
        f2 = 0;

        if(i==600)                             // Missed
        {
            cmd_LCD(0x01);
            str_LCD("MEDICINE MISSED!");
            cmd_LCD(0xc0);
            Show_Next_Med();
            delay_ms(1500);
        }
        else                                   // Taken
        {
            cmd_LCD(0x01);
            str_LCD("MEDICINE 5 TAKEN");

            Show_Next_Med();

            delay_ms(1500);
            cmd_LCD(0x01);
        }
    }
}

/*====================================================================
 *              DISPLAY MEDICINE TIMES (MED1 to MED5)
 *====================================================================*/

/*-------------------- MEDICINE 1 DISPLAY --------------------*/
void Display_MED1()                           // Function to display Medicine 1 time
{
    Arrow_Init();                            // Initialize custom arrow symbols in LCD

    cmd_LCD(0x80);                           // Set cursor to first line (address 0x80)
    str_LCD(" MEDICINE 1 TIME");             // Print title on first line

    cmd_LCD(0xC0);                           // Move cursor to second line (address 0xC0)
    char_LCD(1);                             // Print left arrow symbol (custom char index 1)
    str_LCD("   ");                          // Add spacing after arrow

    display_LCD(M_H1);                       // Display Medicine 1 hour value
    char_LCD(':');                           // Print ':' separator

    display_LCD(M_min1);                     // Display Medicine 1 minute value
    char_LCD(':');                           // Print ':' separator

    str_LCD("00   ");                        // Display fixed seconds "00" and spacing
    char_LCD(0);                             // Print right arrow symbol (custom char index 0)
}


/*-------------------- MEDICINE 2 DISPLAY --------------------*/
void Display_MED2()                           // Function to display Medicine 2 time
{
    Arrow_Init();                            // Load arrow symbols

    cmd_LCD(0x80);                           // First line
    str_LCD(" MEDICINE 2 TIME");             // Title

    cmd_LCD(0xC0);                           // Second line
    char_LCD(1);                             // Left arrow
    str_LCD("   ");                          // Spacing

    display_LCD(M_H2);                       // Hour
    char_LCD(':');                           // Separator

    display_LCD(M_min2);                     // Minute
    char_LCD(':');                           // Separator

    str_LCD("00   ");                        // Seconds fixed
    char_LCD(0);                             // Right arrow
}


/*-------------------- MEDICINE 3 DISPLAY --------------------*/
void Display_MED3()                           // Function to display Medicine 3 time
{
    Arrow_Init();                            // Initialize arrows

    cmd_LCD(0x80);                           // First line
    str_LCD(" MEDICINE 3 TIME");             // Title

    cmd_LCD(0xC0);                           // Second line
    char_LCD(1);                             // Left arrow
    str_LCD("   ");                          // Spacing

    display_LCD(M_H3);                       // Hour
    char_LCD(':');                           // Separator

    display_LCD(M_min3);                     // Minute
    char_LCD(':');                           // Separator

    str_LCD("00   ");                        // Seconds
    char_LCD(0);                             // Right arrow
}


/*-------------------- MEDICINE 4 DISPLAY --------------------*/
void Display_MED4()                           // Function to display Medicine 4 time
{
    Arrow_Init();                            // Load arrow symbols

    cmd_LCD(0x80);                           // First line
    str_LCD(" MEDICINE 4 TIME");             // Title

    cmd_LCD(0xC0);                           // Second line
    char_LCD(1);                             // Left arrow
    str_LCD("   ");                          // Spacing

    display_LCD(M_H4);                       // Hour
    char_LCD(':');                           // Separator

    display_LCD(M_min4);                     // Minute
    char_LCD(':');                           // Separator

    str_LCD("00   ");                        // Seconds
    char_LCD(0);                             // Right arrow
}


/*-------------------- MEDICINE 5 DISPLAY --------------------*/
void Display_MED5()                           // Function to display Medicine 5 time
{
    Arrow_Init();                            // Initialize arrows

    cmd_LCD(0x80);                           // First line
    str_LCD(" MEDICINE 5 TIME");             // Title

    cmd_LCD(0xC0);                           // Second line
    char_LCD(1);                             // Left arrow
    str_LCD("   ");                          // Spacing

    display_LCD(M_H5);                       // Hour
    char_LCD(':');                           // Separator

    display_LCD(M_min5);                     // Minute
    char_LCD(':');                           // Separator

    str_LCD("00   ");                        // Seconds fixed
    char_LCD(0);                             // Right arrow
}


/*====================================================================
 *          DISPLAY MAIN MENU (RTC + MEDICINES VIEW)
 *====================================================================*/
void Display_Menu()                           // Function to display main viewing menu
{
    static int menu = 0;                      // Static menu index (retains value across calls)
    unsigned char key;                        // Variable to store keypad input
    int max_menu = 3;                         // Default max menu (RTC + MED1–3)

    /*-------------------- SET MAX MENU LIMIT --------------------*/
    if(med4 && med5)                          // If both MED4 and MED5 are enabled
        max_menu = 5;                         // Total menus = 0 to 5

    else if(med4)                             // If only MED4 is enabled
        max_menu = 4;                         // Total menus = 0 to 4

    else                                      // If only MED1–3 exist
        max_menu = 3;                         // Total menus = 0 to 3


    /*-------------------- DISPLAY BASED ON MENU --------------------*/
    if(menu==0) DisplayRTC();                 // Show RTC (time + date + day)

    if(menu==1) Display_MED1();               // Show Medicine 1 time

    if(menu==2) Display_MED2();               // Show Medicine 2 time

    if(menu==3) Display_MED3();               // Show Medicine 3 time

    if(menu==4 && med4) Display_MED4();       // Show MED4 only if enabled

    if(menu==5 && med5) Display_MED5();       // Show MED5 only if enabled


    /*-------------------- KEYPAD INPUT --------------------*/
    key = Keypad_Scan();                      // Read key from keypad

    if(key=='5') LCD_Control_UI();            // Show help/control UI


    /*-------------------- MENU INCREMENT --------------------*/
    if(key=='6')                              // If next key pressed
    { 
        menu++;                               // Move to next menu

        if(menu > max_menu)                   // If exceeds max limit
            menu = 0;                         // Wrap back to first menu
    }


    /*-------------------- MENU DECREMENT --------------------*/
    if(key=='4')                              // If previous key pressed
    {
        if(menu == 0)                         // If at first menu
            menu = max_menu;                  // Jump to last menu
        else
            menu--;                           // Otherwise move backward
    }
}
