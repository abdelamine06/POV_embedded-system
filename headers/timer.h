
#ifndef TIMERS_H
#define TIMERS_H

volatile int time_register_compare;
volatile int time_seconds;
volatile int time_minute;
volatile int time_hour;
volatile int clk_frame_value;
volatile int ocr2a_value;


void TIMER_Init();
void Update_Time();
int Seconds();
void update_time_register();

#endif // TIMERS_H
