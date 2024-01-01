/**
* \file  edbg_eui.c
*
* \brief This is the implementation of to read device MAC EUI value from EDBG
*		
*
* Copyright (c) 2018 Microchip Technology Inc. and its subsidiaries. 
*
* \asf_license_start
*
* \page License
*
* Subject to your compliance with these terms, you may use Microchip
* software and any derivatives exclusively with Microchip products. 
* It is your responsibility to comply with third party license terms applicable 
* to your use of third party software (including open source software) that 
* may accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, 
* WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, 
* INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, 
* AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE 
* LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL 
* LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE 
* SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE 
* POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY 
* RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, 
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*
* \asf_license_stop
*
*/
/*
* Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
*/

#include <stdint.h>
#include "string.h"
#include "edbg_eui.h"
#include "asf.h"

extern uint8_t myLongAddress[8];

#if (EDBG_EUI_READ == 1)

/* 128 bit Serial number stored address. It is used for seeding the pseudo random number generator - rand() */
#define S_NO_WORD0 ((uint32_t*)0x0080A00C)
#define S_NO_WORD1 ((uint32_t*)0x0080A040)
#define S_NO_WORD2 ((uint32_t*)0x0080A044)
#define S_NO_WORD3 ((uint32_t*)0x0080A048)

  /*
    4.3.1 Information Interface

    The TWI-bus can be used to request information from the EDBG. This
    is done by sending a special sequence as described below.

    ? START Condition
    ? Address + W
    ? Request token
    ? START Condition (repeated start)
    ? Address + R
    ? Response
    ? ...
    ? STOP Condition

    The sequence starts by sending a normal start condition followed
    by an address byte (default address is 0x28) with the RW bit
    cleared. Then a request token identifying the requested
    information is sent. Normally, a received byte would be put into
    the DGI buffer, but a repeated start will trigger the EDBG to
    parse the received token. Then an address with the RW bit set. The
    EDBG will then start to push the requested data onto the TWIbus.
    All response bytes must be ACKed by the master, and the final byte
    must be NAKed.


    4.3.1.2 Kit Data

    Token: 0xD2

    The EDBG has 256 bytes for storing kit-specific data such as MAC
    address, calibration values etc. See the kit documentation for
    details on how the data is organized. The information can be
    retrieved by using the Kit Data token. Note that not all kits have
    kit-specific data, and the entire section will read as 0.  After
    sending the token, the stored kit-specific data will be sent byte
    by byte starting with location 0, until a stop condition is
    detected.

   */

/*---------------------------------------------------------------------------*/

/******************************** MACROS ***************************************/

#define SLAVE_ADDRESS				0x60//0x28 //kwlee
#define TIMEOUT						1//1000
#define SLAVE_WAIT_TIMEOUT			10
#define LEN_EUI						256
#define MAC_ADDR_LEN				8

#ifndef EDBG_I2C_MODULE
#define EDBG_I2C_MODULE   					SERCOM1
#endif

#ifndef EDBG_I2C_SERCOM_PINMUX_PAD0
#define EDBG_I2C_SERCOM_PINMUX_PAD0			PINMUX_PA16C_SERCOM1_PAD0
#endif

#ifndef EDBG_I2C_SERCOM_PINMUX_PAD1
#define EDBG_I2C_SERCOM_PINMUX_PAD1			PINMUX_PA17C_SERCOM1_PAD1
#endif

/************************** GLOBAL VARIABLES ***********************************/
struct i2c_master_module i2c_master_instance;
struct i2c_master_packet master_packet;

/* Must read out the full 256 bytes of memory from the EDBG, otherwise
   its internal state will be out of synch and the chip won't return
   the first bytes the next time the chip is queried. */
uint8_t readbuf[LEN_EUI];

uint8_t kit_data[256];

static uint8_t write_buffer[2] = {
	0x51, 0xd2,
};

/***************************** FUNCTIONS ***************************************/

/*********************************************************************//**
\brief    Configures the I2C module as Master
*************************************************************************/
static void configure_i2c_master(void)
{
	/* Create and initialize config structure */
	struct i2c_master_config config_i2c;
	i2c_master_get_config_defaults(&config_i2c);

	/* Change pins */
	config_i2c.pinmux_pad0  = EDBG_I2C_SERCOM_PINMUX_PAD0;
	config_i2c.pinmux_pad1  = EDBG_I2C_SERCOM_PINMUX_PAD1;

	/* Initialize and enable device with config */
	i2c_master_init(&i2c_master_instance, EDBG_I2C_MODULE, &config_i2c);

	i2c_master_enable(&i2c_master_instance);
}

