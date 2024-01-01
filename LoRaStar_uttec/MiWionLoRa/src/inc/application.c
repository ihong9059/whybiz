/*
 * CFile1.c
 *
 * Created: 2020-04-29 오후 12:57:09
 *  Author: A14633
 */ 

#include "sio2host.h"
#include "radio_driver_SX1276.h"
#include "radio_driver_hal.h"
#include "radio_registers_SX1276.h"
#include "MiWi.h"
#include "nvm.h"
#include "common_hw_timer.h"
#include "conf_app.h"

#define SERIAL_RX_BUF_SIZE_HOST    220

/************************** Extern variables ***********************************/
extern uint64_t SwTimerGetTime(void);
extern uint8_t rxData[];
extern volatile uint8_t	MACTxBuffer[];
extern uint8_t serial_rx_buf[SERIAL_RX_BUF_SIZE_HOST];
extern uint8_t serial_rx_buf_tail;
extern uint8_t RF_OUTPUT,RF_CHANNEL,SF_DR;
extern bool repeat_role_f;
/************************** Global variables ***********************************/
uint8_t uartRxData[SERIAL_RX_BUF_SIZE_HOST], uartRxDataIndex;

uint8_t uartAlive=0,uartActive=0,uartCompleted=0;
uint8_t uartAlive1=0,uartActive1=0,uartCompleted1=0;
uint8_t set_ScanDuration,set_SearchDuration;
uint8_t rxLenth, rxLenCnt;
uint8_t error_buff_new[8],error_buff_old[8];
uint64_t uartRecevingGetTime=0,RefT0GetTime;
uint64_t Time_Buffer, Time_Buffer2;
uint32_t error_count, data_count;
unsigned long Time_Counter;
long error_counter = -1;

bool chkecho = true;
static bool unicast_test_f, broadcast_test_f, broadcast_continue_test_f, broadcast_timeout_f;
bool chkprotocol = true;

MIWI_TICK t1, t2;
uint32_t t3;

extern void AESInit(void);
extern void Sleep(void);
void ConsoleProc2(void);

extern unsigned int Toggle_Count, Toggle_Continue, Toggle_End, Toggle_Old;

void HandleRxData(void)
{
	uint8_t i=0;
	
	if( MiApp_MessageAvailable() )
	{		
		if(chkecho)printf("\n\r[Data Received]\n\r");
		if(chkecho)printf("SRC PID: %x\r\n",rxMessage.SourcePANID);
		if(chkecho)printf("SRC ADR: %.2x%.2x\r\n",rxMessage.SourceAddress[1],rxMessage.SourceAddress[0]);
		if(chkecho)printf("Length: %d\r\n",rxMessage.PayloadSize);
		if(chkecho)printf("RSSI: %ddBm\r\n",rxMessage.PacketRSSI);
		if(chkecho)printf("Payload: ");
		for(i=0;i<rxMessage.PayloadSize-2;i++){
			if(chkecho)printf("%.2x ",*(uint8_t*)(rxMessage.Payload+i));
		}
		if(chkecho)printf("\n\r");
		if(chkecho)printf("Character: \r\n");		
		for(i=0;i<rxMessage.PayloadSize-2;i++){
			printf("%c",*(uint8_t*)(rxMessage.Payload+i));			
			error_buff_new[i] = *(uint8_t*)(rxMessage.Payload+i);
		}
		
		
		//if(error_buff_new[0] == 'S'){									//ihkim - 담인 test
			//if(error_buff_new[1] == 't'){
				//if(error_buff_new[2] == 'a'){
					//if(error_buff_new[3] == 'r'){	
						//if(error_buff_new[4] == 't'){	
							//LED_Off(PIN_PA14);
						//}
					//}
				//}
			//}
		//}		
		//if(error_buff_new[0] == 'E'){
			//if(error_buff_new[1] == 'n'){
				//if(error_buff_new[2] == 'd'){
					//LED_On(PIN_PA14);
				//}
			//}
		//}
			
				
		error_buff_new[7] = error_buff_new[6] + (error_buff_new[5]*10) + (error_buff_new[4]*100) + (error_buff_new[3]*1000) + (error_buff_new[2]*10000) + (error_buff_new[1]*100000) + (error_buff_new[0]*1000000);
		
		if((error_buff_old[7]+1) != error_buff_new[7])	error_counter++;
		error_buff_old[7] = error_buff_new[7];
		printf("\n\r");		
		MiApp_DiscardMessage();		
		broadcast_timeout_f = 1;
		Time_Buffer = 0;
	}
}
void timer0Compare64bitTime(void)
{
	uint64_t temp64;
	if(uartAlive) {
		uartRecevingGetTime = SwTimerGetTime();
		uartAlive = 0;
		uartActive=1;
	}
	if(uartActive) {
		temp64 = SwTimerGetTime();
		// RefT0GetTime = temp64;
		// 테스트용
		temp64 -= uartRecevingGetTime;
		
		if(temp64 > 100) // > 5msec
		{
			uartRxDataIndex = serial_rx_buf_tail;
			memcpy(&uartRxData[0],&serial_rx_buf[0],uartRxDataIndex);
			memset(&serial_rx_buf[0],0,uartRxDataIndex);
			serial_rx_buf_tail=0;
			uartActive=0;
			uartCompleted=1;
		}
	}
}
void PrintMenu(void)
{
	printf("\r\nNAMOO SAMR35 MOL LPS Ver4.01\r\n");
	printf("**************************************\r\n");
	printf("AT=A: Echo ON/OFF\r\n");
	printf("AT=B: STX, ETX Input\r\n");	
	printf("AT=C: Clear NVM\r\n");
	printf("AT=D: Protocol Data Transceiver Start!\r\n");
	printf("      @@@: Exit This Mode!\r\n");
	printf("AT=E: System Reset\r\n");
	printf("AT=F: Change SF,DR\r\n");
//	printf("AT=G: Change Repeater Mode\r\n");	
	printf("AT=H: Print Menu\r\n");	
	printf("AT=J: Join Network\r\n");
	printf("AT=K: Join Network from NVM\r\n");	
	printf("AT=N: Start Network from NVM\r\n");	
	printf("AT=P: Output Power \r\n");	
	printf("AT=Q: LPS Start\r\n");
	printf("AT=R: Read NVM\r\n");
	printf("AT=S: Start Network \r\n");	
	printf("AT=U: (+)Data(+)Enter: Transfer Broadcast Data(Max 115Byte)\r\n");	
	printf("AT=W: LPS End\r\n");	
	printf("AT=Z: Sleep Mode\r\n");
		
	printf("\n\r");	
	printf("AT=1: Broadcast Message\r\n");
	printf("AT=2: Unicast Message by Index\r\n");
	printf("AT=3: Unicast Message by ADDR\r\n");
	printf("AT=4: Broadcast Test Message per 5sec\r\n");
	printf("AT=5: Unicast Test Message per 5sec\r\n");
	printf("AT=6: USART Borate Change\r\n");
	printf("AT=7: Set Operation Channel\r\n");
	printf("AT=8: Set PAN ID\r\n");
	printf("AT=9: Device Info\r\n");
	printf("AT=0: Dump Connection\r\n");
	printf("**************************************\r\n");	
}

