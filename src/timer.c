#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/timer.h"
#include "../headers/usart.h"
#include "../headers/spi.h"

volatile int time_register_compare = 0;
volatile int time_seconds =0;
volatile int time_minute = 0;
volatile int time_hour = 0;
volatile int clk_frame_value= 0;


/******************************Timer Init******************************************/
void TIMER_Init()
{

    /*****************Initiate value of OCROA To calculate time with seconds***********************************/
  
        // The counting sequence is determined by the setting of the WGM01 and WGM00 
        // bits located in the Timer/Counter control register (TCCR0A) and the WGM02 
        // bit located in the Timer/Counter control 
        // register B (TCCR0B).
        // TCCR0A |= (1 << WGM01) | (0 << WGM00);
        TCCR0B |= (0 << WGM02); // Active Time/count0 
        OCR0A = 125; // output compare register, (TCNT0). 
                    // Defines the top value for the counter, hence also its 
                    // resolution
        TIMSK0 |= (1 << OCIE0A); // active interrupt 
        TCCR0B |= (1<< CS00) | (1 << CS01) | (0 << CS02); // clkI/O/64 (from prescaler)
        EICRA |= (1 << ISC00) | (1 << ISC01); 
        TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);// Prescaler to 1024. Timer1(16-bit)


    //******************************************Manipulate Horloge frequence and OCR2A To make interrupt every 1/60 angle********
    
        OCR2A = 168; // OCRn =  O.75ms/Tour de pov =>Interrupt
        TCCR2A |= (1 << WGM21); // enaable CTC Mode
        TIMSK2 |= (1 << OCIE2A); //enable interrupt on compare match
        TCCR2B |= (1 << CS22) | (0 << CS21) | (0 << CS20); // set prescaler to 1024 
        
    
}

/***********************************************Manipulate Timer***********************/
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
int getMinute(){
   return time_seconds/60;
}


/*
 * return time hours
 */
int getHour(){
    return time_seconds/3600;
}


/* 
* Return clock statique value between 0 and 59
*/
void clk_frame(){
  if (clk_frame_value == 59){
    clk_frame_value = 0;
  }
  else{
    clk_frame_value++;
  }
}


/***********************************************Interrupt***********************
/*
* interrupt to increment time seonconds 
*/

ISR(TIMER0_COMPA_vect){
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
    else if(getHour()== clk_frame_value){
      SPI_MasterTransmit((uint8_t)128);
      SPI_MasterTransmit((uint8_t)255);
    }
    else{
     SPI_MasterTransmit((uint8_t)128);
     SPI_MasterTransmit(0);
    }

    PORTC|= _BV(PC2); //LE à 1
    PORTC&= ~_BV(PC2); //LE à 0
    clk_frame();
}

    
    
