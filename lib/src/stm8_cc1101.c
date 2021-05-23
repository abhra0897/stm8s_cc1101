#include "stm8_cc1101.h"

#include <stdint.h>
#include <string.h>

#define MINIMAL_MODE    1
static uint8_t cc1101_debug_level = 1;

//-------------------[global default settings 868 Mhz]-------------------
static const uint8_t CC1101_GFSK_1_2_kb[] = {
                    0x07,  // IOCFG2        GDO2 Output Pin Configuration
                    0x2E,  // IOCFG1        GDO1 Output Pin Configuration
                    0x80,  // IOCFG0        GDO0 Output Pin Configuration
                    0x07,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
                    0x57,  // SYNC1         Sync Word, High Byte
                    0x43,  // SYNC0         Sync Word, Low Byte
                    0x3E,  // PKTLEN        Packet Length
                    0x0E,  // PKTCTRL1      Packet Automation Control
                    0x45,  // PKTCTRL0      Packet Automation Control
                    0xFF,  // ADDR          Device Address
                    0x00,  // CHANNR        Channel Number
                    0x08,  // FSCTRL1       Frequency Synthesizer Control
                    0x00,  // FSCTRL0       Frequency Synthesizer Control
                    0x21,  // FREQ2         Frequency Control Word, High Byte
                    0x65,  // FREQ1         Frequency Control Word, Middle Byte
                    0x6A,  // FREQ0         Frequency Control Word, Low Byte
                    0xF5,  // MDMCFG4       Modem Configuration
                    0x83,  // MDMCFG3       Modem Configuration
                    0x13,  // MDMCFG2       Modem Configuration
                    0xA0,  // MDMCFG1       Modem Configuration
                    0xF8,  // MDMCFG0       Modem Configuration
                    0x15,  // DEVIATN       Modem Deviation Setting
                    0x07,  // MCSM2         Main Radio Control State Machine Configuration
                    0x0C,  // MCSM1         Main Radio Control State Machine Configuration
                    0x18,  // MCSM0         Main Radio Control State Machine Configuration
                    0x16,  // FOCCFG        Frequency Offset Compensation Configuration
                    0x6C,  // BSCFG         Bit Synchronization Configuration
                    0x03,  // AGCCTRL2      AGC Control
                    0x40,  // AGCCTRL1      AGC Control
                    0x91,  // AGCCTRL0      AGC Control
                    0x02,  // WOREVT1       High Byte Event0 Timeout
                    0x26,  // WOREVT0       Low Byte Event0 Timeout
                    0x09,  // WORCTRL       Wake On Radio Control
                    0x56,  // FREND1        Front End RX Configuration
                    0x17,  // FREND0        Front End TX Configuration
                    0xA9,  // FSCAL3        Frequency Synthesizer Calibration
                    0x0A,  // FSCAL2        Frequency Synthesizer Calibration
                    0x00,  // FSCAL1        Frequency Synthesizer Calibration
                    0x11,  // FSCAL0        Frequency Synthesizer Calibration
                    0x41,  // RCCTRL1       RC Oscillator Configuration
                    0x00,  // RCCTRL0       RC Oscillator Configuration
                    0x59,  // FSTEST        Frequency Synthesizer Calibration Control,
                    0x7F,  // PTEST         Production Test
                    0x3F,  // AGCTEST       AGC Test
                    0x81,  // TEST2         Various Test Settings
                    0x3F,  // TEST1         Various Test Settings
                    0x0B   // TEST0         Various Test Settings
                };

#if MINIMAL_MODE == 0
static const uint8_t CC1101_GFSK_38_4_kb[] = {
                    0x07,  // IOCFG2        GDO2 Output Pin Configuration
                    0x2E,  // IOCFG1        GDO1 Output Pin Configuration
                    0x80,  // IOCFG0        GDO0 Output Pin Configuration
                    0x07,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
                    0x57,  // SYNC1         Sync Word, High Byte
                    0x43,  // SYNC0         Sync Word, Low Byte
                    0x3E,  // PKTLEN        Packet Length
                    0x0E,  // PKTCTRL1      Packet Automation Control
                    0x45,  // PKTCTRL0      Packet Automation Control
                    0xFF,  // ADDR          Device Address
                    0x00,  // CHANNR        Channel Number
                    0x06,  // FSCTRL1       Frequency Synthesizer Control
                    0x00,  // FSCTRL0       Frequency Synthesizer Control
                    0x21,  // FREQ2         Frequency Control Word, High Byte
                    0x65,  // FREQ1         Frequency Control Word, Middle Byte
                    0x6A,  // FREQ0         Frequency Control Word, Low Byte
                    0xCA,  // MDMCFG4       Modem Configuration
                    0x83,  // MDMCFG3       Modem Configuration
                    0x13,  // MDMCFG2       Modem Configuration
                    0xA0,  // MDMCFG1       Modem Configuration
                    0xF8,  // MDMCFG0       Modem Configuration
                    0x34,  // DEVIATN       Modem Deviation Setting
                    0x07,  // MCSM2         Main Radio Control State Machine Configuration
                    0x0C,  // MCSM1         Main Radio Control State Machine Configuration
                    0x18,  // MCSM0         Main Radio Control State Machine Configuration
                    0x16,  // FOCCFG        Frequency Offset Compensation Configuration
                    0x6C,  // BSCFG         Bit Synchronization Configuration
                    0x43,  // AGCCTRL2      AGC Control
                    0x40,  // AGCCTRL1      AGC Control
                    0x91,  // AGCCTRL0      AGC Control
                    0x02,  // WOREVT1       High Byte Event0 Timeout
                    0x26,  // WOREVT0       Low Byte Event0 Timeout
                    0x09,  // WORCTRL       Wake On Radio Control
                    0x56,  // FREND1        Front End RX Configuration
                    0x17,  // FREND0        Front End TX Configuration
                    0xA9,  // FSCAL3        Frequency Synthesizer Calibration
                    0x0A,  // FSCAL2        Frequency Synthesizer Calibration
                    0x00,  // FSCAL1        Frequency Synthesizer Calibration
                    0x11,  // FSCAL0        Frequency Synthesizer Calibration
                    0x41,  // RCCTRL1       RC Oscillator Configuration
                    0x00,  // RCCTRL0       RC Oscillator Configuration
                    0x59,  // FSTEST        Frequency Synthesizer Calibration Control,
                    0x7F,  // PTEST         Production Test
                    0x3F,  // AGCTEST       AGC Test
                    0x81,  // TEST2         Various Test Settings
                    0x3F,  // TEST1         Various Test Settings
                    0x0B   // TEST0         Various Test Settings
                };
#endif

