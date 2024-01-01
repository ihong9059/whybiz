/**
* \file  main.c
*
* \brief Serial Provisioning of LoRaWAN Demo Application
*
* Copyright (c) 2019 Microchip Technology Inc. and its subsidiaries.
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


/****************************** INCLUDES **************************************/
#include "sys.h"
#include "system_init.h"
#include "system_assert.h"
#include "aes_engine.h"
#include "sio2host.h"
#include "extint.h"
#include "conf_app.h"
#include "sw_timer.h"
#include "common_hw_timer.h"

#include "radio_driver_SX1276.h"
#include "radio_driver_hal.h"
#include "radio_registers_SX1276.h"
#include "MiWi.h"
#include "nvm.h"
#include "SymbolTime.h"
#include "system_low_power.h"

#include "pmm.h"
#include "conf_pmm.h"
#include "sleep_timer.h"
#include "sleep.h"

#include "application.h"
#include "certitest.h"
#include "conf_sio2host.h"

#include "uttec.h"

#if (ENABLE_PDS == 1)
#include "pds_interface.h"
#endif


//extern unsigned int MY_PAN_ID;
#define OPCHANNEL		0xffffffff//0x04000000 //400000 23//4000000 27// 80000 20
uint8_t Data_rx;
extern uint8_t serial_rx_buf_tail;
extern uint8_t serial_rx_buf_tail1;
uint16_t TEMP_PAN_ID;
uint8_t RX_TEMP[10];
uint8_t uartRxData[SERIAL_RX_BUF_SIZE_HOST], uartRxDataIndex;
uint64_t Test_Borate;

extern uint8_t serial_rx_buf1[SERIAL_RX_BUF_SIZE_HOST];
extern uint8_t serial_rx_buf[SERIAL_RX_BUF_SIZE_HOST];

extern uint8_t myLongAddress[8];
extern  void edbg_eui_read_eui64(uint8_t *eui);
uint8_t RF_OUTPUT=12,RF_CHANNEL=12,SF_DR=7,RF_BW=7;
//extern uint8_t USE_CHANNEL_NUM_START;
//extern uint8_t USE_CHANNEL_NUM_END;
//extern uint8_t INIT_CHANNEL_NUM;

extern bool chkprotocol;
extern bool chkecho;
extern void ConsoleProc2(void);

/************************** Macro definition ***********************************/

/* Button debounce time in ms */
#define APP_DEBOUNCE_TIME       50


/************************** Global variables ***********************************/
bool button_pressed = false;
bool factory_reset = false;
bool bandSelected = false;
bool wait_nvm_Check;

uint32_t longPress = 0;


extern bool certAppEnabled;
#ifdef CONF_PMM_ENABLE
bool deviceResetsForWakeup = false;
#endif

#if ADDITIONAL_NODE_ID_SIZE > 0
uint8_t AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {0x01};
#endif

/************************** Extern variables ***********************************/


/************************** Function Prototypes ********************************/
static void driver_init(void);
static void config_init(void);
void start_nvm(void);

void Sleep(void);


#if (_DEBUG_ == 1)
static void assertHandler(SystemAssertLevel_t level, uint16_t code);
#endif /* #if (_DEBUG_ == 1) */

/*********************************************************************//**
 \brief      Uninitializes app resources before going to low power mode
*************************************************************************/
#ifdef CONF_PMM_ENABLE
static void app_resources_uninit(void);
#endif

/****************************** FUNCTIONS **************************************/

static void print_reset_causes(void)
{
    enum system_reset_cause rcause = system_get_reset_cause();
    if(chkecho) printf("Last reset cause: ");
    if(rcause & (1 << 6)) {
        if(chkecho) printf("System Reset Request\r\n");
    }
    if(rcause & (1 << 5)) {
        if(chkecho) printf("Watchdog Reset\r\n");
    }
    if(rcause & (1 << 4)) {
        if(chkecho) printf("External Reset\r\n");
    }
    if(rcause & (1 << 2)) {
        if(chkecho) printf("Brown Out 33 Detector Reset\r\n");
    }
    if(rcause & (1 << 1)) {
        if(chkecho) printf("Brown Out 12 Detector Reset\r\n");
    }
    if(rcause & (1 << 0)) {
        if(chkecho) printf("Power-On Reset\r\n");
    }
}

