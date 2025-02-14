/*
 * Lab 3.c
 *
 * Created: 6/02/2025 11:20:59
 * Author : Fatima
 */ 
//*******************************************************************************************************************************
//LIBRERIAS
//*******************************************************************************************************************************
//*******************************************************************************************************************************
#define F_CPU 16000000

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include "SPI/SPI.h"

#define BUFFER_SIZE 4 

//********************************************************************************************************************************************
// Variables Globales
//********************************************************************************************************************************************

uint8_t valor = 0;
float volt = 0;
char buf[20];
uint8_t bits[8];
char recibido;
uint8_t resultado = 0;
uint8_t a = 0;

char buffer[BUFFER_SIZE];

//********************************************************************************************************************************************
// Declarar Funciones
//********************************************************************************************************************************************

void setup(void);
void initUART9600(void);
void floatToString(float num, char *str, int precision);
void textUART(char *texto);


//*******************************************************************************************************************************
//MAIN
//*******************************************************************************************************************************
int main(void)
{
	setup();
    
    while (1) 
    {
		PORTB &= ~(1 << PORTB2);
		spiWrite('P');
		valor = spiRead();
		volt = valor * 5.0 / 255;
		floatToString(volt, buf, 2);
		textUART("Potenciometro1        Potenciometro2\n");
		textUART(buf);
		textUART("                   ");
		//valor = 0;
		PORTB |= (1 << PORTB2);
		_delay_ms(250);
		
		PORTB &= ~(1 << PORTB2);
		spiWrite('p');
		valor = spiRead();
		volt = valor * 5.0 / 255;
		floatToString(volt, buf, 2);
		textUART(buf);
		textUART("\n");
		PORTB |= (1 << PORTB2);
		_delay_ms(250);
		
		spiWrite('L');  // Enviar comando de LED
		valor = spiRead();
		volt = valor * 5.0/255;
		_delay_ms(250);
		spiWrite(resultado);  // Enviar el valor de los LEDs
		valor = spiRead();
		volt= valor * 5.0/255;
		
		//spiWrite((char) resultado);
		PORTD = (PORTD & 0b00000011) | ((resultado & 0b00111111) << 2); // Enviar los primeros 6 bits a PORTD (PD2 a PD7)
		PORTC = (PORTC & 0b11111100) | ((resultado & 0b11000000) >> 6); // Enviar los últimos 2 bits a PORTB (PB0 y PB1)
		
		
		_delay_ms(400);
    }
}

//********************************************************************************************************************************************
// Setup
//********************************************************************************************************************************************

void setup(void){
	DDRB |= (1 << DDB2);
	PORTB |= (1 << PORTB2);
	
	DDRD |= (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
	DDRC |= (1 << DDC0) | (1 << DDC1);
	
	//------------------------ UART ----------------------------
	
	initUART9600();
	
	//------------------------ SPI -----------------------------

	spiInit(SPI_MASTER_OSC_DIV16, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_LOW, SPI_CLOCK_FIRST_EDGE);
	
	sei(); 
}

//********************************************************************************************************************************************
// Sub-Funciones
//********************************************************************************************************************************************

void initUART9600(void){
	DDRD &= ~(1<<DDD0); //RX y TX como entrada y salida
	DDRD |= (1<<DDD1);
	
	UCSR0A = 0;
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	
	UCSR0C = 0;
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	
	UBRR0 = 103; //valor baud y velocidad
}

void floatToString(float num, char *str, int precision) {
	int intPart = (int)num;  // Parte entera
	float decimalPart = num - intPart;  // Parte decimal

	// Convertir parte entera con itoa()
	itoa(intPart, str, 10);

	// Agregar punto decimal
	int i = 0;
	while (str[i] != '\0') i++;  // Buscar el final de la cadena
	str[i++] = '.';

	// Convertir parte decimal manualmente
	for (int j = 0; j < precision; j++) {
		decimalPart *= 10;
		int decDigit = (int)decimalPart;
		str[i++] = decDigit + '0';
		decimalPart -= decDigit;
	}

	str[i] = '\0';  // Terminar la cadena
}

void textUART(char *texto){
	uint8_t i;
	
	for(i = 0; texto[i] != '\0'; i++){
		while(!(UCSR0A & (1 << UDRE0)));
		UDR0 = texto[i];
	}
}

 

//********************************************************************************************************************************************
// Interrupciones
//********************************************************************************************************************************************

ISR(USART_RX_vect){
	 recibido = UDR0;
	 
	 if (recibido == '\n' || recibido == 'l') {
		 buffer[a] = '\0'; // Terminar la cadena
		 resultado = (uint8_t)atoi((char*)buffer);
		 
		 a = 0;
	} 
	else if (a < BUFFER_SIZE - 1){
		 buffer[a++] = recibido; // Almacenar caracter
	 }
	 
	
}
