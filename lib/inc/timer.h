#ifndef _TIMER_H
#define _TIMER_H

#include "stm8s.h"
#include <stdint.h>
#include <stdio.h>

#define TIMER_DEBUG 1

void timer_config();
void timer_delay_ms(uint32_t ms);
void timer_stop();
uint64_t timer_get_ms_tick();
void timer4_isr() __interrupt(TIM4_ISR);

#endif