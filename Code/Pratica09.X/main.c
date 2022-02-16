/*
 * File:   main.c
 * Author: Davi
 *
 */


#include <xc.h>

#pragma config CPUDIV = OSC1_PLL2;
#pragma config FOSC = HS;
#pragma config WDT = OFF;
#pragma config PBADEN = OFF;
#pragma config LVP = OFF;
#pragma config MCLRE = ON;
#pragma config PWRT = ON;
#pragma config XINST=0

short dutyCycle = 0;


void interrupt extInt(void){
    if(INTCONbits.INT0IF){
        INTCONbits.INT0IF = 0;
        dutyCycle += 102;
        
    } else if(INTCON3bits.INT1IF){
        INTCON3bits.INT1IF = 0;
        dutyCycle -= 102;
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
    T2CON=0x06; 
    TRISCbits.RC1=0; 
    CCPR2L=0x00; 
    CCP2CONbits.DC2B=0b00; 
    
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
