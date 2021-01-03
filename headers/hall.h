#ifndef HALL_H
#define HALL_H

volatile int number_rotation;
volatile int time_last_rotation; // Delate of last retaion
volatile int time_rotation ;
volatile int hall_interrupt;
void Hall_Init();

#endif
