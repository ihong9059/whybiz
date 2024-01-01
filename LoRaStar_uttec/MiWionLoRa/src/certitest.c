/*
 * certitest.c
 *
 * Created: 2020-04-29 오후 12:06:36
 *  Author: A14633
 */ 

#include "sio2host.h"
#include "MiWi.h"
#include "radio_driver_SX1276.h"
#include "radio_driver_hal.h"
#include "radio_registers_SX1276.h"

/************************** Extern variables ***********************************/
extern volatile uint8_t	MACTxBuffer[];
extern int16_t LBT_TR;
extern int32_t LBT_LIMIT;


void PrintMenuC(void)
{
	printf("\r\nSAMR34 Certification Firmware\r\n");
	printf("**************************************\r\n");
	printf("1: Send Message\r\n");
	printf("2: CW\r\n");
	printf("3: Send LBT Message\r\n");
	printf("4: RX Mode\r\n");
	printf("c: Channel\r\n");
	printf("b: Bandwidth\r\n");
	printf("s: Spreading Factor\r\n");
	printf("p: Output Power\r\n");
	printf("t: LBT Threshold\r\n");
	printf("x: SLEEP\r\n");
	
	printf("\n\rH: Print Menu\r\n");
	printf("**************************************\r\n");
}
void ConsoleProcC(void)
{
	char rxChar;
	uint8_t i,rlt,tmp;
	uint32_t OperatingChannel = 0x00300000;//0xFFFFFFFF;
	uint64_t timeVal;
	
	int16_t LBT_tmp;
	
	uint32_t fdev = 0;
	uint32_t datarate = 50e3;
	uint16_t preambleLen = 5;
	bool fixLen = 1;
	bool crcOn = 1;
	uint8_t tmp2=0;
	
	
	if((-1) != (rxChar = sio2host_getchar_nowait()))
	{
		switch(rxChar){
			case '1':
			printf("\r\nStart TX...\r\n");
			MiApp_FlushTx();
			for(i=0;i<16;i++){
				MiApp_WriteData(i);
			}
			printf("Press 'q' to stop TX....\r\n");
			while(1){
				
				if((-1) != (rxChar = sio2host_getchar_nowait())){
					if(rxChar == 'q') {
						printf("Stop TX....\r\n");
						
						SX1276_RX_INIT();
						break;
					}
				}
				
				SX1276_TX((uint8_t *)MACTxBuffer, 16);
			}
			
			break;
			case '2':
			printf("\r\nStart CW...\r\n");
			
			tmp2 = RADIO_RegisterRead( REG_OPMODE );
			RADIO_RegisterWrite( REG_OPMODE, ( tmp2 & RF_OPMODE_MASK ) | 0 ); //MODEM_FSK = 0;
			tmp2 = RADIO_RegisterRead( REG_OPMODE );
			RADIO_RegisterWrite( REG_OPMODE, ( tmp2 & RFLR_OPMODE_LONGRANGEMODE_MASK ) | RFLR_OPMODE_LONGRANGEMODE_OFF );
			RADIO_RegisterWrite( REG_DIOMAPPING1, 0x00 );
			RADIO_RegisterWrite( REG_DIOMAPPING2, 0x30 ); // DIO5=ModeReady
			fdev = ( uint16_t )( ( double )fdev / ( double )FREQ_STEP_CW );
			RADIO_RegisterWrite( REG_FDEVMSB, ( uint8_t )( fdev >> 8 ) );
			RADIO_RegisterWrite( REG_FDEVLSB, ( uint8_t )( fdev & 0xFF ) );

			datarate = ( uint16_t )( ( double )XTAL_FREQ_CW / ( double )datarate );
			RADIO_RegisterWrite( REG_BITRATEMSB, ( uint8_t )( datarate >> 8 ) );
			RADIO_RegisterWrite( REG_BITRATELSB, ( uint8_t )( datarate & 0xFF ) );

			RADIO_RegisterWrite( REG_PREAMBLEMSB, ( preambleLen >> 8 ) & 0x00FF );
			RADIO_RegisterWrite( REG_PREAMBLELSB, preambleLen & 0xFF );

			tmp2 = RADIO_RegisterRead( REG_PACKETCONFIG1 );
			
			RADIO_RegisterWrite( REG_PACKETCONFIG1, ( tmp2 & RF_PACKETCONFIG1_CRC_MASK & RF_PACKETCONFIG1_PACKETFORMAT_MASK ) | ( ( fixLen == 1 ) ? RF_PACKETCONFIG1_PACKETFORMAT_FIXED : RF_PACKETCONFIG1_PACKETFORMAT_VARIABLE ) | ( crcOn << 4 ) );
			
			RADIO_RegisterWrite( REG_DIOMAPPING1, ( RADIO_RegisterRead( REG_DIOMAPPING1 ) & RF_DIOMAPPING1_DIO0_MASK & RF_DIOMAPPING1_DIO1_MASK & RF_DIOMAPPING1_DIO2_MASK ) |RF_DIOMAPPING1_DIO1_01 );

			RADIO_RegisterWrite( REG_DIOMAPPING2, ( RADIO_RegisterRead( REG_DIOMAPPING2 ) & RF_DIOMAPPING2_DIO4_MASK & RF_DIOMAPPING2_MAP_MASK ) );
			
			RADIO_RegisterWrite( REG_OPMODE, ( RADIO_RegisterRead( REG_OPMODE ) & RF_OPMODE_MASK ) | RF_OPMODE_TRANSMITTER );
			
			printf("Press 'q' to stop TX....\r\n");
			while(1){
				rxChar = sio2host_getchar();
				if(rxChar == 'q') {
					printf("Stop TX....\r\n");
					SX1276_Reset();
					SX1276_Config();
					SX1276_RX_INIT();
					break;
				}
				
			}
			break;
			case '3':
			printf("\r\nStart TX with LBT...\r\n");
			MiApp_FlushTx();
			for(i=0;i<16;i++){
				MiApp_WriteData(i);
			}
			printf("Press 'q' to stop TX....\r\n");
			while(1){
				
				if((-1) != (rxChar = sio2host_getchar_nowait())){
					if(rxChar == 'q') {
						printf("Stop TX....\r\n");
						
						SX1276_RX_INIT();
						break;
					}
				}
				if(MiApp_BroadcastPacket(false)){
					printf("*");
					} else {
					printf("LBT TX Fail!!\r\n");
				}
			}
			break;
			case '4':
			printf("\r\nStart RX...\r\n");
			SX1276_RX_INIT();
			printf("RX MODE...\r\n\r\n");

			break;
			case 'c':
			printf("\r\nChannel setting...");
			printf("\r\nChannel: 20 ~ 32 ( 920.9MHz ~ 923.3MHz )");
			
			rxChar = sio2host_getchar();
			tmp = (rxChar-'0')*10;
			rxChar = sio2host_getchar();
			tmp += (rxChar-'0');
			
			if(tmp>19 && tmp<33)
			{
				//20, 26, 32
				MiMAC_SetChannel(tmp);
				
				printf("\r\nSet Channel - %d - Done\r\n\r\n",tmp);
			}
			else
			{
				printf("\r\nInvalid value - %d \r\n\r\n",tmp);
			}
			
			break;
			case 'b':
			// SignalBw [0:  7.8kHz, 1: 10.4kHz, 2: 15.6kHz, 3: 20.8kHz, 4: 31.25kHz,
			//			 5: 41.7kHz, 6: 62.5kHz, 7: 125kHz,  8: 250kHz,  9: 500kHz, other: Reserved]
			printf("\r\nBandwidth setting...\r\n");
			printf("0:  7.8Khz,1: 10.4Khz ,2: 15.6Khz ,3: 20.8Khz ,4: 31.25Khz \r\n");
			printf("5: 41.7Khz,6: 62.5Khz ,7: 125Khz , 8: 250Khz , 9: 500Khz");
			rxChar = sio2host_getchar();
			tmp = (rxChar-'0');
			
			if(tmp>=0 && tmp<=9)
			{
				
				SX1276LoRaSetSignalBandwidth(tmp);
				
				printf("\r\nSet SignalBw - %d - Done\r\n\r\n",tmp);
			}
			else
			{
				printf("\r\nInvalid value - %d \r\n\r\n",tmp);
			}
			
			break;
			case 's':
			
			// SpreadingFactor [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
			printf("\r\nSpreading Factor setting...\r\n");
			printf("06: SF6, 07: SF7, 08: SF8, 09: SF9, 10: SF10, 11: SF11, 12: SF12 \r\n");
			rxChar = sio2host_getchar();
			tmp = (rxChar-'0')*10;
			rxChar = sio2host_getchar();
			tmp += (rxChar-'0');
			
			if(tmp>=6 && tmp<=12)
			{
				
				SX1276LoRaSetSpreadingFactor(tmp);
				
				printf("\r\nSet SpreadingFactor - %d - Done\r\n\r\n",tmp);
			}
			else
			{
				printf("\r\nInvalid value - %d \r\n\r\n",tmp);
			}
			
			
			break;
			case 'p':
			printf("\r\nOutput Power setting...\r\n");
			printf("20, 17 ~ 02 dBm\n\r");
			rxChar = sio2host_getchar();
			tmp = (rxChar-'0')*10;
			rxChar = sio2host_getchar();
			tmp += (rxChar-'0');
			
			if((tmp>=2 && tmp<=17) || tmp == 20)
			{
				
				SX1276LoRaSetRFPower(tmp);
				
				printf("\r\nSet Output Power - %d - Done\r\n\r\n",tmp);
			}
			else
			{
				printf("\r\nInvalid value - %d \r\n\r\n",tmp);
			}
			break;
			case 't':
			printf("\n\rLBT Threshold setting...\r\n");
			
			printf("Current Threshold Value: %d dBm\r\n",LBT_TR);
			printf("Input threshold Value: (-)10 ~ 99 \r\n");
			LBT_tmp = 0;
			rxChar = sio2host_getchar();
			tmp = (rxChar-'0')*10;
			rxChar = sio2host_getchar();
			tmp += (rxChar-'0');
			
			LBT_tmp = tmp;
			
			if(LBT_tmp < 10){
				LBT_tmp = 0;
				} else if(LBT_tmp >= 99){
				LBT_tmp = 99;
			}
			LBT_TR = LBT_tmp * -1;
			LBT_LIMIT = LBT_tmp * -1;
			
			printf("Changed Threshold Value: %d dBm\r\n",LBT_TR);
			
			break;
			case 'h':
			PrintMenuC();
			break;
			case 'x':
			
			
			//Sleep();
			
			break;
		}
	}
}