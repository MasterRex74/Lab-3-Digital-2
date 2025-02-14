/*
 * SPI.c
 *
 * Created: 6/02/2025 11:28:07
 *  Author: Luis
 */ 
#include "SPI.h"

void spiInit(Spi_Type sType, Spi_Data_Order sDataOrder, Spi_Clock_Polarity sClockPolarity, Spi_Clock_Phase sClockPhase){
	//PB2 --> SS
	//PB3 --> MOSI
	//PB4 --> MISO
	//PB5 --> SCK
	
	//VERIFICAR SI ES MAESTRO
	if(sType & (1 << MSTR)){
		DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);		//MOSI, SCK, NEGADO_SS
		DDRB &= ~(1 <<DDB4);		//MISO
		SPCR |= (1 << MSTR);		//MASTER
		
		uint8_t temp = sType & 0b00000111;
		switch(temp){
			case 0: //DIV 2
			SPCR &= ~(1 << SPR1) | (1 << SPR0);
			SPSR |= (1 << SPI2X);
			break;
			
			case 1:	//DIV 4
			SPCR &= ~((1 << SPR1) | (1 << SPR0));
			SPSR &= ~(1 << SPI2X);
			break;
			
			case 2: //DIV8
			SPCR |= (1 << SPR0);
			SPCR &= (1 << SPR1);
			SPSR |= (1 << SPI2X);
			break;
			
			case 3: //DIV 16
			SPCR |= (1 << SPR0);
			SPCR &= ~(1 << SPR1);
			SPSR &= ~(1 << SPI2X);
			break;
			
			case 4: //DIV32
			SPCR &= ~(1 << SPR0);
			SPCR |= (1 << SPR1);
			SPSR |= (1 << SPI2X);
			break;
			
			case 5: //DIV64
			SPCR &= ~(1 << SPR0);
			SPCR |= (1 << SPR1);
			SPSR &= ~(1 << SPI2X);
			break;
			
			case 6: //DIV 128
			SPCR |= (1 << SPR0) | (1 << SPR1);
			SPSR &= ~(1 << SPI2X);
			break;
		}
	}
	else											//ESCLAVO
	{
		DDRB |= (1 << DDB4);		//MISO
		DDRB &= ~((1 << DDB3) | (1 << DDB5) | (1 << DDB2));			//MOSI, SCK, SS
		SPCR &= ~(1 << MSTR);		//ESCLAVO
	}
	SPCR |= (1 << SPE) | sDataOrder | sClockPolarity | sClockPhase;
}

static void spiReceiveWait(){
	while (!(SPSR & (1 << SPIF)));
}

void spiWrite(uint8_t dat){
	SPDR = dat;
}

unsigned spiDataReady(){
	if (SPSR & (1 << SPIF))
	return 1;
	else
	return 0;
}

uint8_t spiRead(void){
	while(!(SPSR & (1 << SPIF)));
	return(SPDR);
}
