/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "main.h"

RCC_ClocksTypeDef RCC_Clocks;

uint8_t UserButtonPressed;
uint8_t ticks;

uint16_t ADCvalue;

uint16_t inputsamples [6500];
uint16_t i;
uint32_t o;

int main(void)
{
	UserButtonPressed = 0;
	ticks = 0;
	i = 0;
	o = 0;

	OutputPortPinInit(GPIOD, LED3_PIN, LED_GPIO_CLK);
	OutputPortPinInit(GPIOD, LED4_PIN, LED_GPIO_CLK);
	OutputPortPinInit(GPIOD, LED5_PIN, LED_GPIO_CLK);
	OutputPortPinInit(GPIOD, LED6_PIN, LED_GPIO_CLK);

	InputPortPinInit(GPIOA, USER_BUTTON_PIN, USER_BUTTON_GPIO_CLK);

	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency/100);

	TIM4_Config();

	ADC3_CH12_Config();

	ADC3->CR2 |= (uint32_t)ADC_CR2_SWSTART;

	//codecInit();

	dacInit();

//	if((I2S3ext->SR && SPI_I2S_FLAG_TXE) == SET)
//	{
//		I2S3ext->DR = inputsamples[o];
//	}

	while(1)
	{
	}
}


