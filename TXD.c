#include <avr/io.h>
#define F_CPU 1000000UL
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 9600
#define FOSC 1000000
#define MYUBRR FOSC/16/BAUD-1



void USART_Init ( unsigned char );
void USART_Transmit( unsigned char );

int main ()
{
    USART_Init( MYUBRR );
    unsigned char data=0;
    while (1)
    {

        data = 1;
        USART_Transmit(data);
        _delay_ms(1000);

        data = 2;
        USART_Transmit(data);
        _delay_ms(1000);

    }

}
void USART_Transmit( unsigned char data)
{
    //wait for empty transmit buffer
    while ( !(UCSRA & (1<<UDRE)) );

    //put data into buffer, sends data
    UDR = data;
}
void USART_Init ( unsigned char ubrr)
{
	// Ustawienie prędkości transmisji
	UBRRH = (uint8_t)(ubrr >> 8);
	UBRRL = (uint8_t)ubrr;

	// Włączenie nadajnika
	UCSRB = (1 << TXEN); // (1 << RXEN)

	// Ustawienie formatu ramki: 8 bitów danych, 2 bit stopu
	UCSRC = (1 << URSEL) | (1 << USBS) | (3<<UCSZ0);
}
