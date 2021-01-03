
#ifndef TIMERS_H
#define TIMERS_H

void TIMER_Init();
void Update_Time();
int Seconds();
volatile int ocr2a_value;
volatile int rotation_table[7];
volatile int average_rotation_sec;
void update_time_register();

#endif // TIMERS_H
