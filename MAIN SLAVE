/*
 * Lab 3.c
 *
 * Created: 6/02/2025 11:20:59
 * Author : Luis
 */ 
//*******************************************************************************************************************************
//LIBRERIAS
//*******************************************************************************************************************************
#define F_CPU 16000000

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include "ADC/ADC.h"
#include "SPI/SPI.h"

//*******************************************************************************************************************************
//PROTOTIPOS DE FUNCIONES
//*******************************************************************************************************************************
/*void refreshPort(uint8_t valor);*/

//*******************************************************************************************************************************
//VARIABLES GLOBALES
//*******************************************************************************************************************************
uint8_t POT = 0;
uint8_t POT1 = 0;
uint8_t POT2 = 0;
uint8_t LED =0;
char buff[0];

//*******************************************************************************************************************************
//MAIN
//*******************************************************************************************************************************
int main(void)
{
    //PUERTOS DE SALIA
	DDRD |=	(1 << DDD0) | (1 << DDD1) | (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
	
	//INCIAR COMUNICACIÓN SPI
    spiInit(SPI_SLAVE_SS, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_LOW, SPI_CLOCK_FIRST_EDGE);
	//INICIAR EL ADC
	setADC(externo, izquierda, 6);
	controlADC(1, 0, 1, 128);
	
	SPCR |= (1 << SPIE);
	sei();
	while (1) 
    {
		ADCSRA |= (1 << ADSC);
		
		
		_delay_ms(100);
    }
}

//*******************************************************************************************************************************
//FUNCIONES
//*******************************************************************************************************************************
/*void refreshPort(uint8_t valor){
	if(valor & 0b10000000){
		PORTD |= (1 << PORTD7);
	}else{
		PORTD &= ~(1 << PORTD7);
	}
	
	if(valor & 0b01000000){
		PORTD |= (1 << PORTD6);
	}else{
		PORTD &= ~(1 << PORTD6);
	}
	
	if(valor & 0b00100000){
		PORTD |= (1 << PORTD5);
	}else{
		PORTD &= ~(1 << PORTD5);
	}
	
	if(valor & 0b00010000){
		PORTD |= (1 << PORTD4);
	}else{
		PORTD &= ~(1 << PORTD4);
	}
	
	if(valor & 0b00001000){
		PORTD |= (1 << PORTD3);
	}else{
		PORTD &= ~(1 << PORTD3);
	}
	
	if(valor & 0b00000100){
		PORTD |= (1 << PORTD2);
	}else{
		PORTD &= ~(1 << PORTD2);
	}
	
	if(valor & 0b00000010){
		PORTD |= (1 << PORTD1);
	}else{
		PORTD &= ~(1 << PORTD1);
	}
	
	if(valor & 0b00000001){
		PORTD |= (1 << PORTD0);
	}else{
		PORTD &= ~(1 << PORTD0);
	}
}*/

//*******************************************************************************************************************************
//INTERRUPCIONES
//*******************************************************************************************************************************
ISR(ADC_vect){
	if (POT == 7){
		POT1 = ADCH; //Leer el potenciometro activado
		setADC(externo, izquierda, 6); //Setear el siguiente potenciometro
	}
	else {
		POT2= ADCH;
		setADC(externo, izquierda, 7);
	}
	
	ADCSRA |= (1 << ADIF); //Apagar la bandera de ADC
}

ISR(SPI_STC_vect){
	uint8_t spiValor = SPDR;
	
	if (LED == 1)
	{
		LED = 0;
		PORTD = spiValor;
		SPDR = POT2;
	}
	if (spiValor == 'P'){
		spiWrite(POT1);
	}
	else if(spiValor == 'p'){
		spiWrite(POT2);
	}
	else if (spiValor == 'L'){
		LED = 1;
		SPDR = POT1;
	}
}