#ifdef CONF_PMM_ENABLE
static void appWakeup(uint32_t sleptDuration)
{
    HAL_Radio_resources_init();
    sio2host_init();
    if(chkecho) printf("\r\nsleep_ok %ld ms\r\n", sleptDuration);

}
#endif

#if (_DEBUG_ == 1)
static void assertHandler(SystemAssertLevel_t level, uint16_t code)
{
    if(chkecho) printf("\r\n%04x\r\n", code);
    (void)level;
}
#endif /* #if (_DEBUG_ == 1) */

static void extint_callback(void)
{

}

static void configure_extint_channel(void)
{

	struct extint_chan_conf config_extint_chan;
	extint_chan_get_config_defaults(&config_extint_chan);
	config_extint_chan.gpio_pin           = BUTTON_0_EIC_PIN;
	config_extint_chan.gpio_pin_mux       = BUTTON_0_EIC_MUX;
	config_extint_chan.gpio_pin_pull      = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
	extint_chan_set_config(BUTTON_0_EIC_LINE, &config_extint_chan);
	extint_register_callback(extint_callback,BUTTON_0_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(BUTTON_0_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
	while (extint_chan_is_detected(BUTTON_0_EIC_LINE)) {
		extint_chan_clear_detected(BUTTON_0_EIC_LINE);
	}
}


static void main_clock_select_osc16m(void)
{
	struct system_gclk_gen_config gclk_conf;
	struct system_clock_source_osc16m_config osc16m_conf;

	/* Switch to new frequency selection and enable OSC16M */
	system_clock_source_osc16m_get_config_defaults(&osc16m_conf);
	osc16m_conf.fsel = CONF_CLOCK_OSC16M_FREQ_SEL;
	osc16m_conf.on_demand = 0;
	osc16m_conf.run_in_standby = CONF_CLOCK_OSC16M_RUN_IN_STANDBY;
	system_clock_source_osc16m_set_config(&osc16m_conf);
	system_clock_source_enable(SYSTEM_CLOCK_SOURCE_OSC16M);
	while(!system_clock_source_is_ready(SYSTEM_CLOCK_SOURCE_OSC16M));

	/* Select OSC16M as mainclock */
	system_gclk_gen_get_config_defaults(&gclk_conf);
	gclk_conf.source_clock = SYSTEM_CLOCK_SOURCE_OSC16M;
	system_gclk_gen_set_config(GCLK_GENERATOR_0, &gclk_conf);
	if (CONF_CLOCK_OSC16M_ON_DEMAND) {
		OSCCTRL->OSC16MCTRL.reg |= OSCCTRL_OSC16MCTRL_ONDEMAND;
	}

}

static void main_clock_select_dfll(void)
{
	struct system_gclk_gen_config gclk_conf;

	/* Select OSCULP32K as new clock source for mainclock temporarily */
	system_gclk_gen_get_config_defaults(&gclk_conf);
	gclk_conf.source_clock = SYSTEM_CLOCK_SOURCE_XOSC32K;
	system_gclk_gen_set_config(GCLK_GENERATOR_0, &gclk_conf);

	/* Select XOSC32K for GCLK1. */
	system_gclk_gen_get_config_defaults(&gclk_conf);
	gclk_conf.source_clock = SYSTEM_CLOCK_SOURCE_XOSC32K;
	system_gclk_gen_set_config(GCLK_GENERATOR_1, &gclk_conf);
	system_gclk_gen_enable(GCLK_GENERATOR_1);

	struct system_gclk_chan_config dfll_gclk_chan_conf;

	system_gclk_chan_get_config_defaults(&dfll_gclk_chan_conf);
	dfll_gclk_chan_conf.source_generator = GCLK_GENERATOR_1;
	system_gclk_chan_set_config(OSCCTRL_GCLK_ID_DFLL48, &dfll_gclk_chan_conf);
	system_gclk_chan_enable(OSCCTRL_GCLK_ID_DFLL48);

	struct system_clock_source_dfll_config dfll_conf;
	system_clock_source_dfll_get_config_defaults(&dfll_conf);

	dfll_conf.loop_mode      = SYSTEM_CLOCK_DFLL_LOOP_MODE_CLOSED;
	dfll_conf.on_demand      = false;
	dfll_conf.run_in_stanby  = CONF_CLOCK_DFLL_RUN_IN_STANDBY;
	dfll_conf.multiply_factor = CONF_CLOCK_DFLL_MULTIPLY_FACTOR;
	system_clock_source_dfll_set_config(&dfll_conf);
	system_clock_source_enable(SYSTEM_CLOCK_SOURCE_DFLL);
	while(!system_clock_source_is_ready(SYSTEM_CLOCK_SOURCE_DFLL));
	if (CONF_CLOCK_DFLL_ON_DEMAND) {
		OSCCTRL->DFLLCTRL.bit.ONDEMAND = 1;
	}

	/* Select DFLL for mainclock. */
	system_gclk_gen_get_config_defaults(&gclk_conf);
	gclk_conf.source_clock = SYSTEM_CLOCK_SOURCE_DFLL;
	system_gclk_gen_set_config(GCLK_GENERATOR_0, &gclk_conf);

}

static void main_clock_select(const enum system_clock_source clock_source)
{
	if (clock_source == SYSTEM_CLOCK_SOURCE_DFLL) {
		main_clock_select_dfll();
		system_clock_source_disable(SYSTEM_CLOCK_SOURCE_OSC16M);
	} else if (clock_source == SYSTEM_CLOCK_SOURCE_OSC16M) {
		main_clock_select_osc16m();
		system_clock_source_disable(SYSTEM_CLOCK_SOURCE_DFLL);
		system_gclk_chan_disable(OSCCTRL_GCLK_ID_DFLL48);
		system_gclk_gen_disable(GCLK_GENERATOR_1);
	} else {
		return ;
	}
}

static void test_active_mode(const enum system_performance_level performance_level)
{

	enum system_performance_level curr_pl = system_get_performance_level();

	printf("System will switch to PL:%d \r\n",performance_level);
	if (curr_pl == performance_level) {
		return ;
	}

	if (curr_pl < performance_level) {

		/* Scaling up the performance level first and then increase clock frequency */
		system_switch_performance_level(performance_level);
		main_clock_select(SYSTEM_CLOCK_SOURCE_DFLL);

	} else {
		/* Scaling down clock frequency and then Scaling down the performance level */
		main_clock_select(SYSTEM_CLOCK_SOURCE_OSC16M);
		system_switch_performance_level(performance_level);
	}

	/* Toggles LED0 once clock frequency successfully */
	//led_toggle_indication(LED0_TOGGLE_2);
}


static void put_radio_to_sleep(void)
{
	uint8_t op_mode, current_mode;
	uint8_t new_mode = 0x00;
	
	/* Initialize the Radio Hardware */
	//HAL_RadioInit();
	
	/* Power On the TCXO Oscillator */
	//HAL_TCXOPowerOn();
	
	/* Reset the radio */
	//RADIO_Reset();

	op_mode = RADIO_RegisterRead(0x01);
	current_mode = op_mode & 0x07;
	
	if (new_mode != current_mode)
	{
		// Do the actual mode switch.
		op_mode &= ~0x07;                // Clear old mode bits
		op_mode |= new_mode;             // Set new mode bits
		while (op_mode != RADIO_RegisterRead(0x01))
		{
			RADIO_RegisterWrite(0x01, op_mode);
		}
	}
	
	/* Power off the oscillator after putting radio to sleep */
	HAL_TCXOPowerOff();
	
	/* Disable the SPI interface */
	HAL_RadioDeInit();
}

void Sleep(void)
{
	if(chkecho) printf("Goto Sleep Mode!\n\r");
	
//-----------	25uA / 1.8V	
	put_radio_to_sleep();

	sio2host_disable();
	sio2host_disable2();
	
	port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_INACTIVE);
	
	port_pin_set_output_level(PIN_PA04, false);
	port_pin_set_output_level(PIN_PA05, false);
	port_pin_set_output_level(PIN_PA28, false);
	port_pin_set_output_level(PIN_PA09, false);
	port_pin_set_output_level(PIN_PA13, false);	
	port_pin_set_output_level(PIN_PA08, false);
	port_pin_set_output_level(PIN_PA06, false);		
	port_pin_set_output_level(LED_0_PIN, LED_0_INACTIVE);
	port_pin_set_output_level(LED_1_PIN, LED_1_INACTIVE);	
	system_gclk_chan_disable(SERCOM0_GCLK_ID_CORE);
	system_clock_source_disable(SYSTEM_CLOCK_SOURCE_XOSC32K);

	system_set_sleepmode(SYSTEM_SLEEPMODE_BACKUP);
			
	system_sleep();
	
	system_clock_source_enable(SYSTEM_CLOCK_SOURCE_XOSC32K);
}


/* Initializes all the hardware and software modules used for Stack operation */
static void driver_init(void)
{
    /* Initialize the Radio Hardware */
    HAL_RadioInit();
    /* Initialize the AES Hardware Engine */
    AESInit();
    /* Initialize the Software Timer Module */
    SystemTimerInit();
#ifdef CONF_PMM_ENABLE
    /* Initialize the Sleep Timer Module */
    SleepTimerInit();
#endif
#if (ENABLE_PDS == 1)
    /* PDS Module Init */
    PDS_Init();
#endif
}

static void config_init(void)
{
	SX1276_Reset();
	SX1276_Config();
	SX1276_RX_INIT();
	
	//Read Netinfo
	NVM_Read_NetInfo(&networkInfo);
		
	if((networkInfo.CH > 0)&&(networkInfo.CH < 33)){
		currentChannel = networkInfo.CH;
	}else{
		currentChannel = networkInfo.CH = 20;
		
		NVM_Write_NetInfo(&networkInfo);			//ihkim
		//		NVM_Write_ConTable(ConnectionTable);
	}
	
	RF_CHANNEL=networkInfo.CH;
	
	//if(networkInfo.OCH == 0x00000000){
		//ChannelMAP = networkInfo.OCH = 0xffffffff;
		//networkInfo.PANID.Val = 0xffffffff;
		//NVM_Write_NetInfo(&networkInfo);			//ihkim	
	//}else{	
		//ChannelMAP = networkInfo.OCH;	
	//}

	//Default Value
	if (networkInfo.BW ==0 || networkInfo.BW==0xff) networkInfo.BW = 7;
	RF_BW = networkInfo.BW;	
	
	if (networkInfo.SF ==0 || networkInfo.SF==0xff) networkInfo.SF = 7;
	SF_DR = networkInfo.SF;	
	
	if (networkInfo.POWER ==0 || networkInfo.POWER==0xff)	networkInfo.POWER = 16;
	RF_OUTPUT = networkInfo.POWER;
		
	//if(networkInfo.echo_chk >0) networkInfo.echo_chk = 1;
	//chkecho = networkInfo.echo_chk;
	
	myPANID = networkInfo.PANID;	
	
	if(networkInfo.PANID.Val == 0xffff){
		myPANID.Val = networkInfo.PANID.Val = 0xabcd;
	}else{
		myPANID.Val = networkInfo.PANID.Val;
	}	
	
	MY_PAN_ID=networkInfo.PANID.Val;
	myPANID.Val=networkInfo.PANID.Val;
	
	myShortAddress.Val = networkInfo.SADDR.Val;
	myParent = networkInfo.myParent;
	
	if(chkecho) printf("networkInfo.echo_chk : %d \n\r",networkInfo.echo_chk);
	if(chkecho) printf("chkecho : %d \n\r",chkecho);
	
	if(networkInfo.Status == 0xff){
		networkInfo.Status = 0;
		wait_nvm_Check = false;
	}else{
		wait_nvm_Check = true;		
	}
	if(chkecho) printf("networkInfo.Status : %d \n\r",networkInfo.Status);
	
	// Channel [20 ~ 32 ( 920.9MHz ~ 923.3MHz )]
	//MiApp_SetChannel(RF_CHANNEL);
	MiApp_SetChannel(networkInfo.CH);
	
	// SignalBw [0:  7.8kHz, 1: 10.4kHz, 2: 15.6kHz, 3: 20.8kHz, 4: 31.25kHz,
	//			 5: 41.7kHz, 6: 62.5kHz, 7: 125kHz,  8: 250kHz,  9: 500kHz, other: Reserved]
	//SX1276LoRaSetSignalBandwidth(7);
	SX1276LoRaSetSignalBandwidth(networkInfo.BW);
	
	// SpreadingFactor [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
	//SX1276LoRaSetSpreadingFactor(7);
	SX1276LoRaSetSpreadingFactor(networkInfo.SF);
//	SX1276LoRaSetSpreadingFactor(SF_DR);
	
	// RF Output Power [20, 17 ~ 02 dBm]
	//SX1276LoRaSetRFPower(12);
	SX1276LoRaSetRFPower(networkInfo.POWER);
//	SX1276LoRaSetRFPower(RF_OUTPUT);
}

#ifdef CONF_PMM_ENABLE
static void app_resources_uninit(void)
{
    /* Disable USART TX and RX Pins */
    struct port_config pin_conf;
    port_get_config_defaults(&pin_conf);
    pin_conf.powersave  = true;
    port_pin_set_config(HOST_SERCOM_PAD0_PIN, &pin_conf);
    port_pin_set_config(HOST_SERCOM_PAD1_PIN, &pin_conf);
    /* Disable UART module */
    sio2host_deinit();
    /* Disable Transceiver SPI Module */
    HAL_RadioDeInit();
}
#endif
/**
 End of File
 */

void start_nvm(void)
{
	int i;
	uint8_t* ptr;	
	
	delay_ms(1000);
	if(wait_nvm_Check){
		if(chkecho) printf("\n\rwait_nvm_Check = true\r\n");
		if(!networkInfo.SADDR.Val){
		// Start NVM Data  - Coordinator = "N"		
			MiApp_ConnectionMode(ENABLE_ALL_CONN);
			if(MiApp_StartConnection(START_FROM_NVM, 0, 0)){
				if(chkecho) printf("Network Created....\n\r");
				MiApp_ConnectionMode(DISABLE_ALL_CONN);
				DumpConnection(0xff);
			}else{
				if(chkecho) printf("Network Start Fail....\n\r");
			}
		}else{
		// Start NVM Data  - End device = "K"
			if( MiApp_EstablishConnection(0, CONN_MODE_NVM) == 0xFF ){
				if(chkecho) printf("Join Fail....\n\r");
			} else {
				if(chkecho) printf("Join Success....4\n\r");
				SX1276_RX_INIT();
				//DumpConnection(0xff);
			}
		}
	}else{
		NVM_Read_NetInfo(&networkInfo);
		RF_CHANNEL = currentChannel = networkInfo.CH;
		
		memset(ConnectionTable,0x00,sizeof(ConnectionTable));
		NVM_Write_ConTable(ConnectionTable);
		
		if(chkecho) printf("\n\rwait_nvm_Check = false\r\n");
	}
	SX1276_RX_INIT();
}

/**
 * \mainpage
 * \section preface Preface
 * This is the reference manual for the LORAWAN Demo Application of EU Band
 */

//#define LED_PA14         							PIN_PA14					//ihkim - ���� test
//#define Switch_PA27       							PIN_PA27  // IN				//ihkim - ���� test
//void user_gpio_init(void)														//ihkim - ���� test
//{
	//struct port_config pin_conf;
	//port_get_config_defaults(&pin_conf);
//
	///* Configure LEDs as outputs, turn them off */
	//pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	//pin_conf.input_pull = PORT_PIN_PULL_UP;
	//port_pin_set_config(LED_PA14, &pin_conf);
	//
	//pin_conf.direction  = PORT_PIN_DIR_INPUT;
	//pin_conf.input_pull = PORT_PIN_PULL_UP;
	//port_pin_set_config(Switch_PA27, &pin_conf);
	//
	//port_pin_set_output_level(LED_PA14, false);
//}

unsigned int Toggle_Count, Toggle_Continue, Toggle_End, Toggle_Old;		
		//ihkim - ���� test
// test__0		
int main(void)
{
	uint8_t* ptr;
	uint8_t j,k;
	uint8_t rlt;
	
	unsigned int Buff_Count;
	
	int i;

    /* System Initialization */	
    system_init();
    /* Initialize the delay driver */
    delay_init();
    /* Initialize the board target resources */
    board_init();
    /* Initialize Hardware and Software Modules */
    driver_init();
    /* ATECC608 Interface */
	edbg_eui_read_eui64((uint8_t *)&myLongAddress[0]);
	/* Initialize the Serial Interface */
		
	NVM_Init();

// test__1	
	NVM_Read_NetInfo(&networkInfo);
	Test_Borate = networkInfo.Uart_Borate;
	if(networkInfo.Uart_Borate == 96){
		Test_Borate = 9600;
	}else if(Test_Borate == 192){
		Test_Borate = 19200;
	}else if(Test_Borate == 384){
		Test_Borate = 38400;
	}else if(Test_Borate == 1152){
		Test_Borate = 115200;
	}else{
		Test_Borate = 115200;
	}
// test__11, echo on/off	
	// chkecho = networkInfo.echo_chk;
	chkecho = 0;

	sio2host_init();					//Terminal Port Setting

    print_reset_causes();
#if (_DEBUG_ == 1)
    SYSTEM_AssertSubscribe(assertHandler);
#endif
	
	printf("%cc",0x1b);  // for clear screen. 2023.12.20. 
	if(chkecho) printf("whybiz project by uttec. 2023.12.20. 20:00\n\r");
	if(chkecho) printf("MAC Address: 0x");
	for(i = 0; i < 8; i++)
	{
		if(chkecho) printf("%.2x",myLongAddress[8-1-i]);
	}
	if(chkecho) printf("\n\r");

	config_init(); //Load Configuration from NVM - Brett

	MiApp_ProtocolInit(false);
	
	//user_gpio_init();																		//ihkim - ���� test
	
////////////////////////////////////////////////////////////////
//#define CERT_MODE 1
#ifdef CERT_MODE
	PrintMenuC();
	while(1){
		ConsoleProcC();
	}
#else
	if(chkecho) PrintMenu();

	start_nvm();
	if(networkInfo.CH == 0){
		networkInfo.CH = 20;					//ihkim
		MiApp_SetChannel(networkInfo.CH);
	}

	chkprotocol = false;
	Toggle_Old = Toggle_Continue;
	
	while(1){
		HandleRxData();

		if(chkprotocol){
			ConsoleProc2();
		}else{
			ConsoleProc();
		}
		test_console();
		
		
		//if(port_pin_get_input_level(Switch_PA27)){					//ihkim - ���� test
			//Toggle_Continue = 1;	
		//}else{
			//Toggle_Continue = 0;
		//}		
		//if(Toggle_Old != Toggle_Continue){
			//Toggle_Old = Toggle_Continue;
			//Toggle_End = 0;			
			//
			//if(!Toggle_Continue){
				//MiApp_FlushTx();
				//MiApp_WriteData('S');
				//MiApp_WriteData('t');
				//MiApp_WriteData('a');
				//MiApp_WriteData('r');
				//MiApp_WriteData('t');
				//
				//if(RF_OUTPUT==20){
				//port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_INACTIVE);//kwlee 200721
				//}else{
				//port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_ACTIVE);//kwlee 200727
				//}
				//
				//MiApp_BroadcastPacket(false);
				//if(chkecho) printf("Broadcast...3\r\n");
				////printf("TEST_Channel %d  %d  %d\n\r",networkInfo.CH,currentChannel,RF_CHANNEL);		//ihkim
				//port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_ACTIVE);//kwlee 200721
				//SX1276_RX_INIT();
			//}else{
				//MiApp_FlushTx();
				//MiApp_WriteData('E');
				//MiApp_WriteData('n');
				//MiApp_WriteData('d');
				//
				//if(RF_OUTPUT==20){
				//port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_INACTIVE);//kwlee 200721
				//}else{
				//port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_ACTIVE);//kwlee 200727
				//}
				//
				//MiApp_BroadcastPacket(false);
				//if(chkecho) printf("Broadcast...3\r\n");
				////printf("TEST_Channel %d  %d  %d\n\r",networkInfo.CH,currentChannel,RF_CHANNEL);		//ihkim
				//port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_ACTIVE);//kwlee 200721
				//SX1276_RX_INIT();
			//}
		//}
		
		
	}
#endif

}