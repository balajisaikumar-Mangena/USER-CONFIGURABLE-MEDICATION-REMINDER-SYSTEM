/*====================================================================
 *                  GLOBAL MEDICINE TIME VARIABLES
 *====================================================================*/

extern volatile unsigned int M_H1;     // MED1 hour
extern volatile unsigned int M_min1;   // MED1 minute
extern volatile unsigned int M_H2;     // MED2 hour
extern volatile unsigned int M_min2;   // MED2 minute
extern volatile unsigned int M_H3;     // MED3 hour
extern volatile unsigned int M_min3;   // MED3 minute
extern volatile unsigned int M_H4;     // MED4 hour (optional)
extern volatile unsigned int M_min4;   // MED4 minute
extern volatile unsigned int M_H5;     // MED5 hour (optional)
extern volatile unsigned int M_min5;   // MED5 minute

extern unsigned int flag;              // Global control flag


/*====================================================================
 *                  DISPLAY FUNCTIONS
 *====================================================================*/

void display_LCD(unsigned int num);    // Display number on LCD

void Display_Menu(void);               // Main display menu
void Display_MED1(void);               // Show MED1 time
void Display_MED2(void);               // Show MED2 time
void Display_MED3(void);               // Show MED3 time
void Display_MED4(void);               // Show MED4 time
void Display_MED5(void);               // Show MED5 time

void DisplayMED1(void);                // Alternate MED1 display
void DisplayMED2(void);                // Alternate MED2 display
void DisplayMED3(void);                // Alternate MED3 display


/*====================================================================
 *                  LCD UI FUNCTIONS
 *====================================================================*/

void Arrow_Init(void);                 // Initialize arrow symbols
void Arrow_Init1(void);                // Initialize up/down arrows
void Arrow_UI(void);                   // Display arrow UI
void LCD_Control_UI(void);             // Show control help screen


/*====================================================================
 *                  VALUE CONTROL FUNCTIONS
 *====================================================================*/

void Increment_Value(unsigned int *value, unsigned int max); // Increment value with limit
void Decrement_Value(unsigned int *value, unsigned int max); // Decrement value with limit


/*====================================================================
 *                  RTC EDIT FUNCTIONS
 *====================================================================*/

void Hour(void);                       // Edit hour
void Min(void);                        // Edit minute
void Sec(void);                        // Edit second
void Date(void);                       // Edit date
void Month(void);                      // Edit month
void Year(void);                       // Edit year
void Day(void);                        // Edit day


/*====================================================================
 *                  MEDICINE EDIT FUNCTIONS
 *====================================================================*/

void Med1_Hour(void);                  // MED1 hour edit
void Med1_Min(void);                   // MED1 minute edit
void Med2_Hour(void);                  // MED2 hour edit
void Med2_Min(void);                   // MED2 minute edit
void Med3_Hour(void);                  // MED3 hour edit
void Med3_Min(void);                   // MED3 minute edit
void Med4_Hour(void);                  // MED4 hour edit
void Med4_Min(void);                   // MED4 minute edit
void Med5_Hour(void);                  // MED5 hour edit
void Med5_Min(void);                   // MED5 minute edit


/*====================================================================
 *                  MENU FUNCTIONS
 *====================================================================*/

void Edit_RTC_Time(void);              // RTC time menu
void Edit_RTC_Date(void);              // RTC date menu
void Edit_RTC_Day(void);               // RTC day menu

void Edit_Med1_Time(void);             // MED1 menu
void Edit_Med2_Time(void);             // MED2 menu
void Edit_Med3_Time(void);             // MED3 menu
void Edit_Med4_Time(void);             // MED4 menu
void Edit_Med5_Time(void);             // MED5 menu


/*====================================================================
 *                  ALERT & LOGIC FUNCTIONS
 *====================================================================*/

void Show_Next_Med(void);              // Display next medicine time
void Alert(void);                      // Trigger alarm/buzzer


/*====================================================================
 *                  SYSTEM MENU FUNCTIONS
 *====================================================================*/

void MENU(void);                       // Main system menu screen
void MRS(void);                        // Welcome/start screen
void Edit_menu(void);                  // Edit control menu
