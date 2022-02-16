/*
 * File:   main.c
 * Author: Davi
 *
 */


#include <xc.h>

#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config FOSC = HS
#pragma config PWRT = ON       // Power-up Timer Enable bit (PWRT disabled)
#pragma config WDT = OFF         // Watchdog Timer Enable bit (WDT enabled)
#pragma config PBADEN = OFF      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config MCLRE = OFF       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))



#define _XTAL_FREQ 4000000
unsigned short dutyCycle = 0;
unsigned char pct = 0;

void __interrupt() extInt(void){
    if(INTCONbits.INT0IF){
        INTCONbits.INT0IF = 0;
        LATCbits.LC2 = ~LATCbits.LC2;
        if(pct < 100){
            dutyCycle += 100;
            pct+=10;
        }
            
        
    } else if(INTCON3bits.INT1IF){
        INTCON3bits.INT1IF = 0;
        LATCbits.LC6 = ~LATCbits.LC6;
        if(pct > 0){
            dutyCycle -= 100;
            pct -= 10;
        }
        
    }
        
}


void main (void) {

	//configurando o EXTINT
    TRISB = 0x03; //rb0 e rb1 entradas
    INTCONbits.INT0IE = 1;
    INTCON3bits.INT1IE = 1;
    INTCON2bits.nRBPU = 0;
    INTCON2bits.INTEDG0 = 0;

	//configurando o PWM em 2,44 kHz
    PR2=255; 
    CCPR2L=0x00; 
    CCP2CONbits.DC2B=0b00; 
    T2CON=0x06; 
    TRISC=0; 
    CCP2CONbits.CCP2M0 = 0;
    CCP2CONbits.CCP2M1 = 0;
    CCP2CONbits.CCP2M2 = 1;
    CCP2CONbits.CCP2M3 = 1;
    
    //habilitando interrupts
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.INT0IF = 0;
    INTCON3bits.INT1IF = 0;
    

	while(1) {
        // Atualiza o duty cycle do PWM a partir da leitura do ADC
        CCPR2L = (char)((dutyCycle & 0xFFFC) >> 2);
        CCP2CONbits.DC2B = (char)(dutyCycle & 0x03);
    }
}