uint8_t TransStep0,TransStep1,TransStep2, EndStep0, EndStep1, EndStep2;

void end_function(uint8_t rxChar)			//ihkim
{
	uint8_t rlt, End_Step, i;
	
	End_Step = networkInfo.ETXL;
	
	if(End_Step == '1'){
		if(rxChar == networkInfo.ETX1){
			MiApp_WriteData(rxChar);
			//Send Data	
#if 0
			//Unicast
			rlt = MiApp_UnicastConnection(0,false);
			if(rlt){
				printf("\r\nRS485 Unicast Success...\r\n");
				} else {
				printf("\r\nRS485 Unicast fail...\r\n");
			}
#else
			//Broadcast
			MiApp_BroadcastPacket(false);
			if(chkecho)	printf(" Broadcast...2\r\n");
#endif			
			SX1276_RX_INIT();
			for(i=0;i<TX_BUFFER_SIZE;i++)	TxBuffer[i] = 0;
			EndStep0 = EndStep1 = EndStep2 = 0;
			TransStep0 = TransStep1 = TransStep2 = 0;
		}else{
			MiApp_WriteData(rxChar);
		}	
	}else if(End_Step == '2'){
		switch(EndStep0){
			case 0:			
				if(rxChar == networkInfo.ETX1){
					MiApp_WriteData(rxChar);
					EndStep0 = 1;
				}else{
					MiApp_WriteData(rxChar);
					EndStep0 = EndStep1 = EndStep2 = 0;
				}
				break;
			case 1:			
				if(rxChar == networkInfo.ETX2){
					MiApp_WriteData(rxChar);
					//Send Data
#if 0
					//Unicast
					rlt = MiApp_UnicastConnection(0,false);
					if(rlt){
						printf("\r\nRS485 Unicast Success...\r\n");
						} else {
						printf("\r\nRS485 Unicast fail...\r\n");
					}
#else
					//Broadcast
					MiApp_BroadcastPacket(false);
					if(chkecho)	printf(" Broadcast...2\r\n");
#endif					
					SX1276_RX_INIT();
					for(i=0;i<TX_BUFFER_SIZE;i++)	TxBuffer[i] = 0;		
				}else{
					MiApp_WriteData(rxChar);				
				}
				EndStep0 = EndStep1 = EndStep2 = 0;
				TransStep0 = TransStep1 = TransStep2 = 0;			
				break;
		}
	}else if(End_Step == '3'){
		switch(EndStep1){
			case 0:
				if(rxChar == networkInfo.ETX1){
					MiApp_WriteData(rxChar);
					EndStep1 = 1;
				}else{
					MiApp_WriteData(rxChar);
					EndStep0 = EndStep1 = EndStep2 = 0;
				}
				break;
			case 1:
				if(rxChar == networkInfo.ETX2){
					MiApp_WriteData(rxChar);
					EndStep1 = 2;
				}else{
					MiApp_WriteData(rxChar);
					EndStep0 = EndStep1 = EndStep2 = 0;
				}
				break;
			case 2:
				if(rxChar == networkInfo.ETX3){
					MiApp_WriteData(rxChar);
					//Send Data				
#if 0
					//Unicast
					rlt = MiApp_UnicastConnection(0,false);
					if(rlt){
						printf("\r\nRS485 Unicast Success...\r\n");
					} else {
						printf("\r\nRS485 Unicast fail...\r\n");
					}
#else				
					//Broadcast
					MiApp_BroadcastPacket(false);
					if(chkecho)	printf(" Broadcast...2\r\n");
#endif		
					SX1276_RX_INIT();
					for(i=0;i<TX_BUFFER_SIZE;i++)	TxBuffer[i] = 0;
				}else{
					MiApp_WriteData(rxChar);				
				}
				EndStep0 = EndStep1 = EndStep2 = 0;
				TransStep0 = TransStep1 = TransStep2 = 0;
				break;
		}		
	}else;
}

