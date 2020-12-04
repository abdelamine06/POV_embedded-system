#include <avr/io.h>
#include <util/delay.h>


int main() {
    // Active et allume la broche PD6 (led)
    DDRD |= _BV(PD6);

    while(1){

        PORTD |= _BV(PD6);

        _delay_ms(50);

        PORTD &= ~_BV(PD6);

        _delay_ms(50);

    }
}
