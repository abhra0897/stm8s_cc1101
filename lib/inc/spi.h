#ifndef _SPI_H
#define _SPI_H

#include "stm8s.h"
#include "gpio.h"
#include <stdint.h>


// Macros can be directly used instead of the function where fast operation is required
#define SPI_WRITE_BYTE(d)           do{ \
                                        SPI_DR = (uint8_t)(d); \
                                        while (!(SPI_SR & SPI_SR_TXE)); /* SPI is busy in communication or Tx buffer is not empty*/\
                                    }while(0)

#define SPI_READ_BYTE()             SPI_DR

#define SPI_CS_ACTIVE()             do{ \
                                        PORT(CS_PORT, ODR) &= ~SPI_CS; /*CS low*/ \
                                    }while(0);

#define SPI_CS_IDLE()               do{ \
                                        while (SPI_SR & SPI_SR_BSY); \
                                        PORT(CS_PORT, ODR) |= SPI_CS; /*CS high*/ \
                                    }while(0);
/**
 * Configure the SPI bus settings
 */
void spi_config();
/**
 * Wait while SPI is busy
 */
void spi_busy_wait();
/**
 * Wait until RX is read
 */
void spi_rx_wait();
/**
 * Write a 24 bit data to SPI
 */
//void spi_write_24bits(uint32_t data);
/**
 * Write a 8 bit data to SPI
 */
void spi_write_8bits(uint8_t data);
/**
 * Read 8 bits from SPI
 */
uint8_t spi_read_8bits();
/**
 * Activate CS by pulling it low
 */
void spi_cs_active();
/**
 * Idle CS by pulling it high
 */
void spi_cs_idle();

#endif