uint8_t Exit_Protocol_Count=0;
void ConsoleProc2(void)
{
	//From RS485
	
	char rxChar,Start_Step, i;
	uint8_t rlt;
//	static bool chk0x0D = false;

	Start_Step = networkInfo.STXL;

	if((-1) != (rxChar = sio2host_getchar_nowait())){
		if(rxChar != 0xff){
			if(chkecho) printf("%c",rxChar);
			if(rxChar == '@'){
				Exit_Protocol_Count++;				
			}else{
				Exit_Protocol_Count = 0;
			}
			if(Exit_Protocol_Count >= 3){
				Exit_Protocol_Count = 0;
				chkprotocol = false;
				if(chkecho)	printf("\n\rExit Protocol Mode..!\n\r");
			}
			if(Start_Step == '1'){
				switch(TransStep0){
					case 0:
						if(rxChar == networkInfo.STX1){
							MiApp_FlushTx();
							MiApp_WriteData(rxChar);
							TransStep0 = 1;
						}else{
							TransStep1 = 0;
						}
						break;
					case 1:
						end_function(rxChar);
						break;
					default:
						MiApp_WriteData(rxChar);
						break;
				}					
			}else if(Start_Step == '2'){
				switch(TransStep1){
					case 0:
						if(rxChar == networkInfo.STX1){
							MiApp_FlushTx();
							MiApp_WriteData(rxChar);
							TransStep1 = 1;
						}else{
							TransStep1 = 0;
						}
						break;
					case 1:				
						if(rxChar == networkInfo.STX2){
							MiApp_WriteData(rxChar);
							TransStep1 = 2;
						}else{
							MiApp_WriteData(rxChar);
							TransStep1 = 0;
						}
						break;
					case 2:
						end_function(rxChar);
						break;
					default:
						MiApp_WriteData(rxChar);
						break;
				}
			}else if(Start_Step == '3'){
				switch(TransStep2){
					case 0:
						if(rxChar == networkInfo.STX1){
							MiApp_FlushTx();
							MiApp_WriteData(rxChar);
							TransStep2 = 1;
						}else{
							TransStep2 = 0;
						}
						break;
					case 1:
						if(rxChar == networkInfo.STX2){
							MiApp_WriteData(rxChar);
							TransStep2 = 2;
						}else{
							MiApp_WriteData(rxChar);
							TransStep2 = 0;
						}
						break;
					case 2:
						if(rxChar == networkInfo.STX3){
							MiApp_WriteData(rxChar);
							TransStep2 = 3;
						}else{
							MiApp_WriteData(rxChar);
							TransStep2 = 0;
						}
						break;					
					case 3:
						end_function(rxChar);
						break;
					default:
						MiApp_WriteData(rxChar);
						break;
				}
			}else;				
		}		
	}			
}
void ConsoleProc(void)
{
	char rxChar,tmp;
	char rxStr[17];
	uint8_t i,rlt,Init_Temp=1; //kwlee
	uint32_t OperatingChannel = ChannelMAP;
	uint64_t timeVal;
	
	WORD_VAL dst;
	uint8_t* ptr;

	uint32_t transe_delay_time_old, transe_delay_time_new;
	
//	static bool chk0x0D = false;
//	static bool chk0xff = false;
	
	if((-1) != (rxChar = sio2host_getchar_nowait())){		
		if(rxChar != 0xff){
			rxLenCnt++;
			if(rxLenCnt == 1){
				if((rxChar == 'A')||(rxChar == 'a')){
					if(chkecho)	printf("%c",rxChar);
				}else{
					if(chkecho)	printf("FAIL\n\r");
					rxLenCnt = 0;
					for(i=0;i<TX_BUFFER_SIZE;i++)	TxBuffer[i] = 0;
				}
			}else if(rxLenCnt == 2){
				if((rxChar == 'T')||(rxChar == 't')){
					if(chkecho)	printf("%c",rxChar);
				}else{
					if(chkecho)	printf("\n\rFAIL\n\r");
					rxLenCnt = 0;
					for(i=0;i<TX_BUFFER_SIZE;i++)	TxBuffer[i] = 0;
				}
			}else if(rxLenCnt == 3){
				if(rxChar == '='){
					if(chkecho)	printf("%c",rxChar);
				}else if(rxChar == 0x0d){
					rxLenCnt = 0;
					if(chkecho)	printf("\n\rOK\n\r");
				}else{
					if(chkecho)	printf("\n\rFAIL\n\r");
					rxLenCnt = 0;
					for(i=0;i<TX_BUFFER_SIZE;i++)	TxBuffer[i] = 0;
				}
			}else if(rxLenCnt == 4){	
				rxLenCnt = 0;
				if(chkecho)	printf("%c\n\r",rxChar);	
					
				switch(rxChar){
					case 'A':
					case 'a':
						error_counter = -1;
						if(chkecho){
							chkecho = false;
							printf("\n\rEcho OFF\n\r",rxChar);
						}else{
							chkecho = true;
							printf("\n\rEcho ON\n\r",rxChar);
						}
						
						networkInfo.echo_chk = chkecho;
						NVM_Write_NetInfo(&networkInfo);
						
						break;
					case 'b':
					case 'B':
						//	NVM Test
						NVM_Read_NetInfo(&networkInfo);
						if(chkecho) printf("\n\r read_STXL = %c",networkInfo.STXL);
						if(chkecho) printf("\n\r read_STX1 = %c",networkInfo.STX1);
						if(chkecho) printf("\n\r read_STX2 = %c",networkInfo.STX2);
						if(chkecho) printf("\n\r read_STX3 = %c",networkInfo.STX3);
						if(chkecho) printf("\n\r read_ETXL = %c",networkInfo.ETXL);
						if(chkecho) printf("\n\r read_ETX1 = %c",networkInfo.ETX1);
						if(chkecho) printf("\n\r read_ETX2 = %c",networkInfo.ETX2);
						if(chkecho) printf("\n\r read_ETX3 = %c\n\r",networkInfo.ETX3);
						//		if(chkecho) printf("\n\r read_Text_Status = %d\n\r",networkInfo.Text_Status);
		
						//	NVM Start Length & Text
						if(chkecho) printf("\r\nInput Start Length of Text(0 ~ 3) : ");
						rxChar = sio2host_getchar();
						if(chkecho) printf("%c", rxChar);
						if((rxChar >= 0x30) && (rxChar <= 0x33)){
							if(chkecho) printf("\n\rSuccess!, Start Length = %c\n\r",rxChar);
							networkInfo.STXL = rxChar;
							networkInfo.Text_Status = 1;
						}else{
							if(chkecho) printf("\n\rFail\n\r");
							if(chkecho) printf("\n\rDo Not Setting Text!\n\r");
							networkInfo.Text_Status = 0;
							break;
						}
						switch(networkInfo.STXL){
							case '0':
								networkInfo.STX1 = 0xff;
								networkInfo.STX2 = 0xff;
								networkInfo.STX3 = 0xff;
								networkInfo.ETX1 = 0xff;
								networkInfo.ETX2 = 0xff;
								networkInfo.ETX3 = 0xff;
								if(chkecho) printf("\n\rDo Not Setting Text!\n\r");
								networkInfo.Text_Status = 0;
								break;
							case '1':
								if(chkecho) printf("\r\nInput First Start of Text : ");
								rxChar = sio2host_getchar();
								if(chkecho) printf(" %c\n\rSuccess!\n\r",rxChar);
								networkInfo.STX1 = rxChar;
								networkInfo.STX2 = 0xff;
								networkInfo.STX3 = 0xff;
								break;
							case '2':
								if(chkecho) printf("\r\nInput First Start of Text : ");
								rxChar = sio2host_getchar();
								if(chkecho) printf(" %c\n\rSuccess!\n\r",rxChar);
								networkInfo.STX1 = rxChar;
								if(chkecho) printf("\r\nInput Second Start of Text : ");
								rxChar = sio2host_getchar();
								if(chkecho) printf(" %c\n\rSuccess!\n\r",rxChar);
								networkInfo.STX2 = rxChar;
								networkInfo.STX3 = 0xff;
								break;
							case '3':
								if(chkecho) printf("\r\nInput First Start of Text : ");
								rxChar = sio2host_getchar();
								if(chkecho) printf(" %c\n\rSuccess!\n\r",rxChar);
								networkInfo.STX1 = rxChar;
								if(chkecho) printf("\r\nInput Second Start of Text : ");
								rxChar = sio2host_getchar();
								if(chkecho) printf(" %c\n\rSuccess!\n\r",rxChar);
								networkInfo.STX2 = rxChar;
								if(chkecho) printf("\r\nInput Third Start of Text : ");
								rxChar = sio2host_getchar();
								if(chkecho) printf(" %c\n\rSuccess!\n\r",rxChar);
								networkInfo.STX3 = rxChar;
								break;
							default:	break;
						}
						//	NVM End Length & Text
						if(networkInfo.Text_Status){
							if(chkecho) printf("\r\nInput End Length of Text(1 ~ 3) : ");
							rxChar = sio2host_getchar();
							if(chkecho) printf("%c", rxChar);
							if((rxChar >= 0x31) && (rxChar <= 0x33)){
								printf("\n\rSuccess!, End Length = %c\n\r",rxChar);
								networkInfo.ETXL = rxChar;
							}else{
								printf("\n\rFail\n\r");
								printf("\n\rDo Not Setting Text!\n\r");
								break;
							}
							switch(networkInfo.ETXL){
								case '0':
									networkInfo.STX1 = 0xff;
									networkInfo.STX2 = 0xff;
									networkInfo.STX3 = 0xff;
									networkInfo.ETX1 = 0xff;
									networkInfo.ETX2 = 0xff;
									networkInfo.ETX3 = 0xff;
									if(chkecho) printf("\n\rDo Not Setting Text!\n\r");
									break;
								case '1':
									if(chkecho) printf("\r\nInput First End of Text : ");
									rxChar = sio2host_getchar();
									if(chkecho) printf(" %c\n\rSuccess!\n\r",rxChar);
									networkInfo.ETX1 = rxChar;
									networkInfo.ETX2 = 0xff;
									networkInfo.ETX3 = 0xff;
									break;
								case '2':
									if(chkecho) printf("\r\nInput First End of Text : ");
									rxChar = sio2host_getchar();
									if(chkecho) printf(" %c\n\rSuccess!\n\r",rxChar);
									networkInfo.ETX1 = rxChar;
									if(chkecho) printf("\r\nInput Second End of Text : ");
									rxChar = sio2host_getchar();
									if(chkecho) printf(" %c\n\rSuccess!\n\r",rxChar);
									networkInfo.ETX2 = rxChar;
									networkInfo.ETX3 = 0xff;
									break;
								case '3':
									if(chkecho) printf("\r\nInput First End of Text : ");
									rxChar = sio2host_getchar();
									if(chkecho) printf(" %c\n\rSuccess!\n\r",rxChar);
									networkInfo.ETX1 = rxChar;
									if(chkecho) printf("\r\nInput Second End of Text : ");
									rxChar = sio2host_getchar();
									if(chkecho) printf(" %c\n\rSuccess!\n\r",rxChar);
									networkInfo.ETX2 = rxChar;
									if(chkecho) printf("\r\nInput Third End of Text : ");
									rxChar = sio2host_getchar();
									if(chkecho) printf(" %c\n\rSuccess!\n\r",rxChar);
									networkInfo.ETX3 = rxChar;
									break;
							}
							if(chkecho) printf("\n\rSuccess!, Text Setting!\n\r");
							networkInfo.Text_Status = 1;
						}else{
							networkInfo.STXL = '0';
							networkInfo.STX1 = 0xff;
							networkInfo.STX2 = 0xff;
							networkInfo.STX3 = 0xff;
							networkInfo.ETXL = '0';
							networkInfo.ETX1 = 0xff;
							networkInfo.ETX2 = 0xff;
							networkInfo.ETX3 = 0xff;
							networkInfo.Text_Status = 0;
						}
						NVM_Write_NetInfo(&networkInfo);
						break;
					case 'c':
					case 'C':
						if(chkecho) printf("\n\rClear NVM...\r\n");
		
						memset(ConnectionTable,0x00,sizeof(ConnectionTable));
						NVM_Write_ConTable(ConnectionTable);
		
						memset(&networkInfo,0xff,sizeof(networkInfo));
						NVM_Write_NetInfo(&networkInfo);		
						//----------------------------------//ihkim
						networkInfo.CH = 20;
						MiApp_SetChannel(networkInfo.CH);
						networkInfo.PANID.Val = 0xabcd;
						MY_PAN_ID=networkInfo.PANID.Val;
						myPANID.Val=networkInfo.PANID.Val;
						//----------------------------------//ihkim
						if(chkecho) printf("\n\Cleared...\r\n");
						break;
					case 'D':
					case 'd':
						if(chkprotocol){
							if(chkecho) printf("\n\rSTOP Protocol Data..! ");
							chkprotocol = false;
						}else{
							//if(!networkInfo.STXL){
							NVM_Read_NetInfo(&networkInfo);
							if(chkecho) printf("\n\r read_STXL = %c",networkInfo.STXL);
							if(chkecho) printf("\n\r read_STX1 = %c",networkInfo.STX1);
							if(chkecho) printf("\n\r read_STX2 = %c",networkInfo.STX2);
							if(chkecho) printf("\n\r read_STX3 = %c",networkInfo.STX3);
							if(chkecho) printf("\n\r read_ETXL = %c",networkInfo.ETXL);
							if(chkecho) printf("\n\r read_ETX1 = %c",networkInfo.ETX1);
							if(chkecho) printf("\n\r read_ETX2 = %c",networkInfo.ETX2);
							if(chkecho) printf("\n\r read_ETX3 = %c\n\r\n\r",networkInfo.ETX3);
							//				if(chkecho) printf("\n\r read_Text_Status = %d\n\r",networkInfo.Text_Status);
							chkprotocol = true;
							//}else{
							//if(chkecho) printf("\n\rThere is no data. Please enter your data first..%c",networkInfo.STXL);
							//
							//}
						}
						break;
					case 'e':
					case 'E':
						system_reset();
						break;
					case 'f':
					case 'F':
						// SpreadingFactor [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
						if(chkecho) printf("\r\nSpreading Factor setting...Current SF - %d \r\n",SF_DR);
						if(chkecho) printf(" 07: SF7, 08: SF8, 09: SF9, 10: SF10, 11: SF11, 12: SF12 \r\n");
						rxChar = sio2host_getchar();
						tmp = (rxChar-'0')*10;
						rxChar = sio2host_getchar();
						tmp += (rxChar-'0');
		
						if(tmp>=7 && tmp<=12)
						{
							SF_DR=tmp;
							SX1276LoRaSetSpreadingFactor(SF_DR);
			
							if(chkecho) printf("\r\nSet SpreadingFactor - %d - Done\r\n\r\n",SF_DR);
							
							networkInfo.SF = SF_DR;
							NVM_Write_NetInfo(&networkInfo);
						}
						else
						{
							if(chkecho) printf("\r\nInvalid value - %d \r\n\r\n",tmp);
						}
						break;
					case 'h':
					case 'H':
						PrintMenu();
						break;
					case 'j':
					case 'J':
						set_SearchDuration = 14;
						MiApp_ConnectionMode(DISABLE_ALL_CONN);
						i = MiApp_SearchConnection(set_SearchDuration, OperatingChannel);
						if(i>0){
							if( MiApp_EstablishConnection(0, CONN_MODE_DIRECT) == 0xFF ){
								if(chkecho) printf("Join Fail....\n\r");
							} else {
								if(chkecho) printf("Join Success....2\n\r");
								DumpConnection(0xff);
								//printf("TEST_Channel %d  %d  %d\n\r",networkInfo.CH,currentChannel,RF_CHANNEL);		//ihkim
								
								MiMAC_SetChannel(RF_CHANNEL);			//ihkim					
								SX1276_RX_INIT();						//ihkim
							}
						}
						else
						{
							if(chkecho) printf("\r\nNot found network... Rescan Please...... \r\n");
						}
						
						networkInfo.POWER = RF_OUTPUT;
						networkInfo.SF = SF_DR;
						NVM_Write_NetInfo(&networkInfo);
						break;
					case 'k':
					case 'K':
						if( MiApp_EstablishConnection(0, CONN_MODE_NVM) == 0xFF ){
							if(chkecho) printf("Join Fail....\n\r");
						} else {
							if(chkecho) printf("Join Success....3\n\r");
							SX1276_RX_INIT();
							DumpConnection(0xff);							
						}
						break;
					case 'n':
					case 'N':
						MiApp_ConnectionMode(ENABLE_ALL_CONN);
						if(MiApp_StartConnection(START_FROM_NVM, 0, 0)){
							if(chkecho) printf("Network Created....\n\r");
							MiApp_ConnectionMode(DISABLE_ALL_CONN);
							DumpConnection(0xff);
						}else{
							if(chkecho) printf("Network Start Fail....\n\r");
						}
						break;
					case 'p':
					case 'P':
						if(chkecho) printf("\r\nOutput Power setting...Current POWER - %d \r\n",RF_OUTPUT);
						if(chkecho) printf("20, 17 ~ 02 dBm");
						rxChar = sio2host_getchar();
						tmp = (rxChar-'0')*10;
						rxChar = sio2host_getchar();
						tmp += (rxChar-'0');
		
						if((tmp>=2 && tmp<=17) || tmp == 20)
						{
							SX1276LoRaSetRFPower(tmp);
							RF_OUTPUT=tmp;
							if(chkecho) printf("\r\nSet Output Power - %d - Done\r\n\r\n",RF_OUTPUT);
							
							networkInfo.POWER = RF_OUTPUT;
							NVM_Write_NetInfo(&networkInfo);
						}
						else
						{
							if(chkecho) printf("\r\nInvalid value - %d \r\n\r\n",tmp);
						}
						break;
					case 'q':
					case 'Q':
						if(chkecho) printf("LPS Start...\n\r");
						MiApp_ConnectionMode(ENABLE_ALL_CONN);
						break;
					case 'r':
					case 'R':
						if(chkecho) printf("Read NVM...\r\n");
		
						NVM_Read_ConTable(ConnectionTable);
						NVM_Read_NetInfo(&networkInfo);
						
						ptr = &networkInfo;
						if(chkecho) printf("\n\rNetwork Information...\r\n");
						for(i=0;i<sizeof(networkInfo);i++){
							printf("%x ", *(ptr+i) );
							if(i%8 == 7) printf("\n\r");
						}
						ptr = ConnectionTable;
						if(chkecho) printf("\n\rConnection Table...\r\n");
						for(i=0;i<sizeof(ConnectionTable);i++){
							if(chkecho) printf("%x ", *(ptr+i) );
							if(i%8 == 7) printf("\n\r");
						}
						break;
					case 's':
					case 'S':
						set_ScanDuration = 7;//14;
		
						MiApp_ConnectionMode(ENABLE_ALL_CONN);
						if(MiApp_StartConnection(START_CONN_ENERGY_SCN, set_ScanDuration, OperatingChannel)){
							if(chkecho) printf("Network Created....\n\r");
							MiApp_BroadcastPacket(false);
							MiApp_ConnectionMode(DISABLE_ALL_CONN);
							DumpConnection(0xff);
						}else{
							if(chkecho) printf("Network Start Fail....\n\r");
						}
						break;
					case 'u':
					case 'U':
						//uartRxDataIndex=0;
						if(chkecho)	printf("======= Start UART Communication 2023.12.19=======\n\r");
						MiApp_FlushTx();
						do{
							if((-1) != (rxChar = sio2host_getchar_nowait())){
								if(rxChar != 0xff){					
									if(rxChar == 0x0D){						
										if(RF_OUTPUT==20){
											port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_INACTIVE);//kwlee 200721			//ihkim 20220509
										}else{
											port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_ACTIVE);//kwlee 200727
										}						
						
						
						
							//Broadcast
										MiApp_BroadcastPacket(false);
										if(chkecho)	printf(" Broadcast...\r\n");
										SX1276_RX_INIT();
										
										uartRxDataIndex=0;
										
							//Unicast
										//rlt = MiApp_UnicastConnection(0,false);
										//SX1276_RX_INIT();										
										//printf("\r\n");
										//if(rlt){
											//printf("Unicast Success...");
											//rxMessage.PacketRSSI = MiMAC_ChannelAssessment(CHANNEL_ASSESSMENT_CARRIER_SENSE);
											//printf("RSSI: %ddBm\r\n",rxMessage.PacketRSSI);
										//}else {
											//printf("Unicast fail..., ");
										//}
										
										
										
										
										break;
									}else{
										//uartRxData[uartRxDataIndex++] = rxChar;
										
										if(uartRxDataIndex++ <= 114){					//ihkim    MAX 115 Byte
											if(chkecho)	printf("%c",rxChar);
											MiApp_WriteData(rxChar);
										}
									}
								}
							}
						}while(1);
						break;
					case 'w':
					case 'W':
						if(chkecho) printf("LPS Stop...\n\r");
						MiApp_ConnectionMode(DISABLE_ALL_CONN);
						break;						
					case 'z':
					case 'Z':
						printf("Goto Sleep Mode!\n\r");
						Sleep();		
						break;		
					case '1':
						MiApp_FlushTx();
						MiApp_WriteData(0x31);
						MiApp_WriteData(0x32);
						MiApp_WriteData(0x33);
						MiApp_WriteData(0x34);
						MiApp_WriteData(0x35);
		
						if(RF_OUTPUT==20){
							port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_INACTIVE);//kwlee 200721
						}else{
							port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_ACTIVE);//kwlee 200727
						}
		
						MiApp_BroadcastPacket(false);
						if(chkecho) printf("Broadcast...3\r\n");
						//printf("TEST_Channel %d  %d  %d\n\r",networkInfo.CH,currentChannel,RF_CHANNEL);		//ihkim					
						port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_ACTIVE);//kwlee 200721
						SX1276_RX_INIT();
						break;
					case '2':
						MiApp_FlushTx();
						MiApp_WriteData(0x36);
						MiApp_WriteData(0x37);
						MiApp_WriteData(0x38);
						MiApp_WriteData(0x39);
						MiApp_WriteData(0x40);
		
						if(chkecho) printf("Input Connection Index:");
						rxChar = sio2host_getchar();
						if(chkecho) printf(" %c\r\n", rxChar);
		
						rxChar = rxChar - 0x30;
		
						if(RF_OUTPUT==20){
							port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_INACTIVE);//kwlee 200721		//ihkim 20220510
						}else{
							port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_ACTIVE);//kwlee 200727		//ihkim 20220510
						}
		
						rlt = MiApp_UnicastConnection(rxChar,false);
						if(rlt){
							if(chkecho) printf("Unicast Success...\r\n");
						} else {
							if(chkecho) printf("Unicast fail...\r\n");
						}
		
						SX1276_RX_INIT();
						break;
					case '3':
						MiApp_FlushTx();
						MiApp_WriteData(0x36);
						MiApp_WriteData(0x37);
						MiApp_WriteData(0x38);
						MiApp_WriteData(0x39);
						MiApp_WriteData(0x40);
		
						if(chkecho) printf("Input Short Address: 0x");
		
						memset(rxStr,0x00,sizeof(rxStr));
						for(i=0;i<4;i++){
							rxStr[i] = sio2host_getchar();
							if(chkecho) printf("%c",rxStr[i]);
						}
		
						if(chkecho) printf("\n\r");
		
						dst.Val = strtoul(rxStr,NULL,16);
		
						if(RF_OUTPUT==20){
							port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_INACTIVE);//kwlee 200721		//ihkim 20220510
						}else{
							port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_ACTIVE);//kwlee 200727		//ihkim 20220510
						}		
		
						rlt = UnicastShortAddress(dst.v);
						if(rlt){
							if(chkecho) printf("Unicast Success...\r\n");
						} else {
							if(chkecho) printf("Unicast fail...\r\n");
						}
		
						SX1276_RX_INIT();
						break;	
					case '4':
						if(broadcast_test_f){
							if(chkecho) printf("Broadcast Test Stop !\n\r");
							broadcast_test_f = 0;
							}else{
							broadcast_test_f = 1;
							if(chkecho) printf("Broadcast Test Start with 26 Byte !\n\r");
							Time_Buffer2 = 200000;
						}
						break;		
					case '5':
						Time_Counter = 0;
						error_count=0;
						data_count = 0;
						if(unicast_test_f){
							unicast_test_f = 0;
							if(chkecho) printf("Unicast Test Stop !\n\r");
						}else{
							unicast_test_f = 1;
//							broadcast_test_f = 0;
							if(chkecho) printf("Unicast Test Start with 26 Byte !\n\r");
						}
						Time_Buffer2 = 200000;
						break;		
					case '6':
						//if(chkecho) printf("USART Borate Input (ex, 1=9600, 2=19200, 3=38400, 4=115200)\n\r");
						//rxChar = sio2host_getchar();
						//if(chkecho) printf(" %c\r\n", rxChar);
			//
						//switch(rxChar){
							//case '1':
								//networkInfo.Uart_Borate = 96;
								//if(chkecho) printf("USART Borate = 9600 bps setting OK!\n\r");
								//NVM_Write_NetInfo(&networkInfo);
								//NVM_Write_ConTable(ConnectionTable);
								//break;
							//case '2':
								//networkInfo.Uart_Borate = 192;
								//if(chkecho) printf("USART Borate = 19200 bps setting OK!\n\r");
								//NVM_Write_NetInfo(&networkInfo);
								//NVM_Write_ConTable(ConnectionTable);
								//break;
							//case '3':
								//networkInfo.Uart_Borate = 384;
								//if(chkecho) printf("USART Borate = 38400 bps setting OK!\n\r");
								//NVM_Write_NetInfo(&networkInfo);
								//NVM_Write_ConTable(ConnectionTable);
								//break;
							//case '4':
								//networkInfo.Uart_Borate = 1152;
								//if(chkecho) printf("USART Borate = 115200 bps setting OK!\n\r");
								//NVM_Write_NetInfo(&networkInfo);
								//NVM_Write_ConTable(ConnectionTable);
								//break;
							//default:
								//if(chkecho) printf("USART Borate setting Fail\n\r");	
						//}
						//break;
						
											
						
						if(chkecho) printf("USART Borate Input (ex, 1=2400, 2=9600, 3=14400, 4=19200, 5=38400, 6=57600, 7=115200)\n\r");
						rxChar = sio2host_getchar();
						if(chkecho) printf(" %c\r\n", rxChar);			
			
						memset(ConnectionTable,0x00,sizeof(ConnectionTable));
						NVM_Write_ConTable(ConnectionTable);
						memset(&networkInfo,0xff,sizeof(networkInfo));
						NVM_Write_NetInfo(&networkInfo);
			
						switch(rxChar){
							case '1':
								networkInfo.Uart_Borate = 24;
								if(chkecho) printf("USART Borate = 2400 bps setting OK!\n\r");
								NVM_Write_NetInfo(&networkInfo);
								NVM_Write_ConTable(ConnectionTable);
								break;
							case '2':
								networkInfo.Uart_Borate = 96;
								if(chkecho) printf("USART Borate = 9600 bps setting OK!\n\r");
								NVM_Write_NetInfo(&networkInfo);
//								NVM_Write_ConTable(ConnectionTable);
								break;
							case '3':
								networkInfo.Uart_Borate = 144;
								if(chkecho) printf("USART Borate = 14400 bps setting OK!\n\r");
								NVM_Write_NetInfo(&networkInfo);
//								NVM_Write_ConTable(ConnectionTable);
								break;
							case '4':
								networkInfo.Uart_Borate = 192;
								if(chkecho) printf("USART Borate = 19200 bps setting OK!\n\r");
								NVM_Write_NetInfo(&networkInfo);
//								NVM_Write_ConTable(ConnectionTable);
								break;
							case '5':
								networkInfo.Uart_Borate = 384;
								if(chkecho) printf("USART Borate = 38400 bps setting OK!\n\r");
								NVM_Write_NetInfo(&networkInfo);
//								NVM_Write_ConTable(ConnectionTable);
								break;
							case '6':
								networkInfo.Uart_Borate = 576;
								if(chkecho) printf("USART Borate = 57600 bps setting OK!\n\r");
								NVM_Write_NetInfo(&networkInfo);
//								NVM_Write_ConTable(ConnectionTable);
								break;
							case '7':
								networkInfo.Uart_Borate = 1152;
								if(chkecho) printf("USART Borate = 115200 bps setting OK!\n\r");
								NVM_Write_NetInfo(&networkInfo);
//								NVM_Write_ConTable(ConnectionTable);
								break;
							default:
								if(chkecho) printf("USART Borate setting Fail\n\r");	
						}
						break;
			
					case '7':
						if(chkecho) printf("\r\nChannel setting...Current Chanel - %d",networkInfo.CH);
						if(chkecho) printf("\r\nChannel: 01 ~ 32 ( 917.1MHz ~ 923.3MHz )");
		
						rxChar = sio2host_getchar();
						tmp = (rxChar-'0')*10;
						rxChar = sio2host_getchar();
						tmp += (rxChar-'0');
		
						if(tmp>0 && tmp<33)
						{
							NVM_Read_NetInfo(&networkInfo);
							MiMAC_SetChannel(tmp);
							RF_CHANNEL=tmp;
							currentChannel=RF_CHANNEL;
							networkInfo.CH = currentChannel;
			
							if(chkecho) printf("\r\nSet Channel - %d - Done\r\n\r\n",tmp);
							SX1276_RX_INIT();
							
							
//							memset(ConnectionTable,0x00,sizeof(ConnectionTable));
//							NVM_Write_ConTable(ConnectionTable);
							NVM_Write_NetInfo(&networkInfo);			//ihkim
						}
						else
						{
							if(chkecho) printf("\r\nInvalid value - %d \r\n\r\n",tmp);
						}
						
						break;
					case '8':
						if(chkecho) printf("\n\rCurrent PANID: 0x%.4x \r\n",myPANID.Val);
						if(chkecho) printf("    New PANID: 0x");
						
						memset(rxStr,0x00,sizeof(rxStr));
						for(i=0;i<4;i++){
							rxStr[i] = sio2host_getchar();
							printf("%c",rxStr[i]);
						}						
						myPANID.Val = strtoul(rxStr,NULL,16);
						
						if(chkecho) printf("\n\rPANID Changed...\n\r");		
						
						networkInfo.PANID.Val = myPANID.Val;
						NVM_Write_NetInfo(&networkInfo);
						break;
					case '9':
						//NVM_Read_NetInfo(&networkInfo);						
						//currentChannel = networkInfo.CH;
						if(chkecho) printf("\n\Device Information...\n\r");
						if(chkecho) printf("Current Channel: %d \r\n",currentChannel);
						if(chkecho) printf("Channel MAP: 0x%.8x \r\n",ChannelMAP);
						if(chkecho) printf("PANID: 0x%.4x \r\n",myPANID.Val);
						if(chkecho) printf("ShortAddr: 0x%.4x \r\n", myShortAddress.Val);
						if(chkecho) printf("MAC Address: 0x");
						for(i = 0; i < MY_ADDRESS_LENGTH; i++)
						{
							printf("%.2x",myLongAddress[MY_ADDRESS_LENGTH-1-i]);
						}
						if(chkecho) printf("\r\nSet SpreadingFactor - %d - \r\n",SF_DR);
						if(chkecho) printf("\rSet Output Power - %d - \r\n",RF_OUTPUT);
						break;
					case '0':
						if(chkecho) printf("\n\rConnection Table...\n\r");
						DumpConnection(0xff);
						break;
					default:
						//printf("\n\rnInvalid value.. Try Start over ! \n\r");
						if(chkecho)	printf("\n\rFAIL\n\r");
						rxLenCnt = 0;
						for(i=0;i<TX_BUFFER_SIZE;i++)	TxBuffer[i] = 0;
						break;
				}	
						
			}else if(rxLenCnt == 10){
				rxLenCnt = 0;
				for(i=0;i<TX_BUFFER_SIZE;i++)	TxBuffer[i] = 0;
				if(chkecho)	printf("  Error.. Exceed 10..");					
			}else if(rxChar == 0x0d){
				rxLenCnt = 0;
				MiApp_FlushTx();
				if(chkecho)	printf(" Enter\n\r");
				SX1276_RX_INIT();
			}else{
				MiApp_WriteData(rxChar);
				if(chkecho)	printf("%c",rxChar);
			}
		}
	}
}

