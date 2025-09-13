#include <avr/io.h> 
#include <util/delay.h>

#define PB0 PIN4


int main(void) {
  PORTA.DIR = PORTA.DIR | (1<<PB0);
  // DDRB = DDRB | (1 << PB0);
  // DDRB = Data Direction Register for port B.
  // Setting PIN1 on PORTB to 1. 1 means it is an output pin.

  while (1) {
    _delay_ms(1000);
    PORTA.OUT = PORTA.OUT ^ (1 << PB0);
  }
}
