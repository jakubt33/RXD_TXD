#include <avr/io.h>
#define F_CPU 1000000UL
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 9600
#define FOSC 1000000
#define MYUBRR FOSC/16/BAUD-1

#define LED1 PC1
#define LED2 PC2
#define LED3 PC3
#define PORT_DIODY DDRC

void USART_Init ( unsigned char ubrr);
unsigned char USART_Receive();
void LED_Init();

int main ()
{
    USART_Init( MYUBRR );
    LED_Init();
    sei();

    while (1)
    {
        PORT_DIODY |= _BV(LED3);
        _delay_ms(750);
         PORT_DIODY &= ~_BV(LED3);
        _delay_ms(750);
    }

}

ISR(USART_RXC_vect)
{
    unsigned char data=0;
    data=UDR;

    if(data==1)
        {
            PORT_DIODY |= _BV(LED1);
            PORT_DIODY &= ~_BV(LED2);
        }
    else if (data == 2)
        {
            PORT_DIODY |= _BV(LED2);
            PORT_DIODY &= ~_BV(LED1);
        }
    else
        PORT_DIODY = 0xff;
}

void LED_Init()
{
    PORTC = 0xff;
    DDRC = 0x00; //zgas wszystkie
}

void USART_Init ( unsigned char ubrr)
{
    // Ustawienie prędkości transmisji
    UBRRH = (uint8_t)(ubrr >> 8);
    UBRRL = (uint8_t)ubrr;

    // Włączenie odbiornika
    UCSRB = (1 << RXEN); //| (1 << TXEN)
    UCSRB |= (1 << RXCIE); //interrupt na receive

    // Ustawienie formatu ramki: 8 bitów danych, 2 bit stopu
    UCSRC = (1 << URSEL) | (1 << USBS) | (3<<UCSZ0);
}