void test_console(void){
	uint8_t rlt;
	
	uint32_t t3;
	
	if(unicast_test_f){
		if(Time_Buffer2++ > 2500){				//5sec  =15000		7500
			Time_Buffer2 = 0;
			Time_Counter++;
				
			MiApp_FlushTx();
			MiApp_WriteData('D');
			MiApp_WriteData('a');
			MiApp_WriteData('t');
			MiApp_WriteData('a');
			MiApp_WriteData('=');			
			MiApp_WriteData((Time_Counter/1000000)+0x30);
			MiApp_WriteData((Time_Counter%1000000/100000)+0x30);
			MiApp_WriteData((Time_Counter%1000000%100000/10000)+0x30);
			MiApp_WriteData((Time_Counter%1000000%100000%10000/1000)+0x30);
			MiApp_WriteData((Time_Counter%1000000%100000%10000%1000/100)+0x30);
			MiApp_WriteData((Time_Counter%1000000%100000%10000%1000%100/10)+0x30);
			MiApp_WriteData((Time_Counter%1000000%100000%10000%1000%100%10)+0x30);
			MiApp_WriteData(',');
			MiApp_WriteData(' ');
			MiApp_WriteData('E');
			MiApp_WriteData('r');
			MiApp_WriteData('r');
			MiApp_WriteData('o');
			MiApp_WriteData('r');
			MiApp_WriteData('=');
			MiApp_WriteData((error_count/1000)+0x30);
			MiApp_WriteData((error_count%1000/100)+0x30);
			MiApp_WriteData((error_count%1000%100/10)+0x30);
			MiApp_WriteData((error_count%1000%100%10)+0x30);
			MiApp_WriteData(' ');	//25
			MiApp_WriteData(' ');	//26
			MiApp_WriteData(' ');	//27
			MiApp_WriteData('T');	//28
			MiApp_WriteData('o');	//29
			MiApp_WriteData('t');	//30
			MiApp_WriteData('a');	//31
			MiApp_WriteData('l');	//32
			MiApp_WriteData(':');	//33
			MiApp_WriteData('6');	//34
			MiApp_WriteData('0');	//35
			MiApp_WriteData(' ');	//36
			MiApp_WriteData('B');	//37
			MiApp_WriteData('y');	//38
			MiApp_WriteData('t');	//39
			MiApp_WriteData('e');	//40
			MiApp_WriteData(' ');	//41
			MiApp_WriteData('t');	//42
			MiApp_WriteData('r');	//43
			MiApp_WriteData('a');	//44
			MiApp_WriteData('n');	//45
			MiApp_WriteData('s');	//46
			MiApp_WriteData('f');	//47
			MiApp_WriteData('e');	//48
			MiApp_WriteData('r');	//49
			MiApp_WriteData(' ');	//50
			MiApp_WriteData('C');	//51
			MiApp_WriteData('o');	//52
			MiApp_WriteData('m');	//53
			MiApp_WriteData('p');	//54
			MiApp_WriteData('l');	//55
			MiApp_WriteData('e');	//56
			MiApp_WriteData('t');	//57
			MiApp_WriteData('e');	//58
			MiApp_WriteData('.');	//59
			MiApp_WriteData('!');	//60
			//
			//
			//
			//
			//MiApp_WriteData('5');
			//MiApp_WriteData('6');
			//MiApp_WriteData('7');
			//MiApp_WriteData('8');
			//MiApp_WriteData('9');
			//MiApp_WriteData('0');			//30
			//MiApp_WriteData('1');
			//MiApp_WriteData('2');
			//MiApp_WriteData('3');
			//MiApp_WriteData('4');
			//MiApp_WriteData('5');
			//MiApp_WriteData('6');
			//MiApp_WriteData('7');
			//MiApp_WriteData('8');
			//MiApp_WriteData('9');
			//MiApp_WriteData('0');			//40
			//MiApp_WriteData('1');
			//MiApp_WriteData('2');
			//MiApp_WriteData('3');
			//MiApp_WriteData('4');
			//MiApp_WriteData('5');
			//MiApp_WriteData('6');
			//MiApp_WriteData('7');
			//MiApp_WriteData('8');
			//MiApp_WriteData('9');
			//MiApp_WriteData('0');
			//MiApp_WriteData('1');
			//MiApp_WriteData('2');
			//MiApp_WriteData('3');
			//MiApp_WriteData('4');
			//MiApp_WriteData('5');
			//MiApp_WriteData('6');
			//MiApp_WriteData('7');
			//MiApp_WriteData('8');
			//MiApp_WriteData('9');
			//MiApp_WriteData('0');
			//MiApp_WriteData('1');
			//MiApp_WriteData('2');
			//MiApp_WriteData('3');
			//MiApp_WriteData('4');
			//MiApp_WriteData('5');
			//MiApp_WriteData('6');
			//MiApp_WriteData('7');
			//MiApp_WriteData('8');
			//MiApp_WriteData('9');
			//MiApp_WriteData('0');
			//MiApp_WriteData('1');
			//MiApp_WriteData('2');
			//MiApp_WriteData('3');
			//MiApp_WriteData('4');
			//MiApp_WriteData('5');
			//MiApp_WriteData('6');
			//MiApp_WriteData('7');
			//MiApp_WriteData('8');
			//MiApp_WriteData('9');
			//MiApp_WriteData('0');
			//MiApp_WriteData('1');
			//MiApp_WriteData('2');
			//MiApp_WriteData('3');
			//MiApp_WriteData('4');
			//MiApp_WriteData('5');
			//MiApp_WriteData('6');
			//MiApp_WriteData('7');
			//MiApp_WriteData('8');
			//MiApp_WriteData('9');
			//MiApp_WriteData('0');
			//MiApp_WriteData('1');
			//MiApp_WriteData('2');
			//MiApp_WriteData('3');
			//MiApp_WriteData('4');
			//MiApp_WriteData('5');
			//MiApp_WriteData('6');
			//MiApp_WriteData('7');
			//MiApp_WriteData('8');
			//MiApp_WriteData('9');
			//MiApp_WriteData('0');		//100
			//MiApp_WriteData('1');
			//MiApp_WriteData('2');
			//MiApp_WriteData('3');
			//MiApp_WriteData('4');
			//MiApp_WriteData('5');
			//MiApp_WriteData('6');
			//MiApp_WriteData('7');
			//MiApp_WriteData('8');
			//MiApp_WriteData('9');
			//MiApp_WriteData('0');		//110
			//MiApp_WriteData('1');
			//MiApp_WriteData('2');
			//MiApp_WriteData('3');
			
			
			//transe_delay_time_new = MiWi_TickGet();
			
			if(RF_OUTPUT==20){
				port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_INACTIVE);//kwlee 200721
			}else{
				port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_ACTIVE);//kwlee 200727
			}
			
			t1 = MiWi_TickGet();
			rlt = MiApp_UnicastConnection(0,false); //?
			t2 = MiWi_TickGet();
			
			//port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_ACTIVE);//kwlee 200721		//ihkim 20220509
			SX1276_RX_INIT();
			
			t3 = t2.Val - t1.Val;
			
			if(rlt){
				//data_count++;
				printf("Unicast Success... Delay=%d, ",t3);
				printf("Data=%d, ",Time_Counter);
				printf("Error=%d\r\n",error_count);
			} else {
				error_count++;
				printf("Unicast fail..., ");
				printf("Error=%d\r\n",error_count);
			}
		}
	}
	
	if(broadcast_test_f){
		if(Time_Buffer++ > 2000){		//5sec  =15000				1000
			Time_Buffer = 0;
			Time_Counter++;
			
			MiApp_FlushTx();
			
			MiApp_WriteData('B');
			MiApp_WriteData('r');
			MiApp_WriteData('o');
			MiApp_WriteData('a');
			MiApp_WriteData('d');
			MiApp_WriteData('c');
			MiApp_WriteData('a');
			MiApp_WriteData('s');
			MiApp_WriteData('t');
			MiApp_WriteData('.');
			MiApp_WriteData('.');
			MiApp_WriteData(' ');
			MiApp_WriteData('D');
			MiApp_WriteData('a');
			MiApp_WriteData('t');
			MiApp_WriteData('a');
			MiApp_WriteData('=');
			MiApp_WriteData((Time_Counter/1000000)+0x30);
			MiApp_WriteData((Time_Counter%1000000/100000)+0x30);
			MiApp_WriteData((Time_Counter%1000000%100000/10000)+0x30);
			MiApp_WriteData((Time_Counter%1000000%100000%10000/1000)+0x30);
			MiApp_WriteData((Time_Counter%1000000%100000%10000%1000/100)+0x30);
			MiApp_WriteData((Time_Counter%1000000%100000%10000%1000%100/10)+0x30);
			MiApp_WriteData((Time_Counter%1000000%100000%10000%1000%100%10)+0x30);
			MiApp_WriteData(' ');	//25
			MiApp_WriteData(' ');	//26			
			MiApp_WriteData(' ');	//27
			MiApp_WriteData('T');	//28
			MiApp_WriteData('o');	//29
			MiApp_WriteData('t');	//30
			MiApp_WriteData('a');	//31
			MiApp_WriteData('l');	//32			
			MiApp_WriteData(':');	//33			
			MiApp_WriteData('6');	//34
			MiApp_WriteData('0');	//35
			MiApp_WriteData(' ');	//36
			MiApp_WriteData('B');	//37
			MiApp_WriteData('y');	//38
			MiApp_WriteData('t');	//39
			MiApp_WriteData('e');	//40
			MiApp_WriteData(' ');	//41
			MiApp_WriteData('t');	//42
			MiApp_WriteData('r');	//43
			MiApp_WriteData('a');	//44
			MiApp_WriteData('n');	//45
			MiApp_WriteData('s');	//46
			MiApp_WriteData('f');	//47
			MiApp_WriteData('e');	//48
			MiApp_WriteData('r');	//49
			MiApp_WriteData(' ');	//50
			MiApp_WriteData('C');	//51
			MiApp_WriteData('o');	//52
			MiApp_WriteData('m');	//53
			MiApp_WriteData('p');	//54
			MiApp_WriteData('l');	//55
			MiApp_WriteData('e');	//56
			MiApp_WriteData('t');	//57
			MiApp_WriteData('e');	//58
			MiApp_WriteData('.');	//59
			MiApp_WriteData('!');	//60
			
			
			
			if(RF_OUTPUT==20){
				port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_INACTIVE);//kwlee 200721
			}else{
				port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_ACTIVE);//kwlee 200727
			}
			
			MiApp_BroadcastPacket(false);
			printf("Test Broadcast... Counter : %d\r\n",Time_Counter);
			
			//port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_ACTIVE);//kwlee 200721
			SX1276_RX_INIT();
		}
	}
}

