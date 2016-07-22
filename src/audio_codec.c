/*
 * audio_codec.c
 *
 *  Created on: Jul 12, 2016
 *      Author: Tristan
 */

#include "audio_codec.h"

//static AudioCallbackFunction *CallbackFunction;
static void *CallbackContext;
static int16_t * volatile NextBufferSamples;
static volatile int NextBufferLength;
static volatile int BufferNumber;
//static volatile bool DMARunning;

void codecInit (void)
{

//	codecGPIOInit();

//	codecI2SInit();

	codecI2CCommandBusInit();

//	Audio44100HzSettings 271,2,6,0
	int plln = 271;
	int pllr = 2;
	int i2sdiv = 6;
	int i2sodd = 0;

	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	I2S_InitTypeDef I2S_InitStructure;		//Struct for I2S init

	OutputPortPinInit(GPIOD, GPIO_Pin_4, RCC_AHB1Periph_GPIOD);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);	// Needed for Audio chip
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// Enable GPIO port A4 as an analog output
	GPIO_StructInit( &GPIO_InitStructure );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_SPI3);

	// Configure C7,C10,C12 from I2S (SPI3)
	GPIO_StructInit( &GPIO_InitStructure );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);

	// Disable I2S.
//	SPI3 ->I2SCFGR = 0;
	SPI_I2S_DeInit(SPI3);
	// I2S clock configuration
	// Disable PLLI2S and wait until is off
	RCC->CR &= ~RCC_CR_PLLI2SON;
	while ( RCC -> CR & RCC_CR_PLLI2SON );

	RCC ->CFGR &= ~RCC_CFGR_I2SSRC; // PLLI2S clock used as I2S clock source.
	RCC ->PLLI2SCFGR = (pllr << 28) | (plln << 6);


	I2S_InitStructure.I2S_AudioFreq = 44100;
	I2S_InitStructure.I2S_Standard = I2S_STANDARD_PHILLIPS;
	I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
	I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
	I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
	I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
	I2S_Init(I2S3ext, &I2S_InitStructure);

	// Configure I2S.
	SPI3 ->I2SPR = i2sdiv | (i2sodd << 8) | SPI_I2SPR_MCKOE;


	// Configure I2S as Master receiver, Phillips mode, 16-bit values, clock polarity low, enable.
	//SPI3 ->I2SCFGR = SPI_I2SCFGR_I2SMOD | SPI_I2SCFGR_I2SCFG_1 | SPI_I2SCFGR_I2SCFG_0
	//		| SPI_I2SCFGR_I2SE;

	// WARNING: In MASTER TRANSMITTER MODE, the STM32F407 will only generate the master clock signal when data is being
	// actively transmitted.  At all other times, the Master Clock (MCLK) signal will be turned off.
	// If you are using the CS43L22 chip, which uses the MCLK to run a charge pump for its analog components during
	// ANALOG PASSTHRU mode this might have unintended consequences.

	// Enable PLLI2S and wait until it is ready.
	RCC ->CR |= RCC_CR_PLLI2SON;
	while (!(RCC ->CR & RCC_CR_PLLI2SRDY ));

	NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	SPI_I2S_ITConfig(I2S3ext, SPI_I2S_IT_TXE, ENABLE);

	I2S_Cmd(I2S3ext, ENABLE);

	// Initialize the CS43L22 system for audio passthrough
	// Reset the CS43L22 chip.
	GPIO_ResetBits(GPIOD, GPIO_Pin_4);
	for( int counter=0; counter < 32000; counter++ );
	GPIO_SetBits(GPIOD,GPIO_Pin_4);

	// Initialization sequence from 4.11 of CS43L22 manual
	// Configure chip for audio passthrough with amplification to headphone jack.
	WriteRegister(0x00, 0x99);
	WriteRegister(0x47, 0x80);
	WriteRegister(0x32, 0x80);
	WriteRegister(0x32, 0x00);
	WriteRegister(0x00, 0x00);

	WriteRegister(0x02, 0x01);
	WriteRegister(0x04, 0xaf);			// HP always on, Speakers always off
	WriteRegister(0x08, 0x01);			// Select AINA1 as input
	WriteRegister(0x09, 0x01);			// Select AINB1 as input
	WriteRegister(0x0e, 0xc0);			// Analog passthrough enabled.
	WriteRegister(0x14, 0xe0);
	WriteRegister(0x15, 0xe0);
	WriteRegister(0x02, 0x9e);			// Activate the chip

}

