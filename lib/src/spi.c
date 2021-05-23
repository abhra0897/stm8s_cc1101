#include "spi.h"

/**
 * Configure the SPI bus settings
 */
void spi_config()
{
    // SPI port setup: MISO is pullup in, MOSI & SCK are push-pull out
    PORT(SPI_PORT, DDR) |= SPI_CLK | SPI_MOSI; // clock and MOSI are Output
    PORT(SPI_PORT, CR1) |= SPI_CLK | SPI_MOSI | SPI_MISO; //Clk and MOSI are push-pull and MISO is pullup
    PORT(SPI_PORT, CR2) |= SPI_CLK | SPI_MOSI;  // Clk and MOSI are high speed (10MHz)

    // CS/SS (PD4) as output
    PORT(CS_PORT, DDR) |= SPI_CS; // Output
    PORT(CS_PORT, CR1) |= SPI_CS; // Push-pull
    PORT(CS_PORT, CR2) |= SPI_CS; // High speed
    PORT(CS_PORT, ODR) |= SPI_CS; // CS high


    // SPI registers: First reset everything
    SPI_CR1 = 0;
    SPI_CR2 = 0;

    // SPI_CR1 LSBFIRST=0 (MSB is transmitted first)
    SPI_CR1 &= ~SPI_CR1_LSBFIRST;
    // Baud Rate Control: 0b111 = fmaster / 256 (62,500 baud)
    SPI_CR1 |= SPI_CR1_BR(0b111);

    // Selecting SPI Mode: 0 (CPOL = 0, CPHA = 0)
    // SPI_CR1 CPOL=0 (Clock Polarity, SCK to 0 when idle)
    SPI_CR1 &= ~SPI_CR1_CPOL; 
    // SPI_CR1 CPHA=0 (Clock Phase, The first clock transition is the first data capture edge)
    SPI_CR1 &= ~SPI_CR1_CPHA;

    SPI_CR2 |= SPI_CR2_SSM; // bit 1 SSM=1 Software slave management, enabled
    SPI_CR2 |= SPI_CR2_SSI; // bit 0 SSI=1 Internal slave select, Master mode
    SPI_CR1 |= SPI_CR1_MSTR;  // CR1 bit 2 MSTR = 1, Master configuration.
    SPI_CR1 |= SPI_CR1_SPE; // Enable SPI
}



/**
 * Wait while SPI is busy
 */
void spi_busy_wait()
{
    while (SPI_SR & SPI_SR_BSY);
}


/**
 * Wait until RX is read
 */
void spi_rx_wait()
{
    while (!(SPI_SR & SPI_SR_RxNE));
}

/**
 * Write a 24 bit data to SPI
 */
/*
void spi_write_24bits(uint32_t data)
{
    // for (int8_t i = 2; i >= 0; i--)
    // {
    //     SPI_WRITE_BYTE(0xFF & (data >> (i << 4)));   // Explanation: i<<4 = i*8 = 16, 8, 0 for i = 2, 1, 0 respectively. So, we're shifting data and then sending
    // }

    SPI_WRITE_BYTE(data >> 16);
    SPI_WRITE_BYTE(data >> 8);
    SPI_WRITE_BYTE(data >> 0);
}
*/


/**
 * Write a 8 bit data to SPI
 */
void spi_write_8bits(uint8_t data)
{
    SPI_WRITE_BYTE(data);
}


/**
 * Read 8 bits from SPI
 */
uint8_t spi_read_8bits()
{
    return SPI_READ_BYTE();
}


/**
 * Activate CS by pulling it low
 */
void spi_cs_active()
{
    SPI_CS_ACTIVE();
}


/**
 * Idle CS by pulling it high
 */
void spi_cs_idle()
{
    SPI_CS_IDLE();
}