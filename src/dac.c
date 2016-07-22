/*
 * dac.c
 *
 *  Created on: Jul 21, 2016
 *      Author: Tristan
 */

#include "dac.h"

void dacInit (void)
{
	DAC_InitTypeDef 	DAC_InitStruct;
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	GPIO_StructInit( &GPIO_InitStructure );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//
	// Configure DAC channel 1
	//
	DAC_StructInit( &DAC_InitStruct );
	DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
	//DAC_InitStruct.DAC_Trigger = DAC_Trigger_T6_TRGO;
	DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable ;
	DAC_Init(DAC_Channel_1, &DAC_InitStruct );

	DAC_Cmd(DAC_Channel_1, ENABLE);

	DAC->DHR12R1 = 0x10;
}