static void WriteRegister(uint8_t address, uint8_t value) {

	I2C_start(I2C1, 0x94, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	I2C_write(I2C1, address); // write one byte to the slave
	I2C_write(I2C1, value); // write another byte to the slave
	I2C_stop(I2C1); // stop the transmission
}

void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction){
	// wait until I2C1 is not busy anymore
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

	// Send I2C1 START condition
	I2C_GenerateSTART(I2Cx, ENABLE);

	// wait for I2C1 EV5 --> Slave has acknowledged start condition
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

	// Send slave Address for write
	I2C_Send7bitAddress(I2Cx, address, direction);

	/* wait for I2C1 EV6, check if
	 * either Slave has acknowledged Master transmitter or
	 * Master receiver mode, depending on the transmission
	 * direction
	 */
	if(direction == I2C_Direction_Transmitter){
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	}
	else if(direction == I2C_Direction_Receiver){
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	}
}

/* This function transmits one byte to the slave device
 * Parameters:
 *		I2Cx --> the I2C peripheral e.g. I2C1
 *		data --> the data byte to be transmitted
 */
void I2C_write(I2C_TypeDef* I2Cx, uint8_t data)
{
	I2C_SendData(I2Cx, data);
	// wait for I2C1 EV8_2 --> byte has been transmitted
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}
/* This function issues a stop condition and therefore
 * releases the bus
 */
void I2C_stop(I2C_TypeDef* I2Cx){
	// Send I2C1 STOP Condition
	I2C_GenerateSTOP(I2Cx, ENABLE);
}


void codecGPIOInit (void)
{
	//Enable GPIO for codec reset pin
//	OutputPortPinInit(GPIOD, GPIO_Pin_4, RCC_AHB1Periph_GPIOD);

	//Enable GPIO for codec I2C commands
	AltFuncPortPinInit(GPIOB, GPIO_Pin_6, RCC_AHB1Periph_GPIOB);	//SCL
	AltFuncPortPinInit(GPIOB, GPIO_Pin_9, RCC_AHB1Periph_GPIOB);	//SDA

	// Connect pins to I2C peripheral
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

	//Enable GPIO for I2S Data pins.
//	AltFuncPortPinInit(GPIOC, GPIO_Pin_7, RCC_AHB1Periph_GPIOC);	//MCK
//	AltFuncPortPinInit(GPIOC, GPIO_Pin_10, RCC_AHB1Periph_GPIOC);	//SCK
//	AltFuncPortPinInit(GPIOC, GPIO_Pin_12, RCC_AHB1Periph_GPIOC);	//SD
//	AltFuncPortPinInit(GPIOA, GPIO_Pin_4, RCC_AHB1Periph_GPIOA);	//WS

	// Connect pins to I2S peripheral
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_SPI3);
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);

	// Connect pins to I2S peripheral
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_SPI3);
}

void codecI2SInit (void)
{
	I2S_InitTypeDef I2S_InitStructure;		//Struct for I2S init
	NVIC_InitTypeDef   NVIC_InitStructure; 	//Struct for I2S Interrupt init

	//Enable I2S periferal
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

	// Enable the GPIO_LED Clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	// CODEC_I2S peripheral configuration
	SPI_I2S_DeInit(SPI3);
	I2S_InitStructure.I2S_AudioFreq = 48000;
	I2S_InitStructure.I2S_Standard = I2S_STANDARD_PHILLIPS;
	I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
	I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
	I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
	I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
	I2S_Init(I2S3ext, &I2S_InitStructure);
/*
	NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	SPI_I2S_ITConfig(I2S3ext, SPI_I2S_IT_TXE, ENABLE);
*/
	I2S_Cmd(I2S3ext, ENABLE);
}

void codecI2CCommandBusInit (void)
{
	//Code I2C command interface enable
	I2C_InitTypeDef I2C_InitStructure;
	GPIO_InitTypeDef  GPIOInitStruct;

	/* Enable the CODEC_I2C peripheral clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	// Pins B6 and B9 are used for I2C communication, configure as alternate function.
	// We use them to communicate with the CS43L22 chip via I2C
	GPIO_StructInit( &GPIOInitStruct );
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9; // we are going to use PB6 and PB9
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF;			// set pins to alternate function
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;		// set GPIO speed
	GPIOInitStruct.GPIO_OType = GPIO_OType_OD;			// set output to open drain --> the line has to be only pulled low, not driven high
	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// enable pull up resistors
	GPIO_Init(GPIOB, &GPIOInitStruct);

	// The I2C1_SCL and I2C1_SDA pins are now connected to their AF
	// so that the I2C1 can take over control of the
	//  pins
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

	/* CODEC_I2C peripheral configuration */
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	/* Enable the I2C peripheral */

	I2C_Init(I2C1, &I2C_InitStructure);

	I2C_Cmd(I2C1, ENABLE);

}

