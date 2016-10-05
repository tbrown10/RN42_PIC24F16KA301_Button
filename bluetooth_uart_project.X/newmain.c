//Taylor Brown 10/4/2016
//DORSAL RN42_PIC24F16KA301_Button 


#include <p24F16KA301.h>		//Include the header file for the Pic
#pragma config FNOSC = FRC		//Select the 8 MHz FRC Oscillator (FRC)  (See Sec. 9 of Datasheet, bits 8-10)		
							
#define FP 4000000			//FP denotes the instruction cycle clock frequency (FOSC/2) (See Pg 176 of PIC24F Family Reference Manual)
#define BAUDRATE 9600		
#define BRGVAL ((FP/(BAUDRATE*16))-1)	//See Pg 176 of PIC24F Family Reference Manual for more info
unsigned int i;
#define DELAY_105uS asm volatile ("REPEAT, #4201"); Nop(); // 105uS delay

//---------------------------------------------------------------------------------------------//
//Interrupt Code
//---------------------------------------------------------------------------------------------//

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
	_CNIF = 0;			//Clear interrupt flag (IFS1 register)
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
	_T1IF = 0;			//Clear Timer 1 Interrupt Flag
}

void __attribute__((__interrupt__)) _U1TXInterrupt(void)
{
	IFS0bits.U1TXIF = 0;        // Clear TX Interrupt flag
	//U1TXREG = '0';              // Transmit one character
}

//---------------------------------------------------------------------------------------------//
//Configuration Code
//---------------------------------------------------------------------------------------------//

void configure_pins(void);
void configure_timer1(void);
void configure_CN1(void);
void configure_UART(void);

int main(){
    

    
    while(1)
    {  
    DELAY_105uS
    U1TXREG = 'a'; 
    
    }



return 0;

}

void configure_pins(void)
{
				//Set Pins to be input(1) or output(0)
	_TRISA0 = 0;		//Pin 2(RA0) is output
	_TRISB1 = 1;		//Pin 5(RB1) is input
	_TRISB2 = 0;		//Pin 6(RB2) is output
	
	ANSA = 0;		//Configure Port A bits as digital 
	ANSB = 0;		//Configure Port B bits as digital
	
				//Read/write to individual bits use _RA6 = 1
				//Read/write to individual latch use _LATA6 = 1
}

void configure_timer1(void)
{
					//Configure Timer1 using T1CON register
	T1CONbits.TON = 1;		//Turn on Timer1 (See Sec. 12 of Datasheet)
	T1CONbits.TCS = 0;		//Select internal instruction cycle clock(0), (Internal clock is Fosc/2)
	T1CONbits.TCKPS = 0b10;		//Set timer prescale(64)
	PR1 = 0xF424;			//Set timer period of 31250 or 0.5 seconds
	TMR1 = 0;			//Reset Timer1 to zero

					//Configure Timer1 interrupt
	_T1IP = 4;			//Set interrupt priority
	_T1IE = 0;			//Disable interrupt (enabling is done in CN interrupt)
	_T1IF = 0;			//Clear interrupt flag
	
}

void configure_CN1(void)
{
					//Configure Change Notification interrupt
	_CN5IE = 1;			//Enable CN on Pin 5
	_CN5PUE = 0;			//Disable pull-up resistor
	_CNIP = 6;			//Set CN interrupt priority
	_CNIF = 0;			//Clear interrupt flag
	_CNIE = 1;			//Enable CN interrupts
}

void configure_UART(void)
{
                    			//Configure UART
   
	U1MODEbits.USIDL = 0;   	//Continue operation in idle mode
	U1MODEbits.IREN = 0;    	//Disable IrDA
	U1MODEbits.STSEL = 0;           // 1-Stop bit
	U1MODEbits.PDSEL = 0;           // No Parity, 8-Data bits
	U1MODEbits.ABAUD = 0;           // Auto-Baud disabled
	U1MODEbits.BRGH = 0;            // Standard-Speed mode
   
	//U1BRG = BRGVAL;               //Why not working?
	U1BRG = 25;            		//Baud Rate setting for 9600(25)
    
	U1STAbits.UTXISEL0 = 0;         //Disable UART TX interrupt
    
	U1MODEbits.UARTEN = 1;          //Enable UART
	U1STAbits.UTXEN = 1;            //Enable UART TX
    
	DELAY_105uS			// Wait at least 105 microseconds (1/9600) before sending first char
}
	
	





























