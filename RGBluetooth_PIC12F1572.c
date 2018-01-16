//PROGRAMA: Controle Luminária RGB.
//Programador: Victor Panitz Magalhães.
//Parte do desenvolvimento: Funcionamento estável.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xc.h>
#define _XTAL_FREQ 16000000   // oscilador interno, 16MHz

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC     = INTOSC // (INTOSC oscillator; I/O function on CLKIN pin)
#pragma config WDTE     = OFF    // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE    = ON     // Power-up Timer Enable (PWRT enabled)
#pragma config MCLRE    = ON     // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP       = OFF    // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN    = OFF    // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF    // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)

// CONFIG2
#pragma config WRT      = OFF    // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN    = OFF    // PLL Enable (4x PLL disabled)
#pragma config STVREN   = OFF    // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV     = LO     // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOREN  = OFF    // Low Power Brown-out Reset enable bit (LPBOR is disabled)
#pragma config LVP      = ON     // Low-Voltage Programming Enable (Low-voltage programming enabled)



//_______________VARIÁVEIS GLOBAIS_______________//

bit  flag_interruption = 0       ;
unsigned char  read          ;
unsigned char UART_counter = 0 ;


void interrupt Serial()     //Clear error flag when needed and handle the UART| limpa flag de erro e trata a uart
 {
             if (RCSTAbits.OERR == 1)
             {
                RCSTAbits.CREN = 0 ;
                read = RCREG    ;
                RCSTAbits.CREN = 1 ;
             }
                    else
                     {
                         UART_handle() ;
                     }
 }


void inicializa_Setup(void)
{
    ANSELA             = 0b00000000 ;  // Define todos os pinos como entrada e saida digital | Define all pins as digital input and output.
    OSCCON             = 0b01111010 ;  //  | 16MHz HF, OSC.
    APFCONbits.RXDTSEL = 1          ;  // Define pino RA4 como Rx | Define pin RA4 as RX.
    APFCONbits.TXCKSEL = 1          ;  // Define pino RA5 como Tx | Define pin RA5 as TX.
    TRISA              = 0b00100000 ;  // Configura porta RA5 como entrada | Config port  RA5 (pin RX) as input.
    PORTA              = 0          ;  // Limpa todas portas como saidas | clear all output ports.
    P1SEL              = 0          ;  // configura pino RA1 como PWM | PWM1 config on pin RA1
    P2SEL              = 0          ;  // Configura pino RA0 como PWM | PWM2 config on pin RA0
}


void inicializa_Serial(void)
{
    RCSTA  = 0b10010000     ;   // Habilita porta serial,recepÁ„o de 8 bit em modo continuo,assincrono. | Enable serial port as 8 bit receiver on continuous mode, async
    TXSTA  = 0b00100100     ;   // Habilita transmiss„o serial, baud rate high speed. | Enable serial transmitter, baud rate high speed.
    SPBRG  = 103            ;   // Valor para gerar Baud Rate. | Value to generate Baud Rate.
    RCIE   = 1              ;   // Habilita interrupÁ„o de recepÁ„o | enable reception interrupter
    TXIE   = 0              ;   // Desabilita interrupÁ„o de transmiss„o | unable transmitter interruption
    PEIE   = 1              ;   // Habilita interrupÁ„o de perifericos do pic. | enable pic peripherals.
    GIE    = 1              ;   // GIE: Global Interrupt Enable bit. | GIE: Global Interrupt Enable bit.
    APFCONbits.RXDTSEL = 1  ;   // Define pino RA1 como RX. | Define pin RA1 as RX.
    APFCONbits.TXCKSEL = 1  ;   // Define pino RA0 como TX. | Define pin RA0 as TX.
    TRISAbits.TRISA5   = 1  ;   // Define RX como entrada. | Define RX as input.
    TRISAbits.TRISA4   = 0  ;   // Define TX como saída. | Define TX as output.
    
}


  //--FUNÇAO DE CONFIG. DO PWM--//

void start_pwm(){

  //-----CONFIG PWM1-----//

       PWM1CLKCON = 0b01110001  ;
       PWM1PH     = 0           ;
       PWM1DC     = 255         ;
       PWM1LD     = 1           ;
       PWM1CON    = 0b11000000  ;
       PWM1PR     = 256         ;
       PWM1PRL    = 0xff        ;
       PWM1PRH    = 0x00        ;


  //-----CONFIG PWM2-----//

       PWM2CLKCON = 0b01110001  ;
       PWM2PH     = 0           ;
       PWM2DC     = 255         ;
       PWM2LD     = 1           ;
       PWM2CON    = 0b11000000  ;
       PWM2PR     = 256         ;
       PWM2PRL    = 0xff        ;
       PWM2PRH    = 0x00        ;


  //-----CONFIG PWM3-----//

       PWM3CLKCON = 0b01110001  ;
       PWM3PH     = 0           ;
       PWM3DC     = 255         ;
       PWM3LD     = 1           ;
       PWM3CON    = 0b11000000  ;
       PWM3PR     = 256         ;
       PWM3PRL    = 0xff        ;
       PWM3PRH    = 0x00        ;

}


//---TRATAMENTO DOS DADOS RECEBIDOS | HANDLE OF RECEIVING DATA---//
int UART_handle()
{
    read = RCREG ;
    UART_counter++ ;

    switch(UART_counter)
    {
        case 1 :
            PWM1DC = 255 - read  ;
            PWM1LD = 1        ;
        break                 ;
            
        case 2 :
            PWM2DC = 255 - read  ;
            PWM2LD = 1        ;
        break                 ;
            
        case 3 :
            PWM3DC = 255 - read  ;
            PWM3LD = 1        ;
            UART_counter = 0 ;
        break                 ;
    }   
}

//---FUNÇAO PRINCIPAL | MAIN FUNCTION---//

void main()
{
    inicializa_Setup()    ;  // Setup básico | Basic setup;
    inicializa_Serial()   ;  // Inicializa Serial | Serial Init.;
    start_pwm()           ;  // Inicialização dos módulos PWM | PWM modules Init.;
    
//---LOOP PRINCIPAL QUE AGUARDA INTERRUPT. | MAIN LOOP---//
    
    for(;;)
    {
    }
}