static const uint8_t CC1101_GFSK_100_kb[] = {
                    0x07,  // IOCFG2        GDO2 Output Pin Configuration
                    0x2E,  // IOCFG1        GDO1 Output Pin Configuration
                    0x80,  // IOCFG0        GDO0 Output Pin Configuration
                    0x07,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
                    0x57,  // SYNC1         Sync Word, High Byte
                    0x43,  // SYNC0         Sync Word, Low Byte
                    0x3E,  // PKTLEN        Packet Length
                    0x0E,  // PKTCTRL1      Packet Automation Control
                    0x45,  // PKTCTRL0      Packet Automation Control
                    0xFF,  // ADDR          Device Address
                    0x00,  // CHANNR        Channel Number
                    0x08,  // FSCTRL1       Frequency Synthesizer Control
                    0x00,  // FSCTRL0       Frequency Synthesizer Control
                    0x21,  // FREQ2         Frequency Control Word, High Byte
                    0x65,  // FREQ1         Frequency Control Word, Middle Byte
                    0x6A,  // FREQ0         Frequency Control Word, Low Byte
                    0x5B,  // MDMCFG4       Modem Configuration
                    0xF8,  // MDMCFG3       Modem Configuration
                    0x13,  // MDMCFG2       Modem Configuration
                    0xA0,  // MDMCFG1       Modem Configuration
                    0xF8,  // MDMCFG0       Modem Configuration
                    0x47,  // DEVIATN       Modem Deviation Setting
                    0x07,  // MCSM2         Main Radio Control State Machine Configuration
                    0x0C,  // MCSM1         Main Radio Control State Machine Configuration
                    0x18,  // MCSM0         Main Radio Control State Machine Configuration
                    0x1D,  // FOCCFG        Frequency Offset Compensation Configuration
                    0x1C,  // BSCFG         Bit Synchronization Configuration
                    0xC7,  // AGCCTRL2      AGC Control
                    0x00,  // AGCCTRL1      AGC Control
                    0xB2,  // AGCCTRL0      AGC Control
                    0x02,  // WOREVT1       High Byte Event0 Timeout
                    0x26,  // WOREVT0       Low Byte Event0 Timeout
                    0x09,  // WORCTRL       Wake On Radio Control
                    0xB6,  // FREND1        Front End RX Configuration
                    0x17,  // FREND0        Front End TX Configuration
                    0xEA,  // FSCAL3        Frequency Synthesizer Calibration
                    0x0A,  // FSCAL2        Frequency Synthesizer Calibration
                    0x00,  // FSCAL1        Frequency Synthesizer Calibration
                    0x11,  // FSCAL0        Frequency Synthesizer Calibration
                    0x41,  // RCCTRL1       RC Oscillator Configuration
                    0x00,  // RCCTRL0       RC Oscillator Configuration
                    0x59,  // FSTEST        Frequency Synthesizer Calibration Control,
                    0x7F,  // PTEST         Production Test
                    0x3F,  // AGCTEST       AGC Test
                    0x81,  // TEST2         Various Test Settings
                    0x3F,  // TEST1         Various Test Settings
                    0x0B   // TEST0         Various Test Settings
                };

#if MINIMAL_MODE == 0
static const uint8_t CC1101_MSK_250_kb[] = {
                    0x07,  // IOCFG2        GDO2 Output Pin Configuration
                    0x2E,  // IOCFG1        GDO1 Output Pin Configuration
                    0x80,  // IOCFG0        GDO0 Output Pin Configuration
                    0x07,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
                    0x57,  // SYNC1         Sync Word, High Byte
                    0x43,  // SYNC0         Sync Word, Low Byte
                    0x3E,  // PKTLEN        Packet Length
                    0x0E,  // PKTCTRL1      Packet Automation Control
                    0x45,  // PKTCTRL0      Packet Automation Control
                    0xFF,  // ADDR          Device Address
                    0x00,  // CHANNR        Channel Number
                    0x0B,  // FSCTRL1       Frequency Synthesizer Control
                    0x00,  // FSCTRL0       Frequency Synthesizer Control
                    0x21,  // FREQ2         Frequency Control Word, High Byte
                    0x65,  // FREQ1         Frequency Control Word, Middle Byte
                    0x6A,  // FREQ0         Frequency Control Word, Low Byte
                    0x2D,  // MDMCFG4       Modem Configuration
                    0x3B,  // MDMCFG3       Modem Configuration
                    0x73,  // MDMCFG2       Modem Configuration
                    0xA0,  // MDMCFG1       Modem Configuration
                    0xF8,  // MDMCFG0       Modem Configuration
                    0x00,  // DEVIATN       Modem Deviation Setting
                    0x07,  // MCSM2         Main Radio Control State Machine Configuration
                    0x0C,  // MCSM1         Main Radio Control State Machine Configuration
                    0x18,  // MCSM0         Main Radio Control State Machine Configuration
                    0x1D,  // FOCCFG        Frequency Offset Compensation Configuration
                    0x1C,  // BSCFG         Bit Synchronization Configuration
                    0xC7,  // AGCCTRL2      AGC Control
                    0x00,  // AGCCTRL1      AGC Control
                    0xB2,  // AGCCTRL0      AGC Control
                    0x02,  // WOREVT1       High Byte Event0 Timeout
                    0x26,  // WOREVT0       Low Byte Event0 Timeout
                    0x09,  // WORCTRL       Wake On Radio Control
                    0xB6,  // FREND1        Front End RX Configuration
                    0x17,  // FREND0        Front End TX Configuration
                    0xEA,  // FSCAL3        Frequency Synthesizer Calibration
                    0x0A,  // FSCAL2        Frequency Synthesizer Calibration
                    0x00,  // FSCAL1        Frequency Synthesizer Calibration
                    0x11,  // FSCAL0        Frequency Synthesizer Calibration
                    0x41,  // RCCTRL1       RC Oscillator Configuration
                    0x00,  // RCCTRL0       RC Oscillator Configuration
                    0x59,  // FSTEST        Frequency Synthesizer Calibration Control,
                    0x7F,  // PTEST         Production Test
                    0x3F,  // AGCTEST       AGC Test
                    0x81,  // TEST2         Various Test Settings
                    0x3F,  // TEST1         Various Test Settings
                    0x0B   // TEST0         Various Test Settings
                };
#endif

#if MINIMAL_MODE == 0
static const uint8_t CC1101_MSK_500_kb[] = {
                    0x07,  // IOCFG2        GDO2 Output Pin Configuration
                    0x2E,  // IOCFG1        GDO1 Output Pin Configuration
                    0x80,  // IOCFG0        GDO0 Output Pin Configuration
                    0x07,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
                    0x57,  // SYNC1         Sync Word, High Byte
                    0x43,  // SYNC0         Sync Word, Low Byte
                    0x3E,  // PKTLEN        Packet Length
                    0x0E,  // PKTCTRL1      Packet Automation Control
                    0x45,  // PKTCTRL0      Packet Automation Control
                    0xFF,  // ADDR          Device Address
                    0x00,  // CHANNR        Channel Number
                    0x0C,  // FSCTRL1       Frequency Synthesizer Control
                    0x00,  // FSCTRL0       Frequency Synthesizer Control
                    0x21,  // FREQ2         Frequency Control Word, High Byte
                    0x65,  // FREQ1         Frequency Control Word, Middle Byte
                    0x6A,  // FREQ0         Frequency Control Word, Low Byte
                    0x0E,  // MDMCFG4       Modem Configuration
                    0x3B,  // MDMCFG3       Modem Configuration
                    0x73,  // MDMCFG2       Modem Configuration
                    0xA0,  // MDMCFG1       Modem Configuration
                    0xF8,  // MDMCFG0       Modem Configuration
                    0x00,  // DEVIATN       Modem Deviation Setting
                    0x07,  // MCSM2         Main Radio Control State Machine Configuration
                    0x0C,  // MCSM1         Main Radio Control State Machine Configuration
                    0x18,  // MCSM0         Main Radio Control State Machine Configuration
                    0x1D,  // FOCCFG        Frequency Offset Compensation Configuration
                    0x1C,  // BSCFG         Bit Synchronization Configuration
                    0xC7,  // AGCCTRL2      AGC Control
                    0x40,  // AGCCTRL1      AGC Control
                    0xB2,  // AGCCTRL0      AGC Control
                    0x02,  // WOREVT1       High Byte Event0 Timeout
                    0x26,  // WOREVT0       Low Byte Event0 Timeout
                    0x09,  // WORCTRL       Wake On Radio Control
                    0xB6,  // FREND1        Front End RX Configuration
                    0x17,  // FREND0        Front End TX Configuration
                    0xEA,  // FSCAL3        Frequency Synthesizer Calibration
                    0x0A,  // FSCAL2        Frequency Synthesizer Calibration
                    0x00,  // FSCAL1        Frequency Synthesizer Calibration
                    0x19,  // FSCAL0        Frequency Synthesizer Calibration
                    0x41,  // RCCTRL1       RC Oscillator Configuration
                    0x00,  // RCCTRL0       RC Oscillator Configuration
                    0x59,  // FSTEST        Frequency Synthesizer Calibration Control,
                    0x7F,  // PTEST         Production Test
                    0x3F,  // AGCTEST       AGC Test
                    0x81,  // TEST2         Various Test Settings
                    0x3F,  // TEST1         Various Test Settings
                    0x0B   // TEST0         Various Test Settings
                };