/*********************************************************************//**
\brief		Reads back the device MAC address stored in User page of EDBG
\param[in]  eui - Device EUI read back from EDBG(8 bytes)
*************************************************************************/
 void edbg_eui_read_eui64(uint8_t *eui)
{
	//configure_i2c_master();
	//uint32_t timeout = 0;
	//
	///** Send the request token */
	//master_packet.address         = SLAVE_ADDRESS;
	//master_packet.data_length     = sizeof(write_buffer);
	//master_packet.data            = write_buffer;
	//master_packet.ten_bit_address = false;
	//master_packet.high_speed      = false;
	//master_packet.hs_master_code  = 0x0;
	//while (i2c_master_write_packet_wait_no_stop(&i2c_master_instance, &master_packet) !=
			//STATUS_OK) {
		///* Increment timeout counter and check if timed out. */
		//if (timeout++ == TIMEOUT) {
			//return;
		//}
	//}
//
	///** Get the extension boards info */
	//master_packet.data_length     = 256;
	//master_packet.data            = kit_data;
	//while (i2c_master_read_packet_wait(&i2c_master_instance, &master_packet) !=
			//STATUS_OK) {
		///* Increment timeout counter and check if timed out. */
		//if (timeout++ == TIMEOUT) {
			//return;
		//}
	//}
   //
    //// First 8bytes value is MAC Address
	//memcpy(eui, kit_data, MAC_ADDR_LEN);
	configure_i2c_master();
	uint32_t timeout = 0;
	uint8_t i;
	/** Send the request token */
	master_packet.address         = 0x00;
	master_packet.data_length     = 0;//sizeof(write_buffer);
	master_packet.data            = 0x00;
	master_packet.ten_bit_address = false;
	master_packet.high_speed      = false;
	master_packet.hs_master_code  = 0x0;
	//i2c_master_write_packet_wait_no_stop(&i2c_master_instance, 0x00);
	//i2c_master_write_byte(&i2c_master_instance, 0x00);
	//port_pin_set_output_level(PIN_PA16, false);
	//delay_ms(5);
	//port_pin_set_output_level(PIN_PA16, true);
	//timeout = 0;
	//delay_ms(5);
	while (i2c_master_write_packet_wait_no_stop(&i2c_master_instance, &master_packet) !=
	STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == TIMEOUT) {
			break;
			//return;
		}
	}
	delay_ms(5);
	
	/** Send the request token */
	master_packet.address         = SLAVE_ADDRESS;
	master_packet.data_length     = sizeof(write_buffer);
	master_packet.data            = write_buffer;
	master_packet.ten_bit_address = false;
	master_packet.high_speed      = false;
	master_packet.hs_master_code  = 0x0;
	
	configure_i2c_master();
	for(i=0;i<32;i++){
		kit_data[i]=0;
	}
	/** Get the extension boards info */
	master_packet.data_length     = 4;//256;
	master_packet.data            = kit_data;
	//read 04 11 33 43
	//timeout = 0;
	while (i2c_master_read_packet_wait(&i2c_master_instance, &master_packet) !=
	STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == TIMEOUT) {
			break;
			//return;
		}
	}
	for(i=0;i<32;i++){
		kit_data[i]=0;
	}
	//write 03 07 02 80 00 00 09 ad
	master_packet.data_length     = 8;//256;
	//for(i=0;i<8;i++){
	kit_data[0]=0x03;
	kit_data[1]=0x07;
	kit_data[2]=0x02;
	kit_data[3]=0x80;
	kit_data[4]=0x00;
	kit_data[5]=0x00;
	kit_data[6]=0x09;
	kit_data[7]=0xAD;
	//}
	master_packet.data            = kit_data;
	//timeout = 0;
	while (i2c_master_write_packet_wait_no_stop(&i2c_master_instance, &master_packet) !=
	STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == TIMEOUT) {
			break;
			//return;
		}
	}
	
	//read 23
	for(i=0;i<32;i++){
		kit_data[i]=0;
	}
	master_packet.data_length     = 1;//256;
	master_packet.data            = kit_data;
	timeout = 0;
	while (i2c_master_read_packet_wait(&i2c_master_instance, &master_packet) !=
	STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == TIMEOUT) {
			break;
			//return;
		}
	}
	//read 01 23 A4 ce 00 00 60 02 6a 2
	for(i=0;i<32;i++){
		kit_data[i]=0;
	}
	master_packet.data_length     = 16;//256;//9
	master_packet.data            = kit_data;
	//timeout = 0;
	while (i2c_master_read_packet_wait(&i2c_master_instance, &master_packet) !=
	STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == TIMEOUT) {
			break;
			//return;
		}
	}
	//for(i=0;i<8;i++){
	//myLongAddress[i]=kit_data[i];
	//}
	myLongAddress[7]=kit_data[2];
	myLongAddress[6]=kit_data[3];
	myLongAddress[5]=kit_data[4];
	myLongAddress[4]=kit_data[9];
	myLongAddress[3]=kit_data[10];
	myLongAddress[2]=kit_data[11];
	myLongAddress[1]=kit_data[12];
	myLongAddress[0]=kit_data[13];
	
	//myLongAddress[7]=0x01;
	//myLongAddress[6]=0x01;
	//myLongAddress[5]=0x01;
	//myLongAddress[4]=0x01;
	//myLongAddress[3]=0x01;
	//myLongAddress[2]=0x01;
	//myLongAddress[1]=0x01;
	//myLongAddress[0]=0x01;
	
	master_packet.data_length     = 1;//256;
	//for(i=0;i<8;i++){
	//kit_data[0]=0x02;
	
	//}
	master_packet.data            = 0x02;
	//write 02
	//timeout = 0;
	while (i2c_master_write_packet_wait_no_stop(&i2c_master_instance, &master_packet) !=
	STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == TIMEOUT) {
			break;
			//return;
		}
	}
	// First 8bytes value is MAC Address
	memcpy(eui, myLongAddress, MAC_ADDR_LEN);
}

#endif /* EDBG_EUI_READ */

/* eof edbg_eui.c */
