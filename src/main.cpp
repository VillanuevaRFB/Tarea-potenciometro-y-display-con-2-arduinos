#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/*
unsigned char voltaje=0;

void config_ADC(void){
  ADCSRA|=(1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//AVcc, prescaler 128
  ADMUX|=(1<<REFS0);//canal ADC0
}

void config_USART(void){
  UBRR0=103;//9600 baudios a 16 MHz
  UCSR0B|=(1<<TXEN0)|(1<<RXEN0);//habilita tx y rx
  UCSR0B|=(1<<UDRIE0);//activa interrupcion
  UCSR0C|=(1<<UCSZ01)|(1<<UCSZ00);
}

ISR(ADC_vect){
  unsigned int valor=ADC;
  voltaje=(valor*50)/1023;//escala a 0–50
  UCSR0B|=(1<<UDRIE0);//activa envio por TX
}

ISR(USART_UDRE_vect){
  UDR0=voltaje;//envia valor a tx
  UCSR0B&=~(1<<UDRIE0);//desactiva interrupción tras enviar
}

int main(void){
  config_ADC();
  config_USART();
  sei();
  while(1){
    ADCSRA|=(1<<ADSC);//conversion ADC
    _delay_ms(100);
  }
}
*/
unsigned char valor=0;//valor recibido

void config_USART(void){
  UBRR0=103;//9600 baudios
  UCSR0B|=(1<<RXEN0)|(1<<RXCIE0);//habilita rx y su interrupcion
  UCSR0C|=(1<<UCSZ01)|(1<<UCSZ00);
}

ISR(USART_RX_vect){
  valor=UDR0;//guarda valor recibido
}

void mostrar_display(unsigned char numero, unsigned char digito){
  PORTD=(PORTD&0x0F)|(numero<<4);
  PORTB&=~(1<<digito);//activa displays
  _delay_ms(5);
  PORTB|=(1<<digito);//apaga displays
}

int main(void){
  DDRD|=0xF0;//PD4–PD7 displays
  DDRB|=0x03;//PB0 y PB1 multiplexado
  config_USART();
  sei();
  while(1){
    unsigned char entero=valor/10; //entero del voltaje
    unsigned char decimal=valor%10; //decimal del voltaje
    mostrar_display(decimal,0); //muestra decimal en PB0
    mostrar_display(entero,1); //muestra entero en PB1
  }
}