#endif

#if MINIMAL_MODE == 0
static const uint8_t CC1101_OOK_4_8_kb[] = {
                    0x06,  // IOCFG2        GDO2 Output Pin Configuration
                    0x2E,  // IOCFG1        GDO1 Output Pin Configuration
                    0x06,  // IOCFG0        GDO0 Output Pin Configuration
                    0x47,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
                    0x57,  // SYNC1         Sync Word, High Byte
                    0x43,  // SYNC0         Sync Word, Low Byte
                    0xFF,  // PKTLEN        Packet Length
                    0x04,  // PKTCTRL1      Packet Automation Control
                    0x05,  // PKTCTRL0      Packet Automation Control
                    0x00,  // ADDR          Device Address
                    0x00,  // CHANNR        Channel Number
                    0x06,  // FSCTRL1       Frequency Synthesizer Control
                    0x00,  // FSCTRL0       Frequency Synthesizer Control
                    0x21,  // FREQ2         Frequency Control Word, High Byte
                    0x65,  // FREQ1         Frequency Control Word, Middle Byte
                    0x6A,  // FREQ0         Frequency Control Word, Low Byte
                    0x87,  // MDMCFG4       Modem Configuration
                    0x83,  // MDMCFG3       Modem Configuration
                    0x3B,  // MDMCFG2       Modem Configuration
                    0x22,  // MDMCFG1       Modem Configuration
                    0xF8,  // MDMCFG0       Modem Configuration
                    0x15,  // DEVIATN       Modem Deviation Setting
                    0x07,  // MCSM2         Main Radio Control State Machine Configuration
                    0x30,  // MCSM1         Main Radio Control State Machine Configuration
                    0x18,  // MCSM0         Main Radio Control State Machine Configuration
                    0x14,  // FOCCFG        Frequency Offset Compensation Configuration
                    0x6C,  // BSCFG         Bit Synchronization Configuration
                    0x07,  // AGCCTRL2      AGC Control
                    0x00,  // AGCCTRL1      AGC Control
                    0x92,  // AGCCTRL0      AGC Control
                    0x87,  // WOREVT1       High Byte Event0 Timeout
                    0x6B,  // WOREVT0       Low Byte Event0 Timeout
                    0xFB,  // WORCTRL       Wake On Radio Control
                    0x56,  // FREND1        Front End RX Configuration
                    0x17,  // FREND0        Front End TX Configuration
                    0xE9,  // FSCAL3        Frequency Synthesizer Calibration
                    0x2A,  // FSCAL2        Frequency Synthesizer Calibration
                    0x00,  // FSCAL1        Frequency Synthesizer Calibration
                    0x1F,  // FSCAL0        Frequency Synthesizer Calibration
                    0x41,  // RCCTRL1       RC Oscillator Configuration
                    0x00,  // RCCTRL0       RC Oscillator Configuration
                    0x59,  // FSTEST        Frequency Synthesizer Calibration Control
                    0x7F,  // PTEST         Production Test
                    0x3F,  // AGCTEST       AGC Test
                    0x81,  // TEST2         Various Test Settings
                    0x35,  // TEST1         Various Test Settings
                    0x09,  // TEST0         Various Test Settings
                };
#endif
                           //Patable index: -30  -20- -15  -10   0    5    7    10 dBm



static const uint8_t patable_power_315[] = {0x17,0x1D,0x26,0x69,0x51,0x86,0xCC,0xC3};
static const uint8_t patable_power_433[] = {0x6C,0x1C,0x06,0x3A,0x51,0x85,0xC8,0xC0};
static const uint8_t patable_power_868[] = {0x03,0x17,0x1D,0x26,0x50,0x86,0xCD,0xC0};
static const uint8_t patable_power_915[] = {0x0B,0x1B,0x6D,0x67,0x50,0x85,0xC9,0xC1};
//static uint8_t patable_power_2430[] = {0x44,0x84,0x46,0x55,0xC6,0x6E,0x9A,0xFE};

//----------------------------------[END]---------------------------------------

//-------------------------[CC1101 cc1101_reset function]------------------------------
void cc1101_reset(void)                  // cc1101_reset defined in CC1101 datasheet
{
    SPI_CS_ACTIVE();
    delay_us(10);
    SPI_CS_IDLE();
    delay_us(40);

    cc1101_spi_write_strobe(SRES);
    delay_ms(1);
}
//-----------------------------[END]--------------------------------------------

//------------------------[set Power Down]--------------------------------------
void cc1101_powerdown(void)
{
    cc1101_sidle();
    cc1101_spi_write_strobe(SPWD);               // CC1101 Power Down
}
//-----------------------------[end]--------------------------------------------

/*
//---------------------------[WakeUp]-------------------------------------------
void cc1101_wakeup(void)
{
    SPI_CS_ACTIVE();
    delay_us(10);
    SPI_CS_IDLE();
    delay_us(10);
    cc1101_receive();                            // go to RX Mode
}

//-----------------------------[end]--------------------------------------------
*/

//---------------------[CC1101 set debug level]---------------------------------
uint8_t cc1101_set_debug_level(uint8_t cc1101_set_debug_level)  //default OFF
{
    cc1101_debug_level = cc1101_set_debug_level;        //set debug level of CC1101 outputs

    return cc1101_debug_level;
}
//-----------------------------[end]--------------------------------------------

//---------------------[CC1101 get debug level]---------------------------------
uint8_t cc1101_get_debug_level(void)
{
    return cc1101_debug_level;
}
//-----------------------------[end]--------------------------------------------

