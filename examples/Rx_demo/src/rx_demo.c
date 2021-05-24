#include "stm8_cc1101.h"
#include "timer.h"

//---------------------------=[Global variables]=----------------------------
uint32_t rf_timecode = 0;
//--------------------------[Global CC1100 variables]------------------------
uint8_t Tx_fifo[FIFOBUFFER], Rx_fifo[FIFOBUFFER];
uint8_t My_addr, pktlen;
uint8_t rx_addr,sender,lqi;
int8_t rssi_dbm;
volatile uint8_t is_packet_available;

//---------------------------------[SETUP]-----------------------------------
void main(void) 
{
	//----------------[STM8S peripheral initialization]--------------------
	CLK_CKDIVR = 0;

	DEBUG_PIN_CONFIG();

	uart_config();
	timer_config();
	enableInterrupts();
	//-------------------------------[end]---------------------------------

	// init CC1101 RF-module
	My_addr = 0x10;
	cc1101_begin(&My_addr);                   //inits RF module with main default settings

	cc1101_sidle();                          //set to ILDE first
    cc1101_set_mode(0x01);                   //set modulation mode 1 = GFSK_1_2_kb; 2 = GFSK_38_4_kb; 3 = GFSK_100_kb; 4 = MSK_250_kb; 5 = MSK_500_kb; 6 = OOK_4_8_kb
    cc1101_set_ISM(0x02);                    //set frequency 1=315MHz; 2=433MHz; 3=868MHz; 4=915MHz
    cc1101_set_channel(0x01);                //set channel
    cc1101_set_output_power_level(0);        //set PA level in dbm
    cc1101_set_myaddr(My_addr);                 //set my own address

	/* See Page 62 of 98 in cc1101 datasheet */
	/* Asserts when sync word has been sent / received, 
	 * and de-asserts at the end of the packet. 
	 */
	cc1101_spi_write_register(IOCFG2,0x06);

	cc1101_show_register_settings();         //shows current CC1101 register values
	cc1101_receive();                        //set to RECEIVE mode

	uart_println("CC1101 RX Demo for STM8S");   //welcome message

	while (1)
	{
		/*
		 * TODO: Implement interrupt so mcu can wake from sleep when GDO2 changes state on receiving packet
		 */
		
		/*----------------------------------------------
		- Here we are expecting a 7-byte message.
		- Byte[6:3] contain a 32-bit timestamp.
		------------------------------------------------*/
		
		//if valid package is received
		if(is_packet_available == TRUE)
		{
			rf_timecode = ((uint32_t)Rx_fifo[3] << 24) + 
						((uint32_t)Rx_fifo[4] << 16) + 
						((uint16_t)Rx_fifo[5] <<  8) + 
									Rx_fifo[6];
			uart_print("TX_Timecode: "); uart_printnum(rf_timecode, 10); uart_println("ms\n");
			is_packet_available = FALSE;
		}

		if(cc1101_packet_available() == TRUE)
		{
			// in cc1101_get_payload function, we need to pass address of those variables
			if(cc1101_get_payload(Rx_fifo, &pktlen, &rx_addr, &sender, &rssi_dbm, &lqi) == TRUE) //stores the payload data to Rx_fifo
			{
				is_packet_available = TRUE;                                //set flag that a package is in RX buffer
				uart_print("Received packet!");
			}
			else
			{
				is_packet_available = FALSE;                               //set flag that an package is corrupted
			}
		} 
	}
}
