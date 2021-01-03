#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../headers/hall.h"
#include "../headers/usart.h"
#include "../headers/interrupt.h"
#include "../headers/spi.h"
#include "../headers/timer.h"


void Hall_Init()
{
    DDRD &= ~(1 << DDD2);
    PORTD |= (1 << PORTD2);
    EIMSK |= (1 << INT0);     // Enable interrupt
    hall_interrupt=1;

}


// ********************* Interrupt *************************

ISR (INT0_vect)
{
    hall_interrupt=1;
    time_rotation = TCNT1;
    if(number_rotation<7)
    {
      rotation_table[number_rotation] = time_rotation;
      number_rotation++;
    }
    // We calculate the average time to do 7 rotations
    //and then we estimate how much rotation it will be done in 1 sec
    else if(number_rotation==7)
    {
      int sum_rotations = 0;
        for (int i=0; i<number_rotation; i++)
        {
          sum_rotations+=rotation_table[i];
        }
       if (sum_rotations != 0)
       {
         long average = sum_rotations/number_rotation; // how much the average TCNT it takes to do 1 rotation
         long tcnt_sec = 13000000/ 64;   // how much TCNT does our microcontroler does in 1sec
          average_rotation_sec = (int)(tcnt_sec/average); // how much rotation per secondes, it's in Hz
       }
       else
       {
         average_rotation_sec = 0;
       }
       number_rotation=0;
    }

    TCNT1 = 0;
}