//----------------------[CC1101 init functions]---------------------------------
uint8_t cc1101_begin(uint8_t *My_addr)
{
    uint8_t CC1101_freq_select, CC1101_mode_select, CC1101_channel_select;
    uint8_t partnum, version;

    cc1101_GDO_set();                            //setup AVR GPIO ports

    cc1101_set_debug_level(cc1101_set_debug_level(1));   //set debug level of CC1101 outputs

    #if MINIMAL_MODE == 0
    if(cc1101_debug_level > 0)
    {
        uart_println("Init CC1101...");
    }
    #endif

    cc1101_spi_begin();                          //inits SPI Interface
    cc1101_reset();                              //CC1101 init cc1101_reset

    cc1101_spi_write_strobe(SFTX);delay_us(100);//flush the TX_fifo content
    cc1101_spi_write_strobe(SFRX);delay_us(100);//flush the RX_fifo content

    partnum = cc1101_spi_read_register(PARTNUM); //reads CC1101 partnumber
    version = cc1101_spi_read_register(VERSION); //reads CC1101 version number

    //checks if valid Chip ID is found. Usualy 0x03 or 0x14. if not -> abort
    if(version == 0x00 || version == 0xFF)
    {
        #if MINIMAL_MODE == 0
        if(cc1101_debug_level > 0)
        {
            uart_print("no CC11xx found!");
            uart_println("");
        }
        #endif
        return FALSE;
    }

    #if MINIMAL_MODE == 0
    if(cc1101_debug_level > 0)
    {
        uart_print("Partnumber:");
        cc1101_uart_puthex_byte(partnum);
        uart_println("");

        uart_print("Version:");
        cc1101_uart_puthex_byte(version);
        uart_println("");
    }
    #endif

    //default settings
    *My_addr = 0x00;
    CC1101_freq_select = 0x02;    //433.92MHz
    CC1101_mode_select = 0x01;    //gfsk 1.2kbps
    CC1101_channel_select = 0x01;


    //set modulation mode
    cc1101_set_mode(CC1101_mode_select);

    //set ISM band
    cc1101_set_ISM(CC1101_freq_select);

    //set channel
    cc1101_set_channel(CC1101_channel_select);

    //set output power amplifier
    cc1101_set_output_power_level(0);            //set PA to 0dBm as default

    //set my receiver address
    cc1101_set_myaddr(*My_addr);                  //set addr

    #if MINIMAL_MODE == 0
    if(cc1101_debug_level > 0)
    {
        uart_println("...done");
    }
    #endif

    cc1101_receive();                            //set CC1101 in cc1101_receive mode

    return TRUE;
}
//-------------------------------[end]------------------------------------------

//-----------------[finish's the CC1101 operation]------------------------------
void cc1101_end(void)
{
    cc1101_powerdown();                          //power down CC1101
    cc1101_spi_end();                            //disable SPI Interface (Does nothing for MSP430)
}
//-------------------------------[end]------------------------------------------

//-----------------------[show all CC1101 registers]----------------------------
void cc1101_show_register_settings(void)
{
    #if MINIMAL_MODE == 0
    if(cc1101_debug_level > 0)
    {
        uint8_t config_reg_verify[CFG_REGISTER],Patable_verify[CFG_REGISTER];

        cc1101_spi_read_burst(READ_BURST,config_reg_verify,CFG_REGISTER);  //reads all 47 config register
        cc1101_spi_read_burst(PATABLE_BURST,Patable_verify,8);             //reads output power settings
        uart_println("Cfg_reg:");

        for(uint8_t i = 0 ; i < CFG_REGISTER; i++)  //showes rx_buffer for debug
            {
                cc1101_uart_puthex_byte(config_reg_verify[i]);uart_print(" ");
                if(i == 9 || i == 19 || i == 29 || i == 39) //just for beautiful output style
                    {
                        uart_println("");
                    }
            }
            uart_println("");
            uart_println("PaTable:");

            for(uint8_t i = 0 ; i < 8; i++)         //showes rx_buffer for debug
                {
                    cc1101_uart_puthex_byte(Patable_verify[i]);uart_print(" ");
                }
        uart_println("");
    }
    #endif
}
//-------------------------------[end]------------------------------------------


//----------------------------[idle mode]---------------------------------------
uint8_t cc1101_sidle(void)
{
    uint8_t marcstate;

    cc1101_spi_write_strobe(SIDLE);              //sets to idle first. must be in

    marcstate = 0xFF;                     //set unknown/dummy state value

    while(marcstate != 0x01)              //0x01 = cc1101_sidle
    {
        marcstate = (cc1101_spi_read_register(MARCSTATE) & 0x1F); //read out state of CC1101 to be sure in RX
        //cc1101_uart_puthex_byte(marcstate);
    }
    //uart_println("");
    delay_us(100);
    return TRUE;
}
//-------------------------------[end]------------------------------------------

//---------------------------[cc1101_transmit mode]------------------------------------
uint8_t cc1101_transmit(void)
{
    uint8_t marcstate;

    cc1101_sidle();                              //sets to idle first.
    cc1101_spi_write_strobe(STX);                //sends the data over air

    marcstate = 0xFF;                     //set unknown/dummy state value

    while(marcstate != 0x01)              //0x01 = ILDE after sending data
    {
        marcstate = (cc1101_spi_read_register(MARCSTATE) & 0x1F); //read out state of CC1101 to be sure in IDLE and TX is finished
        //cc1101_uart_puthex_byte(marcstate);
    }
    //uart_println("");
    delay_us(100);
    return TRUE;
}
//-------------------------------[end]------------------------------------------

//---------------------------[cc1101_receive mode]-------------------------------------
uint8_t cc1101_receive(void)
{
    uint8_t marcstate;

    cc1101_sidle();                              //sets to idle first.
    cc1101_spi_write_strobe(SRX);                //writes cc1101_receive strobe (cc1101_receive mode)

    marcstate = 0xFF;                     //set unknown/dummy state value

    while(marcstate != 0x0D)              //0x0D = RX
    {
        marcstate = (cc1101_spi_read_register(MARCSTATE) & 0x1F); //read out state of CC1101 to be sure in RX
        //cc1101_uart_puthex_byte(marcstate);
    }
    //uart_println("");
    delay_us(100);
    return TRUE;
}
//-------------------------------[end]------------------------------------------

//------------[enables WOR Mode  EVENT0 ~1890ms; rx_timeout ~235ms]--------------------
void cc1101_wor_enable()
{
/*
    EVENT1 = WORCTRL[6:4] -> Datasheet page 88
    EVENT0 = (750/Xtal)*(WOREVT1<<8+WOREVT0)*2^(5*WOR_RES) = (750/26Meg)*65407*2^(5*0) = 1.89s

                        (WOR_RES=0;RX_TIME=0)               -> Datasheet page 80
i.E RX_TIMEOUT = EVENT0*       (3.6038)      *26/26Meg = 235.8ms
                        (WOR_RES=0;RX_TIME=1)               -> Datasheet page 80
i.E.RX_TIMEOUT = EVENT0*       (1.8029)      *26/26Meg = 117.9ms
*/
    cc1101_sidle();

    cc1101_spi_write_register(MCSM0, 0x18);    //FS Autocalibration
    cc1101_spi_write_register(MCSM2, 0x01);    //MCSM2.RX_TIME = 1b

    // configure EVENT0 time
    cc1101_spi_write_register(WOREVT1, 0xFF);  //High byte Event0 timeout
    cc1101_spi_write_register(WOREVT0, 0x7F);  //Low byte Event0 timeout

    // configure EVENT1 time
    cc1101_spi_write_register(WORCTRL, 0x78);  //WOR_RES=0b; tEVENT1=0111b=48d -> 48*(750/26MHz)= 1.385ms

    cc1101_spi_write_strobe(SFRX);             //flush RX buffer
    cc1101_spi_write_strobe(SWORRST);          //resets the WOR timer to the programmed Event 1
    cc1101_spi_write_strobe(SWOR);             //put the radio in WOR mode when CSn is released

    delay_us(100);
}
//-------------------------------[end]------------------------------------------

//------------------------[disable WOR Mode]-------------------------------------
void cc1101_wor_disable()
{
    cc1101_sidle();                            //exit WOR Mode
    cc1101_spi_write_register(MCSM2, 0x07);    //stay in RX. No RX timeout
}
//-------------------------------[end]------------------------------------------

//------------------------[resets WOR Timer]------------------------------------
void cc1101_wor_reset()
{
    cc1101_sidle();                            //go to IDLE
    cc1101_spi_write_register(MCSM2, 0x01);    //MCSM2.RX_TIME = 1b
    cc1101_spi_write_strobe(SFRX);             //flush RX buffer
    cc1101_spi_write_strobe(SWORRST);          //resets the WOR timer to the programmed Event 1
    cc1101_spi_write_strobe(SWOR);             //put the radio in WOR mode when CSn is released

    delay_us(100);
}
//-------------------------------[end]------------------------------------------

