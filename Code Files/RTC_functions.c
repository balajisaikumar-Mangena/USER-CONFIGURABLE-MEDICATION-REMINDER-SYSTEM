#include <lpc21xx.h>              // LPC21xx registers
#include "delay.h"                // Delay functions
#include "lcd.h"                  // LCD functions
#include "KPM.h"                  // Keypad
#include "M_P_func.h"             // Misc functions


/*====================================================================
 *                      LCD PIN DEFINITIONS
 *====================================================================*/
#define DATA (0xFF)               // Data pins
#define RS   (1<<8)               // Register select
#define RW   (1<<9)               // Read/Write
#define EN   (1<<10)              // Enable


/*====================================================================
 *                      RTC CONTROL MACROS
 *====================================================================*/
#define RTC_ENABLE (1<<0)         // Enable RTC
#define RTC_RESET  (1<<1)         // Reset RTC


/*====================================================================
 *                      DAY DEFINITIONS
 *====================================================================*/
#define SUN 0
#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6


/*====================================================================
 *                      GLOBAL VARIABLES
 *====================================================================*/
extern int hour,min,sec;          // Time variables
extern int date,month,year;       // Date variables
extern int day;                   // Day variable

char week[][4]={"SUN","MON","TUE","WED","THU","FRI","SAT"}; // Day names


/*====================================================================
 *                      RTC INITIALIZATION
 *====================================================================*/
void RTC_Init(void)
{
    CCR = RTC_RESET;              // Reset RTC
    PREINT  = 457;                // Set integer divider
    PREFRAC = 25024;              // Set fractional divider
    CCR = RTC_ENABLE;             // Enable RTC
}


/*====================================================================
 *                      TIME FUNCTIONS
 *====================================================================*/
void GetRTCTimeInfo(int *h,int *m,int *s)
{
    *h = HOUR;                    // Read hour
    *m = MIN;                     // Read minute
    *s = SEC;                     // Read second
}

void DisplayRTCTime(unsigned int h,unsigned int m,unsigned int s)
{
    h = HOUR;                     // Get hour
    m = MIN;                      // Get minute
    s = SEC;                      // Get second
    
    char_LCD((h/10)+48);          // Hour
    char_LCD((h%10)+48);
    char_LCD(':');

    char_LCD((m/10)+48);          // Minute
    char_LCD((m%10)+48);
    char_LCD(':');

    char_LCD((s/10)+48);          // Second
    char_LCD((s%10)+48);
}

void SetRTCTimeInfo(unsigned int h,unsigned int m,unsigned int s)
{
    HOUR = h;                     // Set hour
    MIN  = m;                     // Set minute
    SEC  = s;                     // Set second
}


/*====================================================================
 *                      DATE FUNCTIONS
 *====================================================================*/
void GetRTCDateInfo(int *d,int *m,int *y)
{
    *d = DOM;                     // Read date
    *m = MONTH;                   // Read month
    *y = YEAR;                    // Read year
}

void DisplayRTCDate(unsigned int d,unsigned int m,unsigned int y)
{
    d = DOM;                      // Get date
    m = MONTH;                    // Get month
    y = YEAR;                     // Get year

    char_LCD((d/10)+48);
    char_LCD((d%10)+48);
    char_LCD('/');

    char_LCD((m/10)+48);
    char_LCD((m%10)+48);
    char_LCD('/');

    char_LCD((y/1000)%10+48);
    char_LCD((y/100)%10+48);
    char_LCD((y/10)%10+48);
    char_LCD((y%10)+48);
}

void SetRTCDateInfo(unsigned int d,unsigned int m,unsigned int y)
{
    DOM = d;                      // Set date
    MONTH = m;                    // Set month
    YEAR = y;                     // Set year
}


/*====================================================================
 *                      DAY FUNCTIONS
 *====================================================================*/
void GetRTCDay(int *d)
{
    *d = DOW;                     // Read day
}

void DisplayRTCDay(unsigned int d)
{
    d = DOW;                      // Get day
    str_LCD(week[d]);             // Display day
}

void SetRTCDay(unsigned int d)
{
    DOW = d;                      // Set day
}

void Day_OW()
{
    unsigned int temp = DOW;      // Read day
    cmd_LCD(0xC0);                // LCD 2nd line
    str_LCD("   DOW : ");

    if(temp==0) str_LCD("SUN");   // Display day
    if(temp==1) str_LCD("MON");
    if(temp==2) str_LCD("TUE");
    if(temp==3) str_LCD("WED");
    if(temp==4) str_LCD("THU");
    if(temp==5) str_LCD("FRI");
    if(temp==6) str_LCD("SAT");

    str_LCD("    ");
}


/*====================================================================
 *                      MAIN DISPLAY FUNCTION
 *====================================================================*/
void DisplayRTC(void)
{
    Arrow_Init();                 // Init cursor

    GetRTCTimeInfo(&hour,&min,&sec);
    cmd_LCD(0x80);                // Line 1
    str_LCD("TIME  : ");
    DisplayRTCTime(hour,min,sec);

    GetRTCDateInfo(&date,&month,&year);
    cmd_LCD(0xC0);                // Line 2
    DisplayRTCDate(date,month,year);

    GetRTCDay(&day);
    cmd_LCD(0xCA);                // Position
    str_LCD(" ");
    DisplayRTCDay(day);
    str_LCD(" ");
    char_LCD(0);                  // Arrow symbol
}
