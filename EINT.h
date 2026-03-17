extern volatile unsigned char fl;
extern volatile unsigned char f2;

void EINT0_ISR(void) __irq;
void EINT1_ISR(void) __irq;

void Interrupt_Init(void);