void unicast_test_console(void){
	uint8_t rlt;
	//MIWI_TICK t1, t2;
	//uint32_t t3;
	
	if(unicast_test_f){
		if(Time_Buffer2++ > 7000){				//5sec  =15000
			Time_Buffer2 = 0;
			Time_Counter++;
			
			MiApp_FlushTx();
		
			MiApp_WriteData('D');
			MiApp_WriteData('a');
			MiApp_WriteData('t');
			MiApp_WriteData('a');
			MiApp_WriteData('=');
			MiApp_WriteData((Time_Counter/1000000)+0x30);
			MiApp_WriteData((Time_Counter%1000000/100000)+0x30);
			MiApp_WriteData((Time_Counter%1000000%100000/10000)+0x30);
			MiApp_WriteData((Time_Counter%1000000%100000%10000/1000)+0x30);
			MiApp_WriteData((Time_Counter%1000000%100000%10000%1000/100)+0x30);
			MiApp_WriteData((Time_Counter%1000000%100000%10000%1000%100/10)+0x30);
			MiApp_WriteData((Time_Counter%1000000%100000%10000%1000%100%10)+0x30);
			MiApp_WriteData(',');
			MiApp_WriteData(' ');
			MiApp_WriteData('E');
			MiApp_WriteData('r');
			MiApp_WriteData('r');
			MiApp_WriteData('o');
			MiApp_WriteData('r');
			MiApp_WriteData('=');
			MiApp_WriteData((error_count/1000)+0x30);
			MiApp_WriteData((error_count%1000/100)+0x30);
			MiApp_WriteData((error_count%1000%100/10)+0x30);
			MiApp_WriteData((error_count%1000%100%10)+0x30);
			
			//transe_delay_time_new = MiWi_TickGet();
			
			if(RF_OUTPUT==20){
				port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_INACTIVE);//kwlee 200721
			}else{
				port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_ACTIVE);//kwlee 200727
			}
			
			t1 = MiWi_TickGet();
			rlt = MiApp_UnicastConnection(0,false);
			t2 = MiWi_TickGet();
			
			t3 = t2.Val - t1.Val;
			
			if(rlt){
				data_count++;
				printf("Unicast Success... Delay=%d, ",t3);
				printf("Data=%d, ",Time_Counter);
				printf("Error=%d\r\n",error_count);
		
				rxMessage.PacketRSSI = MiMAC_ChannelAssessment(CHANNEL_ASSESSMENT_CARRIER_SENSE);
				printf("RSSI: %ddBm\r\n",rxMessage.PacketRSSI);		
				
			}else {
				error_count++;
				printf("Unicast fail..., ");
				printf("Error=%d\r\n",error_count);
			}			
			SX1276_RX_INIT();
		}
	}
}