#ifndef _GPIO_H
#define _GPIO_H

#include "stm8s.h"

#define SPI_PORT                PC
#define SPI_MOSI                PIN6
#define SPI_MISO                PIN7
#define SPI_CLK                 PIN5
#define CS_PORT                 PD
#define SPI_CS                  PIN4

#define GDO0_PORT               PD
#define GDO0_PIN                PIN3
#define GDO2_PORT               PD
#define GDO2_PIN                PIN2

#define DEBUG_PORT              PC
#define DEBUG_PIN               PIN3 //PC3

#define CONFIG_GDO0_PIN_AS_INPUT()      (PORT(GDO0_PORT, DDR) &= ~GDO0_PIN)
#define GDO0_PIN_IS_HIGH()              (PORT(GDO0_PORT, IDR) & GDO0_PIN)

#define CONFIG_GDO2_PIN_AS_INPUT()      (PORT(GDO2_PORT, DDR) &= ~GDO2_PIN)
#define GDO2_PIN_IS_HIGH()              (PORT(GDO2_PORT, IDR) & GDO2_PIN)

#define SPI_SO_PIN_IS_HIGH()            (PORT(SPI_PORT, IDR) & SPI_MISO)
#define SPI_SI_PIN_IS_HIGH()            (PORT(SPI_PORT, IDR) & SPI_MOSI)

#define DEBUG_PIN_CONFIG()              do{PORT(DEBUG_PORT, DDR) |= DEBUG_PIN;\
                                            PORT(DEBUG_PORT, ODR) &= ~DEBUG_PIN;}while(0)
#define DEBUG_PIN_TOGGLE()              (PORT(DEBUG_PORT, ODR) ^= DEBUG_PIN)
#define DEBUG_PIN_SET_HIGH()            (PORT(DEBUG_PORT, ODR) |= DEBUG_PIN)
#define DEBUG_PIN_SET_LOW()             (PORT(DEBUG_PORT, ODR) &= ~DEBUG_PIN)
#endif