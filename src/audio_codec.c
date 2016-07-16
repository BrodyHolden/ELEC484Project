/*
 * audio_codec.c
 *
 *  Created on: Jul 12, 2016
 *      Author: Tristan
 */

#include "audio_codec.h"


void codecInit (void)
{

	codecGPIOInit();

	codecI2SInit();

	codecI2CCommandBusInit();

	CodecResetCmd();
}

void codecGPIOInit (void)
{
	//Enable GPIO for codec reset pin
	OutputPortPinInit(GPIOD, GPIO_Pin_4, RCC_AHB1Periph_GPIOD);

	//Enable GPIO for codec I2C commands
	AltFuncPortPinInit(GPIOB, GPIO_Pin_6, RCC_AHB1Periph_GPIOB);	//SCL
	AltFuncPortPinInit(GPIOB, GPIO_Pin_9, RCC_AHB1Periph_GPIOB);	//SDA

	// Connect pins to I2C peripheral
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

	//Enable GPIO for I2S Data pins.
	AltFuncPortPinInit(GPIOC, GPIO_Pin_7, RCC_AHB1Periph_GPIOC);	//MCK
	AltFuncPortPinInit(GPIOC, GPIO_Pin_10, RCC_AHB1Periph_GPIOC);	//SCK
	AltFuncPortPinInit(GPIOC, GPIO_Pin_12, RCC_AHB1Periph_GPIOC);	//SD
	AltFuncPortPinInit(GPIOA, GPIO_Pin_4, RCC_AHB1Periph_GPIOA);	//WS

	// Connect pins to I2S peripheral
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);

	// Connect pins to I2S peripheral
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_SPI3);
}

void codecI2SInit (void)
{
	I2S_InitTypeDef I2S_InitStructure;		//Struct for I2S init
	NVIC_InitTypeDef   NVIC_InitStructure; 	//Struct for I2S Interrupt init

	//Enable I2S periferal
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

	// CODEC_I2S peripheral configuration
	SPI_I2S_DeInit(SPI3);
	I2S_InitStructure.I2S_AudioFreq = 44100;
	I2S_InitStructure.I2S_Standard = I2S_STANDARD_PHILLIPS;
	I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
	I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
	I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
	I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
	I2S_Init(I2S3ext, &I2S_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	SPI_I2S_ITConfig(I2S3ext, SPI_I2S_IT_TXE, ENABLE);

	I2S_Cmd(I2S3ext, ENABLE);
}

void codecI2CCommandBusInit (void)
{
	//Code I2C command interface enable
	I2C_InitTypeDef I2C_InitStructure;

	/* Enable the CODEC_I2C peripheral clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	/* CODEC_I2C peripheral configuration */
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x33;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	/* Enable the I2C peripheral */
	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);

}

void CodecResetCmd(void)
{
  /* Power Down the codec */
  GPIO_WriteBit(GPIOD, GPIO_Pin_4, Bit_RESET);

  /* wait for a delay to insure registers erasing */
  Delay(0x4FFF);

  /* Power on the codec */
  GPIO_WriteBit(GPIOD, GPIO_Pin_4, Bit_SET);
}

//STMs dirty delay from their code.
void Delay( __IO uint32_t nCount)
{
  for (; nCount != 0; nCount--);
}
