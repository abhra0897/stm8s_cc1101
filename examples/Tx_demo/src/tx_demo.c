#include "stm8_cc1101.h"
#include "timer.h"

uint8_t Tx_fifo[FIFOBUFFER], Rx_fifo[FIFOBUFFER];
uint8_t My_addr, Rx_addr, Pktlen;


void main(void)
{
    //----------------[STM8S peripheral initialization]--------------------
    CLK_CKDIVR = 0;
    
    DEBUG_PIN_CONFIG();

    uart_config();
    timer_config();
    enableInterrupts();
    //-------------------------------[end]---------------------------------

    //-------------------[CC1101 RELATED OPERATIONS]-----------------------

    // init CC1101 RF-module
    My_addr = 0x00;                                              // address of this device (tx)
    Rx_addr = 0x10;                                              //receiver address
    cc1101_begin(&My_addr);                   //inits RF module with main default settings

    cc1101_sidle();                          //set to ILDE first
    cc1101_set_mode(0x01);                   //set modulation mode 1 = GFSK_1_2_kb; 2 = GFSK_38_4_kb; 3 = GFSK_100_kb; 4 = MSK_250_kb; 5 = MSK_500_kb; 6 = OOK_4_8_kb
    cc1101_set_ISM(0x02);                    //set frequency 1=315MHz; 2=433MHz; 3=868MHz; 4=915MHz
    cc1101_set_channel(0x01);                //set channel
    cc1101_set_output_power_level(0);        //set PA level in dbm
    cc1101_set_myaddr(0x00);                 //set my own address

    cc1101_show_register_settings();         //shows current CC1101 register values
    cc1101_receive();                        //set to RECEIVE mode

    uart_println("CC1101 TX Demo for STM8S");   //welcome message

    while (1)
    {

        uint32_t time_stamp = timer_get_ms_tick();                       //generate time stamp

        Tx_fifo[3] = (uint8_t)(time_stamp >> 24);                    //split 32-Bit timestamp to 4 byte array
        Tx_fifo[4] = (uint8_t)(time_stamp >> 16);
        Tx_fifo[5] = (uint8_t)(time_stamp >> 8);
        Tx_fifo[6] = (uint8_t)(time_stamp);

        Pktlen = 0x07;                                               //set packet len to 0x13
        cc1101_send_packet(My_addr, Rx_addr, Tx_fifo, Pktlen, 40);   //sents package over air. ACK is received via GPIO polling    

        timer_delay_ms(1000);
    }
    //-------------------------------[end]---------------------------------

}