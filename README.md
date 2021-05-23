### CC1101 STM8S Library

Last updated (May 24, 2021)

#### Note
This is a driver library to interface CC1101 RF transceiver with STM8S series MCUs, written in C. This library is compatible with SpaceTeddy's CC1101 Library for Arduino and also with my CC1101 library for MSP430.

If you want to communicate between a STM8S and:
1. **an Arduino (AVR based)**, use SpaceTeddy's library for Arduino (https://github.com/SpaceTeddy/CC1101).
2. **an MSP430**, use my msp430_cc1101_energia_v2 library for MSP430 (https://github.com/abhra0897/msp430_cc1101_energia_v2).

This library is compatible with both of them.

This is inspired by SpaceTeddy's CC1101-Arduino library, since that one is popular enough and works flawlessly. I added the hardware support for STM8S and memory optimized the code at some places.

#### How to use
1. Download the [SDCC compiler](https://sourceforge.net/projects/sdcc/). The compiler version I've used is: 4.0.0
2. Install the compiler by following instructions in INSTALL.txt (in SDCC downloaded folder)
3. Clone/Download this repo.
4. Download and install "Make" if not already installed
5. Navigate to this `examples` folder and then pick any example
6. To compile, run `make`. To burn, run `make burn`. To clean, `make clean`


#### Supported Launchpads/MCUs
- STM8S003
- STM8S103

#### Supported Frequencies
- 315 MHz
- 433 MHz
- 868 MHz
- 915 MHz

Check the examples for basic usage.


#### Pinouts
See [this pin description image of the CC1101](http://www.kooing.com/image/cache/data/CC1101_3-500x500.jpg). The proper orientation is with the antenna facing to your right, with the text "RF1100SE" in the correct orientation. You may have a different module, but the below pinout works for all of them.

    ** MCU Pin numbers are as per the datasheet.
	VDD  -> VCC
    SCK (Serial Clock)      -> PC5
	SI (Slave In)           -> PC6
	SO (Slave Out)          -> PC7
	CSn (Chip Select)       -> PD4	
	GDO2                    -> PD2 (Till now, unused)
	GDO0                    -> PD3 (Till now, unused)
	GND                     -> GND

#### General description of RF packet

```
-> pkt_len [1byte] | rx_addr [1byte] | tx_addr [1byte] | payload data [1..60bytes]
```

pkt_len = count of bytes which shall transfered over air (rx_addr + tx_addr + payload data)<br />
rx_addr = address of device, which shall receive the message (0x00 = broadcast to all devices)<br />
tx_addr = transmitter or my address. the receiver should know who has sent a message.<br />
payload = 1 to 60 bytes payload data.<br />

TX Bytes example:<br />
-> 0x06 0x03 0x01 0x00 0x01 0x02 0x03<br />

#### Basic configuration

use **uint8_t cc1101_begin(volatile uint8_t *My_addr)** always as first configuration step. This function returns the device address.

##### Device address

you should set a unique device address for the transmitter and a unique device address for the receiver.
This can be done with **void cc1101_set_myaddr(uint8_t addr)**.

i.E. -> TX = 0x01 ; RX = 0x03

##### Modulation modes

the following modulation modes can be set by **void cc1101_set_mode(uint8_t mode)**. Transmitter and receiver must have the same Mode setting.

```
1 = GFSK_1_2_kb
2 = GFSK_38_4_kb
3 = GFSK_100_kb
4 = MSK_250_kb
5 = MSK_500_kb
6 = OOK_4_8_kb
```

##### ISM frequency band

you can set a frequency operation band by **void CC1101_set_ISM(uint8_t ism_freq)** to make it compatible with your hardware.

```
1 = 315
2 = 433
3 = 868
4 = 915
```

#### MINIMAL_MODE
Since stm8s has a small flash (8kb), using MINIMAL_MODE saves some space (around 1KB saving).
To use minimal mode, set `#define MINIMAL_MODE  1` in stm8_cc1101.c file. To disable MINIMAL_MODE, set it to 0. In minimal mode, only a few pre-configured settings work. You can't set custom freq/data rate etc. To be honest, most of the time one will use the pre-configures ones anyway.

To see what won't work in MINIMAL_MODE, go through the stm8_cc1101.c file.

#### Important API Methods
```C
    // Use it as the begining to configure the initial settings
    uint8_t cc1101_begin(volatile uint8_t *My_addr);

    // Enable or disable debug output with 1 or 0
    uint8_t cc1101_set_debug_level(uint8_t set_debug_level);

    // Following methods are for SPI read and write. You may not need to use them in main program.
    void cc1101_spi_write_strobe(uint8_t spi_instr);
    void cc1101_spi_write_register(uint8_t spi_instr, uint8_t value);
    void cc1101_spi_write_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t length);
    void cc1101_spi_read_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t length);
    uint8_t cc1101_spi_read_register(uint8_t spi_instr);
    uint8_t cc1101_spi_read_status(uint8_t spi_instr);

    // Resets the CC1101 module following the reset sequence
    void cc1101_reset(void);

    // Enables powersaving mode of CC1101. Consumes around 200nA
    void cc1101_powerdown(void);

    // Enables Wakeup-On-Radio mode. If enabled, CC1101 wakes up automatically when packet is received
    void cc1101_wor_enable(void);

    // Disables WOR
    void cc1101_wor_disable(void);

    // Resets WOR
    void cc1101_wor_reset(void);

    // Sets CC1101 in idle mode
    uint8_t cc1101_sidle(void);

    // Enables transmit mode
    uint8_t cc1101_transmit(void);

    // Enables receive mode
    uint8_t cc1101_receive(void);

    // Shows the CC1101 register settings by reading using SPI. Use it to be sure if registers are written properly.
    void cc1101_show_register_settings(void);

    // Returns TRUE (1) if packet is available. Call it often to check packet availibility
    uint8_t cc1101_packet_available();

    // If packet is available, call this method to get the data from RXFIFO
    uint8_t cc1101_get_payload(uint8_t *rxbuffer, uint8_t *pktlen_rx,uint8_t *my_addr,
                                uint8_t *sender, int8_t *rssi_dbm, uint8_t *lqi);

    // Clear RXFIFO and TXFIFO
    void cc1101_rx_fifo_erase(uint8_t *rxbuffer);
    void cc1101_tx_fifo_erase(uint8_t *txbuffer);

    // Fill RXFIFO first with your data (8-bit chunks) and then send them using this method.
    // rx_addr must be same as the address of the receiver
    // tx_addr represents your address
    // pktlen is the length of the packet. Must be equal to or greater than your packet's length and shorter than the maximum TXFIFO size
    uint8_t cc1101_send_packet(uint8_t my_addr, uint8_t rx_addr, uint8_t *txbuffer, uint8_t pktlen, uint8_t tx_retries);

    // This method sets your address (rx or tx)
    void cc1101_set_myaddr(uint8_t addr);

    // Sets the channel. Rx and Tx must be in same channel.
    void cc1101_set_channel(uint8_t channel);

    // Sets the frequency you want to use. Pass the index number of the desired ISM band.
    void cc1101_set_ISM(uint8_t ism_freq);

    // Sets the modulation mode. Pass the index number.
    void cc1101_set_mode(uint8_t mode);

    // Sets the transmitter's output power in dBm. (-30dBm to 10dBm)
    void cc1101_set_output_power_level(int8_t dbm);

    *** To know all the available methods, check the CC1101_MSP430.h file
```


##### Thanks to
- [SpaceTeddy](https://github.com/SpaceTeddy)


#### License
Unless otherwise mentioned, all files that I have authored are licensed under The MIT License (MIT).


The MIT License (MIT)

Copyright (c) 2021 Avra Mitra

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
