#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/timer.h"
#include "../headers/usart.h"
#include "../headers/spi.h"
#include "../headers/hall.h"

/****************************** Timer Init ******************************************/
void TIMER_Init()
{

  time_register_compare = 0;
  time_seconds =0;
  time_minute = 0;
  time_hour = 0;
  clk_frame_value= 59;
  ocr2a_value =255;

  // The counting sequence is determined by the setting of the WGM01 and WGM00
  // bits located in the Timer/Counter control register (TCCR0A) and the WGM02
  // bit located in the Timer/Counter control
  // register B (TCCR0B).
  TCCR0A |= (1 << WGM01) | (0 << WGM00);
  TCCR0B |= (0 << WGM02); // Active Time/count0
  OCR0A = 125; // output compare register, (TCNT0).
               // Defines the top value for the counter, hence also its
               // resolution
  TIMSK0 |= (1 << OCIE0A); // active interrupt
  TCCR0B |= (1<< CS00) | (1 << CS01) | (0 << CS02); // clkI/O/64 (from prescaler)
  EICRA |= (1 << ISC00) | (1 << ISC01);
  TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);// Prescaler to 1024. Timer1(16-bit)

  OCR2A = ocr2a_value;     // OCRn =  O.75ms/Tour de pov =>Interrupt
  TCCR2A |= (1 << WGM21);  // enaable CTC Mode
  TIMSK2 |= (1 << OCIE2A); //enable interrupt on compare match
  TCCR2B |= (1 << CS22) | (0 << CS21) | (0 << CS20); // set prescaler to 64 
}

void Update_Time()
{
    if (time_register_compare == 1625)
    {
        time_seconds++;
        time_register_compare = 0;
    }
}
/*
 * return time seconds
 */
int getSeconds()
{
    return time_seconds;
}
/*
 * return time minutes
 */
int getMinute()
{
   return (int) (time_seconds/60);
}
/*
 * return time hours
 */
int getHour(){
    return (int) (time_seconds/3600) ;
}
/*
* Return clock statique value between 0 and 59
*/
void clk_frame(){
  if (clk_frame_value == 0){
    clk_frame_value = 59;
  }
  else{
    clk_frame_value--;
  }
}

void update_time_register()
{
    clk_frame_value = 0;

    unsigned long target_value = (13000000 * 60 / 1024)/tcnt1_value;

    ocr2a_value = (13000000/64)/target_value - 1; // The target frequency here is average_rotation_sec
    
    if (ocr2a_value<=255 && ocr2a_value > 0) // check if it's lower than the max value
    {

      OCR2A = ocr2a_value;
    }
    else
    {

      OCR2A = 255;
    }

    TCNT2 = 0;
}

/*********************************************** Interrupt *********************************
 
/*
* interrupt to increment time seonconds
*/

ISR(TIMER0_COMPA_vect)
{
    time_register_compare++;
}

/*
* Interrupt every 1/60 circle frame on owr Pov
*/

ISR (TIMER2_COMPA_vect){

    if(getMinute() == clk_frame_value){
      SPI_MasterTransmit((uint8_t)255);
      SPI_MasterTransmit((uint8_t)255);
    }
     else if(getHour() == clk_frame_value){
      SPI_MasterTransmit((uint8_t)128);
      SPI_MasterTransmit((uint8_t)255);
    }
    else{
     SPI_MasterTransmit((uint8_t)128);
     SPI_MasterTransmit(0);
    }
    clk_frame();
    PORTC|= _BV(PC2); //LE à 1
    PORTC&= ~_BV(PC2); //LE à 0
}