//-------------------------[cc1101_tx_payload_burst]-----------------------------------
uint8_t cc1101_tx_payload_burst(uint8_t my_addr, uint8_t rx_addr,
                              uint8_t *txbuffer, uint8_t length)
{
    txbuffer[0] = length - 1;
    txbuffer[1] = rx_addr;
    txbuffer[2] = my_addr;

    cc1101_spi_write_burst(TXFIFO_BURST, txbuffer, length); //writes TX_Buffer +1 because of pktlen must be also transfered

    #if MINIMAL_MODE == 0
    if(cc1101_debug_level > 0)
    {
        uart_print("TX_FIFO:");
        for(uint8_t i = 0 ; i < length; i++)       //TX_fifo debug out
        {
            cc1101_uart_puthex_byte(txbuffer[i]);
        }
        uart_println("");
    }
    #endif
  return TRUE;
}
//-------------------------------[end]------------------------------------------

//------------------[cc1101_rx_payload_burst - package received]-----------------------
uint8_t cc1101_rx_payload_burst(uint8_t *rxbuffer, uint8_t *pktlen)
{
    uint8_t bytes_in_RXFIFO = 0;
    uint8_t res = 0;

    bytes_in_RXFIFO = cc1101_spi_read_register(RXBYTES);              //reads the number of bytes in RXFIFO

    if((bytes_in_RXFIFO & 0x7F) && !(bytes_in_RXFIFO & 0x80))  //if bytes in buffer and no RX Overflow
    {
        cc1101_spi_read_burst(RXFIFO_BURST, rxbuffer, bytes_in_RXFIFO);
        *pktlen = rxbuffer[0];
        res = TRUE;
    }
    else
    {
        #if MINIMAL_MODE == 0
        if(cc1101_debug_level > 0)
        {
            uart_print("no bytes in RX buffer or RX Overflow!: ");
            cc1101_uart_puti(bytes_in_RXFIFO);
        }
        #endif
        cc1101_sidle();                                                  //set to IDLE
        cc1101_spi_write_strobe(SFRX);delay_us(100);            //flush RX Buffer
        cc1101_receive();                                                //set to cc1101_receive mode
        res = FALSE;
    }

    return res;
}
//-------------------------------[end]------------------------------------------

//---------------------------[send packet]--------------------------------------
uint8_t cc1101_send_packet(uint8_t my_addr, uint8_t rx_addr, uint8_t *txbuffer,
                            uint8_t pktlen, uint8_t tx_retries)
{
    uint8_t pktlen_ack = 0;                                         //default package len for ACK
    uint8_t rxbuffer[FIFOBUFFER] = {0};
    uint8_t tx_retries_count = 0;
    uint8_t from_sender = 0;
    uint16_t ackWaitCounter = 0;

    if(pktlen > (FIFOBUFFER - 1))                               //FIFO overflow check
    {
        uart_println("ERROR: package size overflow\r\n");
        return FALSE;
    }

    do 
    {
        cc1101_tx_payload_burst(my_addr, rx_addr, txbuffer, pktlen);   //loads the data in CC1101 buffer
        cc1101_transmit();                                             //sends data over air
        cc1101_receive();                                              //cc1101_receive mode

        if(rx_addr == BROADCAST_ADDRESS)
        {                       
            //no wait acknowledge if send to broadcast address or tx_retries = 0
            return TRUE;                                        //successful send to BROADCAST_ADDRESS
        }

        while (ackWaitCounter < ACK_TIMEOUT )                   //wait for an acknowledge
        {
            if (cc1101_packet_available() == TRUE)                     //if RF package received check package acknowge
            {
                from_sender = rx_addr;                          //the original message sender address
                cc1101_rx_fifo_erase(rxbuffer);                        //erase RX software buffer
                cc1101_rx_payload_burst(rxbuffer, &pktlen_ack);         //reads package in buffer
                cc1101_check_acknowledge(rxbuffer, pktlen_ack, from_sender, my_addr); //check if received message is an acknowledge from client
                return TRUE;                                    //package successfully send
            }
            else
            {
                ackWaitCounter++;                               //increment ACK wait counter
                delay_ms(1);                                    //delay_ms to give receiver time
            }
        }

        ackWaitCounter = 0;                                     //resets the ACK_Timeout
        tx_retries_count++;                                     //increase tx retry counter

        #if MINIMAL_MODE == 0
        if(cc1101_debug_level > 0)
        {                                    //debug output messages
            uart_print(" #:");
            cc1101_uart_puthex_byte(tx_retries_count-1);
            uart_println("");
        }
        #endif
    }	
    while(tx_retries_count <= tx_retries);                     //while count of retries is reaches

    return FALSE;                                              //send failed. too many retries
}
//-------------------------------[end]------------------------------------------

//--------------------------[send ACKNOWLEDGE]------------------------------------
void cc1101_send_acknowledge(uint8_t my_addr, uint8_t tx_addr)
{
    uint8_t pktlen = 0x06;                                      //complete Pktlen for ACK packet
    uint8_t tx_buffer[0x06];                                    //tx buffer array init

    tx_buffer[3] = 'A'; tx_buffer[4] = 'c'; tx_buffer[5] = 'k'; //fill buffer with ACK Payload

    cc1101_tx_payload_burst(my_addr, tx_addr, tx_buffer, pktlen);      //load payload to CC1101
    cc1101_transmit();                                                 //send package over the air
    cc1101_receive();                                                  //set CC1101 in cc1101_receive mode

    #if MINIMAL_MODE == 0
    if(cc1101_debug_level > 0)
    {                                        //debut output
        uart_println("Ack_send!");
    }
    #endif
}
//-------------------------------[end]------------------------------------------
//----------------------[check if Packet is received]---------------------------
uint8_t cc1101_packet_available()
{
    if(GDO2_PIN_IS_HIGH())                           //if RF package received
    {
        if(cc1101_spi_read_register(IOCFG2) == 0x06)               //if sync word detect mode is used
        {
            while(GDO2_PIN_IS_HIGH())
            {   //wait till sync word is fully received
                //uart_println("!"));
            }
        }
        return TRUE;
    }
    return FALSE;
}
//-------------------------------[end]------------------------------------------

