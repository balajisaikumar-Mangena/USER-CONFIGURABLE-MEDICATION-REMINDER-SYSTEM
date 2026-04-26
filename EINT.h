/*====================================================================
 *              EXTERNAL INTERRUPT DECLARATIONS
 *====================================================================*/

extern volatile unsigned char fl;   // Flag for EINT0 (edit/menu control)
extern volatile unsigned char f2;   // Flag for EINT1 (stop buzzer)

/*-------------------- ISR PROTOTYPES --------------------*/
void EINT0_ISR(void) __irq;         // ISR for External Interrupt 0
void EINT1_ISR(void) __irq;         // ISR for External Interrupt 1

/*-------------------- INITIALIZATION --------------------*/
void Interrupt_Init(void);          // Function to initialize interrupts
