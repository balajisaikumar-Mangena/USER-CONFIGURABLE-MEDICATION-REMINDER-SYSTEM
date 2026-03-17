#include <lpc21xx.h>

extern volatile unsigned char fl,f2;

void EINT0_ISR(void) __irq{
    fl = 1;                        // Edit mode flag
    EXTINT = 1<<0;                 // Clear interrupt flag
    VICVectAddr = 0;               // Acknowledge interrupt
}

void EINT1_ISR(void) __irq{
    f2 = 1;
    EXTINT = 1<<1;
    VICVectAddr = 0;
}

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
