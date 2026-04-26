/*====================================================================
 *                  LCD FUNCTION DECLARATIONS
 *====================================================================*/

void cmd_LCD(unsigned char cmd);      // Send command to LCD
void char_LCD(unsigned char ch);      // Send single character to LCD
void init_LCD(void);                  // Initialize LCD (8-bit mode)
void str_LCD(char *str);              // Display string on LCD
void init_LCD_4(void);                // Alternate LCD initialization
void num_LCD(int n);                  // Display integer number
