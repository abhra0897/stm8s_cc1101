#ifndef _DELAY_H
#define _DELAY_H

#include <stdint.h>

#ifndef F_CPU
    #warning F_CPU is not defined! Using 16MHz
    #define F_CPU   16000000UL
#endif

/* 
 * Func delayed N cycles, where N = 3 + ( ticks * 3 )
 * so, ticks = ( N - 3 ) / 3, minimum delay is 6 CLK
 * when tick = 1, because 0 equels 65535
 */

// NOT ACCURATE!
static inline void _delay_cycl( unsigned short __ticks )
{
#if defined(__CSMC__)
/* COSMIC */
  #define T_COUNT(x) (( F_CPU * x / 1000000UL )-3)/3)
	// ldw X, __ticks ; insert automaticaly
	_asm("nop\n $N:\n decw X\n jrne $L\n nop\n ", __ticks);
#elif defined(__SDCC)
  #define T_COUNT(x) ((( F_CPU * x / 1000000UL )-5)/5)
	__asm__("nop\n nop\n"); 
	do { 		// ASM: ldw X, #tick; lab$: decw X; tnzw X; jrne lab$
                __ticks--;//      2c;                 1c;     2c    ; 1/2c   
        } while ( __ticks );
	__asm__("nop\n");
#elif defined(__RCST7__)
/* RAISONANCE */
  #error ToDo for RAISONANCE
#elif defined(__ICCSTM8__)
/* IAR */
  #error ToDo for IAR
#else
 #error Unsupported Compiler!          /* Compiler defines not found */
#endif
}

// NOT ACCURATE!
static inline void delay_us( unsigned short __us )
{
	_delay_cycl( (unsigned short)( T_COUNT(__us) ));
}

// NOT ACCURATE!
static inline void delay_ms( unsigned short __ms )
{
	while ( __ms-- )
	{
		delay_us( 1000 );
	}
}


#endif