//------------------[check Payload for ACK or Data]-----------------------------
uint8_t cc1101_get_payload(uint8_t *rxbuffer, uint8_t *pktlen, uint8_t *my_addr,
                               uint8_t *sender, int8_t *rssi_dbm, uint8_t *lqi)
{
    uint8_t crc;

    cc1101_rx_fifo_erase(rxbuffer);                               //delete rx_fifo bufffer

    if(cc1101_rx_payload_burst(rxbuffer, pktlen) == FALSE)        //read package in buffer
    {
        cc1101_rx_fifo_erase(rxbuffer);                           //delete rx_fifo bufffer
        return FALSE;                                    //exit
    }
    else
    {
        *my_addr = rxbuffer[1];                             //set receiver address to my_addr
        *sender = rxbuffer[2];

        if(cc1101_check_acknowledge(rxbuffer, *pktlen, *sender, *my_addr) == TRUE) //acknowlage received?
        {
            cc1101_rx_fifo_erase(rxbuffer);                       //delete rx_fifo bufffer
            return FALSE;                                //Ack received -> finished
        }
        else                                               //real data, and send acknowladge
        {
            *rssi_dbm = cc1101_rssi_convert(rxbuffer[*pktlen + 1]); //converts receiver strength to dBm
            *lqi = cc1101_lqi_convert(rxbuffer[*pktlen + 2]);       //get rf quialtiy indicator
            crc = cc1101_check_crc(*lqi);                          //get packet CRC

            #if MINIMAL_MODE == 0
            if(cc1101_debug_level > 0)
            {                           //debug output messages
                if(rxbuffer[1] == BROADCAST_ADDRESS)       //if my receiver address is BROADCAST_ADDRESS
                {
                    uart_println("BROADCAST message");
                }

                uart_print("RX_FIFO:");
                for(uint8_t i = 0 ; i < *pktlen + 1; i++)   //showes rx_buffer for debug
                {
                    cc1101_uart_puthex_byte(rxbuffer[i]);
                }
                uart_print(" |");
                cc1101_uart_puthex_byte(rxbuffer[*pktlen + 1]);
                cc1101_uart_puthex_byte(rxbuffer[*pktlen + 2]);
                uart_print("|");
                uart_println("");

                uart_print("RSSI:");cc1101_uart_puti(*rssi_dbm);uart_print(" ");
                uart_print("LQI:");cc1101_uart_puthex_byte(*lqi);uart_print(" ");
                uart_print("CRC:");cc1101_uart_puthex_byte(crc);
                uart_println("");
            }
            #endif

            *my_addr = rxbuffer[1];                         //set receiver address to my_addr
            *sender = rxbuffer[2];                          //set from_sender address

            if(my_addr != BROADCAST_ADDRESS)               //send only ack if no BROADCAST_ADDRESS
            {
                cc1101_send_acknowledge(*my_addr, *sender);           //sending acknowledge to sender!
            }

            return TRUE;
        }
        return FALSE;
    }
}
//-------------------------------[end]------------------------------------------

//-------------------------[check ACKNOWLEDGE]------------------------------------
uint8_t cc1101_check_acknowledge(uint8_t *rxbuffer, uint8_t pktlen, uint8_t sender, uint8_t my_addr)
{
    int8_t rssi_dbm;
    uint8_t crc, lqi;

    if((pktlen == 0x05 && \
       (rxbuffer[1] == my_addr || rxbuffer[1] == BROADCAST_ADDRESS)) && \
        rxbuffer[2] == sender && \
        rxbuffer[3] == 'A' && rxbuffer[4] == 'c' && rxbuffer[5] == 'k') //acknowledge received!
        {
            if(rxbuffer[1] == BROADCAST_ADDRESS)
            {                           
                //if receiver address BROADCAST_ADDRESS skip acknowledge
                #if MINIMAL_MODE == 0
                if(cc1101_debug_level > 0)
                {
                    uart_println("BROADCAST ACK");
                }
                #endif
                return FALSE;
            }
            rssi_dbm = cc1101_rssi_convert(rxbuffer[pktlen + 1]);
            lqi = cc1101_lqi_convert(rxbuffer[pktlen + 2]);
            crc = cc1101_check_crc(lqi);

            #if MINIMAL_MODE == 0
            if(cc1101_debug_level > 0)
            {
                //uart_println("");
                uart_print("ACK! ");
                uart_print("RSSI:");cc1101_uart_puti(rssi_dbm);uart_print(" ");
                uart_print("LQI:");cc1101_uart_puthex_byte(lqi);uart_print(" ");
                uart_print("CRC:");cc1101_uart_puthex_byte(crc);
                uart_println("");
            }
            #endif
            return TRUE;
        }
    return FALSE;
}
//-------------------------------[end]------------------------------------------

//------------[check if Packet is received within defined time in ms]-----------
uint8_t cc1101_wait_for_packet(uint16_t milliseconds)
{
    for(uint16_t i = 0; i < milliseconds; i++)
        {
            delay_ms(1);                 //delay_ms till system has data available
            if (cc1101_packet_available())
            {
                return TRUE;
            }
        }
    //uart_println("no packet received!"));
    return FALSE;
}
//-------------------------------[end]------------------------------------------

//--------------------------[cc1101_tx_fifo_erase]-------------------------------------
void cc1101_tx_fifo_erase(uint8_t *txbuffer)
{
    memset(txbuffer, 0, sizeof(FIFOBUFFER));  //erased the TX_fifo array content to "0"
}
//-------------------------------[end]------------------------------------------

//--------------------------[cc1101_rx_fifo_erase]-------------------------------------
void cc1101_rx_fifo_erase(uint8_t *rxbuffer)
{
    memset(rxbuffer, 0, sizeof(FIFOBUFFER)); //erased the RX_fifo array content to "0"
}
//-------------------------------[end]------------------------------------------

//------------------------[set CC1101 address]----------------------------------
void cc1101_set_myaddr(uint8_t addr)
{
    cc1101_spi_write_register(ADDR,addr);          //stores MyAddr in the CC1101
}
//-------------------------------[end]------------------------------------------

//---------------------------[set channel]--------------------------------------
void cc1101_set_channel(uint8_t channel)
{
    cc1101_spi_write_register(CHANNR,channel);   //stores the new channel # in the CC1101
}
//-------------------------------[end]------------------------------------------

//-[set modulation mode 1 = GFSK_1_2_kb; 2 = GFSK_38_4_kb; 3 = GFSK_100_kb; 4 = MSK_250_kb; 5 = MSK_500_kb; 6 = OOK_4_8_kb]-
void cc1101_set_mode(uint8_t mode)
{
    switch (mode)
    {
        case 0x01:
                    cc1101_spi_write_burst(WRITE_BURST, (uint8_t*)CC1101_GFSK_1_2_kb,CFG_REGISTER);                            //writes all 47 config register
                    break;
        #if MINIMAL_MODE == 0
        case 0x02:
                    
                    cc1101_spi_write_burst(WRITE_BURST, (uint8_t*)CC1101_GFSK_38_4_kb,CFG_REGISTER);                            //writes all 47 config register
                    break;
        #endif
        case 0x03:
                    cc1101_spi_write_burst(WRITE_BURST, (uint8_t*)CC1101_GFSK_100_kb,CFG_REGISTER);
                    break;
        #if MINIMAL_MODE == 0
        case 0x04:
                    cc1101_spi_write_burst(WRITE_BURST, (uint8_t*)CC1101_MSK_250_kb,CFG_REGISTER);
                    break;
        #endif
        #if MINIMAL_MODE == 0
        case 0x05:
                    cc1101_spi_write_burst(WRITE_BURST, (uint8_t*)CC1101_MSK_500_kb,CFG_REGISTER);
                    break;
        #endif
        #if MINIMAL_MODE == 0
        case 0x06:
                    cc1101_spi_write_burst(WRITE_BURST, (uint8_t*)CC1101_OOK_4_8_kb,CFG_REGISTER);
                    break;
        #endif
        default:
                    cc1101_spi_write_burst(WRITE_BURST, (uint8_t*)CC1101_GFSK_1_2_kb,CFG_REGISTER);
                    mode = 0x02;
                    break;
    }
}
//-------------------------------[end]------------------------------------------

