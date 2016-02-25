/*
 * Playstation2_Atmega128A.c
 *
 * Created: 2016-02-25 오후 11:10:15
 * Author : Jinwoo - PMNXIS
 */ 


#include <avr/io.h>
#define F_CPU 16000000UL	/*  Clock is 16Mhz  */
#include <util/delay.h>	
#include <avr/interrupt.h>

/*  CONTROL WIRE  */

#define ACK_H	{PORTB |=   0b00010000;}
#define ACK_L		{PORTB &= ~0b00010000;}
//  #define SPIF_STS	0b10000000
//  #define WAIT_SPI_TRANS	{while(!(SPSR & SPIF_STS));}
#define WAIT_SPI_TRANS	{while(!(SPSR & (1<<SPIF)));}

void init(void)
{
	/*  Initializing for Button  */
	DDRF = 0x00;	/*  PORT F0 : SELECT , F1 : START , F2 : L3 , F3 : R3 , F4 : L-UP , F5 : L-RIGHT , F6 : L-DOWN , F7 : L-LEFT  */
	DDRE = 0x00;	/*  PORT E0 : L2 , E1 : R2 , E2 : L1 , E3 : R1 , E4 : R-Up (Triangle) , E5 : R-Right (Circle) , E6 : R-Down (Cross (X)) , E7 : R-Left (Rectangle)  */ 
	/*  PS2 Communication Interface Set-up  */
	/*  out CPU will be SPI slave for this  */
	
	/*  Pull-Up Switching Circuit  */
	PORTF = 0xFF;
	PORTE = 0xFF;
	
	/*
	DR		PIN	ROLE	USE		COLOR(the wire what i using, idk exact modelname..)
	INPUT	B0	SS		~SS		-	yellow
	INPUT	B1	SCLK	clock	-	blue
	INPUT	B2	MOSI	command -	orange
	OUTPUT	B3	MISO	-	data	brown
	OUTPUT	B4	Custom	ACK	-		green
	*/
	
	/* Enable SPI */
	SPCR = 0b01101100;
	SPSR = 0x00;
	DDRB = 0b11111000; /*  0b##111000  */
	// PORTB = 0b00011000;
	
	
}

void acknowledge(void)
{
	/*  ACK  */
	_delay_us(12);
	ACK_L
	_delay_us(2);
	ACK_H
	/*  ACK - END  */		
}

int main(void)
{
	uint8_t recv_cmd;
	init();
	
    /* Replace with your application code */	
	
// 	while (1) 
    do
	{		
		/*  1st Byte  */ 
		SPDR = 0xFF;
		WAIT_SPI_TRANS
		recv_cmd = SPDR;  
		acknowledge();


		/*  2nd Byte  */				
		if(recv_cmd == 0x01)
		{
			SPDR = 0x73;
			WAIT_SPI_TRANS
			recv_cmd = SPDR;
			acknowledge();
		}

		/*  3rd Byte  */
		if(recv_cmd == 0x42)
		{
			SPDR = 0x5A;
			WAIT_SPI_TRANS
			recv_cmd = SPDR;
			acknowledge();
		}

		/*  4th Byte  */				
		SPDR = PINF;
		WAIT_SPI_TRANS;
		recv_cmd = SPDR;
		acknowledge();

		/*  5th Byte  */	
		SPDR = PINE;
		WAIT_SPI_TRANS;
		recv_cmd =SPDR;
		acknowledge();

		//  dummy 
		
		/*  6th Byte  */
		SPDR = 0x7F;
		WAIT_SPI_TRANS;
		recv_cmd =SPDR;
		acknowledge();
		
		/*  7th Byte  */
		SPDR = 0x7F;
		WAIT_SPI_TRANS;
		recv_cmd =SPDR;
		acknowledge();
		
		/*  8th Byte  */
		SPDR = 0x7F;
		WAIT_SPI_TRANS;
		recv_cmd =SPDR;
		acknowledge();
		
		/*  9th Byte  */
		SPDR = 0x7F;
		WAIT_SPI_TRANS;
		recv_cmd =SPDR;



	}while(1);
}