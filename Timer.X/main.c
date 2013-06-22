// PIC18F14K22 Configuration Bit Settings
#define _XTAL_FREQ 4194304
#include <xc.h>

// CONFIG1H
#pragma config FOSC = HS        // Oscillator Selection bits (external oscillator HS)
#pragma config PLLEN = OFF      // 4 X PLL Enable bit (PLL is under software control)
#pragma config PCLKEN = ON      // Primary Clock Enable bit (Primary clock enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 19        // Brown Out Reset Voltage bits (VBOR set to 1.9 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT is controlled by SWDTEN bit of the WDTCON register)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config HFOFST = ON      // HFINTOSC Fast Start-up bit (HFINTOSC starts clocking the CPU without waiting for the oscillator to stablize.)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled, RA3 input pin disabled)

void Timer1_init(void){
    /* Timer interrupt frequency:
     * f = _XTAL_FREQ / 4*prescaler*Timer1 resolution
     * f = 4194304 / 4*8*65536 = 2Hz */
    // Timer 1 prescaler 1:8
    T1CONbits.T1CKPS1 = 1;
    T1CONbits.T1CKPS0 = 1;
    // Timer 1 overflow interrupt enabled
    PIE1bits.TMR1IE = 1;
    // Timer 1 on
    T1CONbits.TMR1ON = 1;
}

int main(void){

    Timer1_init();

    // set up IO pin
    TRISBbits.RB7 = 0; // output
    LATBbits.LATB7 = 1; // high

    INTCONbits.PEIE = 1; // enable peripheral interrupts
    INTCONbits.GIE = 1;  // global interrupt enable

    while(1);
    return 0;
}

void interrupt ISR(void){
    // check for Timer 1 overflow
    if(PIR1bits.TMR1IF == 1){
        LATBbits.LATB7 ^= 1; // toggle RB7
        PIR1bits.TMR1IF = 0; // reset interrupt flag
    }
}