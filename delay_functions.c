/*====================================================================
 *                  DELAY FUNCTIONS
 *====================================================================*/

void delay_s(unsigned int d_s)          // Delay in seconds
{
    for(d_s *= 12000000; d_s > 0; d_s--) // Multiply for 1 second (CPU cycles loop)
        ;                                 // Empty loop (do nothing)
}

void delay_ms(unsigned int d_ms)        // Delay in milliseconds
{
    for(d_ms *= 12000; d_ms > 0; d_ms--) // Multiply for 1 ms delay
        ;                                 // Empty loop
}

void delay_us(unsigned int d_us)        // Delay in microseconds
{
    for(d_us *= 12; d_us > 0; d_us--)    // Multiply for 1 µs delay
        ;                                 // Empty loop
}
