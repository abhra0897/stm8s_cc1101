#include <stdint.h>
#include "timer.h"
#include "gpio.h"

volatile static uint64_t g_ticks;

void timer4_isr() __interrupt(TIM4_ISR)
{
    //disableInterrupts();
    #if TIMER_DEBUG == 1
    DEBUG_PIN_TOGGLE();
    #endif
    ++g_ticks; 
    TIM4_SR &= ~TIM_SR1_UIF;    // clear the interrupt flag
    //enableInterrupts();
}

void timer_config()
{
    // Target is to generate 1ms pulse
    // F_CNT = F_CLK / Prescaler = 16M / 64 = 250000
    // So, counting 250 takes 1ms

    /* Prescaler = 2^TIM4_PSCR */
    TIM4_PSCR = 0b00000110; // 64
    // /* Period = 1ms */
    TIM4_ARR = 249;

    TIM4_CNTR = 0;

    //TIM4_CR1 |= TIM_CR1_URS;
    TIM4_IER |= TIM_IER_UIE; // Enable Update Interrupt
    TIM4_CR1 |= TIM_CR1_CEN; // Enable TIM4
}


// Mostly accurate timing
void timer_delay_ms(uint32_t ms)
{
    uint64_t reading = timer_get_ms_tick();
    while (timer_get_ms_tick() - reading <= ms)
    {
        // kill time
    }
}

uint64_t timer_get_ms_tick()
{
    return g_ticks;
}

void timer_stop()
{
    disableInterrupts();

    TIM4_CNTR = 0;
    TIM4_CR1 &= ~TIM_CR1_CEN;
    TIM4_IER &= ~TIM_IER_UIE;
}