#include <lpc21xx.h>
#include "delay.h"
#include "lcd.h"
#include "KPM.h"
#include "RTC.h"
#include "EINT.h"
#include "M_P_func.h"

/*====================================================================
 *                      HARDWARE INTERFACE
 *====================================================================*/

/*
 * LCD Interface (8-bit mode)
 * RS  -> P0.5
 * EN  -> P0.6
 * D0–D7 -> P0.8 – P0.15
 *
 * Keypad Interface
 * Rows -> P1.16 – P1.19
 * Cols -> P1.20 – P1.23
 *
 * Buzzer
 * BUZZ -> P0.16
 *
 * External Interrupt Inputs
 * SW1 -> P0.1   : Enter Edit Mode
 * SW2 -> P0.13  : Stop Buzzer
 */


/*====================================================================
 *                      GLOBAL VARIABLES
 *====================================================================*/

/* 
 * fl  : Set by SW1 interrupt ? triggers Edit Mode
 * f2  : Set by SW2 interrupt ? used to stop buzzer
 * flag: Reserved for application-specific logic
 */
volatile unsigned char f1  = 0;
volatile unsigned char f2  = 0;
unsigned int           flag = 0;


/*====================================================================
 *                      MAIN APPLICATION
 *====================================================================*/

int main(void)
{
    /*---------------- System Initialization ----------------*/
    
    init_LCD();                           /* Initialize LCD module */
    init_KPM();                           /* Initialize Keypad interface */
    RTC_Init();                           /* Initialize Real-Time Clock */
    Interrupt_Init();                     /* Configure External Interrupts */
		__enable_irq();

    /*---------------- Startup Display ----------------*/
    
    MRS();                                /* Display project title / splash screen */

    /*---------------- Default RTC Configuration ----------------*/
    
    SetRTCTimeInfo(10, 00, 50);           /* HH:MM:SS */
    SetRTCDateInfo(03, 04, 2026);         /* DD/MM/YYYY */
    SetRTCDay(5);                         /* Day index (implementation-specific) */


    /*---------------- Main Control Loop ----------------*/
    
    while (1)
    {
        /*------------------------------------------------------
         * Handle Edit Mode Trigger (External Interrupt SW1)
         *-----------------------------------------------------*/
        if (f1 == 1)
        {
            cmd_LCD(0x01);               /* Clear display */

            MENU();                      /* Display configuration menu */
            Edit_menu();                 /* User edits time/date/alert */

            f1 = 0;                     /* Reset flag after handling */

            cmd_LCD(0x01);               /* Refresh display */
        }

        /*------------------------------------------------------
         * Normal Operation Mode
         *-----------------------------------------------------*/
        
        Display_Menu();                  /* Show RTC time, date, and alarm */

        /*------------------------------------------------------
         * Alert Monitoring
         *-----------------------------------------------------*/
        
        Alert();                         /* Activate buzzer on match condition */
    }
}