//---------[set ISM Band 1=315MHz; 2=433MHz; 3=868MHz; 4=915MHz]----------------
void cc1101_set_ISM(uint8_t ism_freq)
{
    uint8_t freq2, freq1, freq0;

    switch (ism_freq)                                                       //loads the RF freq which is defined in CC1101_freq_select
    {
        #if MINIMAL_MODE == 0
        case 0x01:                                                          //315MHz
                    freq2 = 0x0C;
                    freq1 = 0x1D;
                    freq0 = 0x89;
                    cc1101_spi_write_register(FREQ2, freq2);                                         //stores the new freq setting for defined ISM band
                    cc1101_spi_write_register(FREQ1, freq1);
                    cc1101_spi_write_register(FREQ0, freq0);
                    cc1101_spi_write_burst(PATABLE_BURST, (uint8_t*)patable_power_315, 8);                                //writes output power settings to CC1101

                    break;
        #endif
        case 0x02:                                                          //433.92MHz
                    freq2 = 0x10;
                    freq1 = 0xB0;
                    freq0 = 0x71;
                    cc1101_spi_write_register(FREQ2, freq2);                                         //stores the new freq setting for defined ISM band
                    cc1101_spi_write_register(FREQ1, freq1);
                    cc1101_spi_write_register(FREQ0, freq0);
                    cc1101_spi_write_burst(PATABLE_BURST,  (uint8_t*)patable_power_433, 8);                                //writes output power settings to CC1101

                    break;
        case 0x03:                                                          //868.3MHz
                    freq2 = 0x21;
                    freq1 = 0x65;
                    freq0 = 0x6A;
                    cc1101_spi_write_register(FREQ2, freq2);                                         //stores the new freq setting for defined ISM band
                    cc1101_spi_write_register(FREQ1, freq1);
                    cc1101_spi_write_register(FREQ0, freq0);
                    cc1101_spi_write_burst(PATABLE_BURST,  (uint8_t*)patable_power_868, 8);                                //writes output power settings to CC1101

                    break;
        #if MINIMAL_MODE == 0
        case 0x04:                                                          //915MHz
                    freq2 = 0x23;
                    freq1 = 0x31;
                    freq0 = 0x3B;
                    cc1101_spi_write_register(FREQ2, freq2);                                         //stores the new freq setting for defined ISM band
                    cc1101_spi_write_register(FREQ1, freq1);
                    cc1101_spi_write_register(FREQ0, freq0);
                    cc1101_spi_write_burst(PATABLE_BURST,  (uint8_t*)patable_power_915, 8);                                //writes output power settings to CC1101

                    break;
        #endif
        default:                                                             //default is 868.3MHz
                    freq2 = 0x21;
                    freq1 = 0x65;
                    freq0 = 0x6A;
                    cc1101_spi_write_register(FREQ2, freq2);                                         //stores the new freq setting for defined ISM band
                    cc1101_spi_write_register(FREQ1, freq1);
                    cc1101_spi_write_register(FREQ0, freq0);
                    cc1101_spi_write_burst(PATABLE_BURST,  (uint8_t*)patable_power_868, 8);                                //writes output power settings to CC1101

                    ism_freq = 0x03;
                    break;
    }
}
//-------------------------------[end]------------------------------------------

//---------------------------[set PATABLE]--------------------------------------
void cc1101_set_patable(uint8_t *patable_arr)
{
    cc1101_spi_write_burst(PATABLE_BURST, patable_arr, 8);   //writes output power settings to CC1101    "104us"
}
//-------------------------------[end]------------------------------------------

//-------------------------[set output power]-----------------------------------
void cc1101_set_output_power_level(int8_t dBm)
{
    uint8_t pa = 0xC0;

    if      (dBm <= -30) pa = 0x00;
    else if (dBm <= -20) pa = 0x01;
    else if (dBm <= -15) pa = 0x02;
    else if (dBm <= -10) pa = 0x03;
    else if (dBm <= 0)   pa = 0x04;
    else if (dBm <= 5)   pa = 0x05;
    else if (dBm <= 7)   pa = 0x06;
    else if (dBm <= 10)  pa = 0x07;

    cc1101_spi_write_register(FREND0, pa);
}
//-------------------------------[end]------------------------------------------


//-------[set Modulation type 2-FSK=0; GFSK=1; ASK/OOK=3; 4-FSK=4; MSK=7]------
#if MINIMAL_MODE == 0
void cc1101_set_modulation_type(uint8_t cfg)
{
    uint8_t data;
    data = cc1101_spi_read_register(MDMCFG2);
    data = (data & 0x8F) | (((cfg) << 4) & 0x70);
    cc1101_spi_write_register(MDMCFG2, data);
    //Serial.printl("MDMCFG2: 0x")
    //uart_println(data);
}
#endif
//-------------------------------[end]-----------------------------------------

//------------------------[set preamble Len]-----------------------------------
#if MINIMAL_MODE == 0
void cc1101_set_preamble_len(uint8_t cfg)
{
    uint8_t data;
    data = cc1101_spi_read_register(MDMCFG1);
    data = (data & 0x8F) | (((cfg) << 4) & 0x70);
    cc1101_spi_write_register(MDMCFG1, data);
    //Serial.printl("MDMCFG2: 0x");
    //uart_println(data);
}
#endif
//-------------------------------[end]-----------------------------------------

//-------------------[set modem datarate and deviant]--------------------------
#if MINIMAL_MODE == 0
void cc1101_set_datarate(uint8_t mdmcfg4, uint8_t mdmcfg3, uint8_t deviant)
{
    cc1101_spi_write_register(MDMCFG4, mdmcfg4);
    cc1101_spi_write_register(MDMCFG3, mdmcfg3);
    cc1101_spi_write_register(DEVIATN, deviant);
}
#endif
//-------------------------------[end]-----------------------------------------

//----------------------[set sync mode no sync=0;]-----------------------------
#if MINIMAL_MODE == 0
void cc1101_set_sync_mode(uint8_t cfg)
{
    uint8_t data;
    data = cc1101_spi_read_register(MDMCFG2);
    data = (data & 0xF8) | (cfg & 0x07);
    cc1101_spi_write_register(MDMCFG2, data);
    //uart_println("MDMCFG2: 0x%02X\n", data);
}
#endif
//-------------------------------[end]-----------------------------------------

//---------------[set FEC ON=TRUE; OFF=FALSE]----------------------------------
#if MINIMAL_MODE == 0
void cc1101_set_fec(uint8_t cfg)
{
    uint8_t data;
    data = cc1101_spi_read_register(MDMCFG1);
    data = (data & 0x7F) | (((cfg) << 7) & 0x80);
    cc1101_spi_write_register(MDMCFG1, data);
    uart_print("MDMCFG1: 0x");
    cc1101_uart_puti(data);
}
#endif
//-------------------------------[end]------------------------------------------

//---------------[set data_whitening ON=TRUE; OFF=FALSE]------------------------
#if MINIMAL_MODE == 0
void cc1101_set_data_whitening(uint8_t cfg)
{
    uint8_t data;
    data = cc1101_spi_read_register(PKTCTRL0);
    data = (data & 0xBF) | (((cfg) << 6) & 0x40);
    cc1101_spi_write_register(PKTCTRL0, data);
    //uart_print("PKTCTRL0: 0x");
    //uart_println(data);
}
#endif
//-------------------------------[end]-----------------------------------------

//------------[set manchester encoding ON=TRUE; OFF=FALSE]---------------------
#if MINIMAL_MODE == 0
void cc1101_set_manchester_encoding(uint8_t cfg)
{
    uint8_t data;
    data = cc1101_spi_read_register(MDMCFG2);
    data = (data & 0xF7) | (((cfg) << 3) & 0x08);
    cc1101_spi_write_register(MDMCFG2, data);
    //uart_print("MDMCFG2: 0x");
    //uart_println(data);
}
#endif
//-------------------------------[end]------------------------------------------

//--------------------------[cc1101_rssi_convert]--------------------------------------
int8_t cc1101_rssi_convert(uint8_t Rssi_hex)
{
    int8_t rssi_dbm = 0;
    int16_t Rssi_dec = 0;

    Rssi_dec = Rssi_hex;        //convert unsigned to signed

    if (Rssi_dec >= 128)
    {
        rssi_dbm = ((Rssi_dec - 256) / 2) - RSSI_OFFSET_868MHZ;
    }
    else
    {
        if (Rssi_dec < 128)
        {
            rssi_dbm = ((Rssi_dec) / 2) - RSSI_OFFSET_868MHZ;
        }
    }
    return rssi_dbm;
}
//-------------------------------[end]------------------------------------------

