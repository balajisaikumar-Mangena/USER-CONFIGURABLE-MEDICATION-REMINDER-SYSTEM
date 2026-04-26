/*====================================================================
 *                  EXTERNAL INTERRUPT (EINT) MODULE
 *====================================================================*/

#include <lpc21xx.h>                         // LPC21xx register definitions

extern volatile unsigned char f1, f2;        // Flags updated by interrupts


/*====================================================================
 *                  EINT0 INTERRUPT SERVICE ROUTINE
 *====================================================================*/
void EINT0_ISR(void) __irq                   // ISR for EINT0 (P0.1)
{
    f1 = 1;                                  // Set flag ? enter edit mode
    EXTINT = 1 << 0;                         // Clear EINT0 interrupt flag
    VICVectAddr = 0;                         // Acknowledge interrupt to VIC
}

/*====================================================================
 *                  EINT1 INTERRUPT SERVICE ROUTINE
 *====================================================================*/
void EINT1_ISR(void) __irq                   // ISR for EINT1 (P0.3)
{
    f2 = 1;                                  // Set flag ? stop buzzer
    EXTINT = 1 << 1;                         // Clear EINT1 interrupt flag
    VICVectAddr = 0;                         // End interrupt
}

/*====================================================================
 *                  INTERRUPT INITIALIZATION
 *====================================================================*/

void Interrupt_Init(void){
	
    PINSEL0 &=~(3<<2);             // Clear P0.1 and P0.3
		PINSEL0|=0X0C;
    PINSEL0 &=~(3<<6);	           // P0.1=EINT0 , P0.3=EINT1
		PINSEL0|=0XC0;

		VICIntEnable |= (1<<14) | (1<<15);
		VICVectCntl0 = (1<<5) | 14;
		VICVectCntl1 = (1<<5) | 15;
    VICVectAddr0 = (unsigned int)EINT0_ISR;
    VICVectAddr1 = (unsigned int)EINT1_ISR;   
}