//----------------------------[lqi convert]-------------------------------------
uint8_t cc1101_lqi_convert(uint8_t lqi)
{
    return (lqi & 0x7F);
}
//-------------------------------[end]------------------------------------------

//----------------------------[check crc]---------------------------------------
uint8_t cc1101_check_crc(uint8_t lqi)
{
    return (lqi & 0x80);
}
//-------------------------------[end]------------------------------------------


//----------------------------[get temp]----------------------------------------
#if MINIMAL_MODE == 0
uint8_t cc1101_get_temp(uint8_t *ptemp_Arr)
{
    // const uint8_t num_samples = 8;
    // uint16_t adc_result = 0;
    // uint32_t temperature = 0;

    // cc1101_sidle();                              //sets CC1100 into IDLE
    // cc1101_spi_write_register(PTEST, 0xBF);       //enable temp sensor
    // delay_ms(50);                            //wait a bit

    // for(uint8_t i = 0; i < num_samples; i++)    //sampling analog temperature value
    // {
    //     adc_result += analogRead(GDO0);
    //     delay_ms(1);
    // }
    // adc_result = adc_result / num_samples;
    // //uart_println(adc_result);

    // temperature = (adc_result * CC1101_TEMP_ADC_MV) / CC1101_TEMP_CELS_CO;

    // ptemp_Arr[0] = temperature / 10;      //cut last digit
    // ptemp_Arr[1] = temperature % 10;      //isolate last digit

    // if(cc1101_debug_level > 0){
    //     uart_print("Temp:");uart_print(ptemp_Arr[0]);uart_print(".");uart_println(ptemp_Arr[1]);
    // }

    // cc1101_spi_write_register(PTEST,0x7F);       //writes 0x7F back to PTest (app. note)

    // cc1101_receive();
    // return (*ptemp_Arr);
    ptemp_Arr[0] = 0;
    ptemp_Arr[1] = 0;
    return 0;
}
#endif
//-------------------------------[end]------------------------------------------


//|==================== SPI Initialisation for CC1101 =========================|
void cc1101_spi_begin(void)
{
   /* configure SPI */
    spi_config();
    spi_cs_idle();
}
//-------------------------------[end]------------------------------------------

void cc1101_GDO_set()
{
    CONFIG_GDO0_PIN_AS_INPUT();
	CONFIG_GDO2_PIN_AS_INPUT();
}


//|==================== SPI Initialisation for CC1101 =========================|

void cc1101_spi_end(void)
{
    /// Not needed, so unimplemented
    /// Kept here for compatibility purpose
	return;
}
//-------------------------------[end]------------------------------------------



//|============================= SPI Transmission =============================|
uint8_t cc1101_spi_putc(uint8_t data)
{

    SPI_WRITE_BYTE(data);
    spi_busy_wait();
    uint8_t statusByte = SPI_READ_BYTE();

    return statusByte;
}
//|==================== schreibe strobe command  ==============================|
void cc1101_spi_write_strobe(uint8_t spi_instr)
{
    SPI_CS_ACTIVE();          // CS low
    while (SPI_SO_PIN_IS_HIGH());  // Wait until SO becomes LOW
    cc1101_spi_putc(spi_instr);
    SPI_CS_IDLE();         // CS high
}
//|======================= Read status byte ==================================|
/*
uint8_t cc1101_spi_read_status(uint8_t spi_instr)
{
    SPI_CS_ACTIVE();          // CS low
    while (SPI_SO_PIN_IS_HIGH());  // Wait until SO becomes LOW
    cc1101_spi_putc(spi_instr | Read_burst);
    spi_instr = cc1101_spi_putc(0xFF);
    SPI_CS_IDLE();         // CS high

    return spi_instr;
}
*/
//|========================== Read registers ==============================|
uint8_t cc1101_spi_read_register(uint8_t spi_instr)
{
    SPI_CS_ACTIVE();          // CS low
    while (SPI_SO_PIN_IS_HIGH());  // Wait until SO becomes LOW
    cc1101_spi_putc(spi_instr | READ_SINGLE_BYTE);
    spi_instr = cc1101_spi_putc(0xFF);
    SPI_CS_IDLE();         // CS high

    return spi_instr;
}
//|========== Read multiple registers in burst mode ========|
void cc1101_spi_read_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t length)
{
    SPI_CS_ACTIVE();          // CS low
    while (SPI_SO_PIN_IS_HIGH());  //Wait until SO becomes LOW
    cc1101_spi_putc(spi_instr | READ_BURST);

    for(uint8_t i=0; i<length; i++)
        {
            pArr[i] = cc1101_spi_putc(0xFF);
        }

    SPI_CS_IDLE();
}
//|======================= Write registers =============================|
void cc1101_spi_write_register(uint8_t spi_instr, uint8_t value)
{
    SPI_CS_ACTIVE();          // CS low
    while (SPI_SO_PIN_IS_HIGH());  //Wait until SO becomes LOW
    cc1101_spi_putc(spi_instr | WRITE_SINGLE_BYTE);
    cc1101_spi_putc(value);
    SPI_CS_IDLE();
}
//|======= Write multiple registers in burst mode =======|
void cc1101_spi_write_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t length)
{
    SPI_CS_ACTIVE();          // CS low
    while (SPI_SO_PIN_IS_HIGH());  //Wait until SO becomes LOW
    cc1101_spi_putc(spi_instr | WRITE_BURST);

    for(uint8_t i=0; i<length ;i++)
    {
        cc1101_spi_putc(pArr[i]);
    }

    SPI_CS_IDLE();
}
//|=================================== END ====================================|


//|=========================== UART helper ====================================|

/*******************************************************************************
Function: cc1101_uart_puthex_nibble()
Purpose:  cc1101_transmit lower nibble as ASCII-hex to UART
Input:    byte value
Returns:  none
This functions has been added by Martin Thomas <eversmith@heizung-thomas.de>
Don't blame P. Fleury if it doesn't work ;-)
*******************************************************************************/
void cc1101_uart_puthex_nibble(const unsigned char b)
{
    unsigned char  c = b & 0x0f;
    if (c>9) c += 'A'-10;
    else c += '0';
    uart_write_byte(c);                  //uart_putc replaced to Serial.write
} /* cc1101_uart_puthex_nibble */

/*******************************************************************************
Function: cc1101_uart_puthex_byte()
Purpose:  cc1101_transmit upper and lower nibble as ASCII-hex to UART
Input:    byte value
Returns:  none
This functions has been added by Martin Thomas <eversmith@heizung-thomas.de>
Don't blame P. Fleury if it doesn't work ;-)
*******************************************************************************/
void cc1101_uart_puthex_byte(const unsigned char b)
{
    cc1101_uart_puthex_nibble(b>>4);
    cc1101_uart_puthex_nibble(b);
} /* cc1101_uart_puthex_byte */

/*******************************************************************************
Function: cc1101_uart_puti()
Purpose:  cc1101_transmit integer as ASCII to UART
Input:    integer value
Returns:  none
This functions has been added by Martin Thomas <eversmith@heizung-thomas.de>
Don't blame P. Fleury if it doesn't work ;-)
*******************************************************************************/
void cc1101_uart_puti(const int val)
{
    uart_printnum(val, 10);

}/* cc1101_uart_puti */
//|================================= END ======================================|
