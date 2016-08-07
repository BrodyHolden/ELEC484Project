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
#include "main.h"

#include "settings.h"
#include <stdbool.h>
#include <string.h>

RCC_ClocksTypeDef RCC_Clocks;

uint8_t UserButtonPressed;

uint16_t ADCvalue;

uint16_t g_inputSamples [INPUT_BUFFER_SIZE];
uint16_t g_inputIndex;
uint16_t g_outputIndex;
uint16_t g_samplesInNewWindow;
bool g_hasNewWindow;

int main(void)
{
	UserButtonPressed = 0;
	g_inputIndex = WINDOW_SIZE * 2;
	g_outputIndex = 0;
	g_samplesInNewWindow = 0;
	g_hasNewWindow = false;

	const uint16_t* lastElementOfBufferPtr = g_inputSamples + INPUT_BUFFER_SIZE - 1;
	uint16_t* copyPtr = g_inputSamples + WINDOW_SIZE;

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

	codecInit();

//	dacInit();

	while(g_inputIndex <= (WINDOW_SIZE * 2));

	if((SPI3->SR && SPI_I2S_FLAG_TXE) == SET)
	{
		SPI3->DR = g_inputSamples[g_outputIndex];
	}

	while(1)
	{
		GPIO_PinToggle(GPIOD, LED4_PIN);
		// Spin lock until a windows worth of input is collected.
		while (! g_hasNewWindow);
		g_hasNewWindow = false;

		uint16_t x[FFT_SIZE];

		memcpy(x, copyPtr, WINDOW_SIZE);
		// Zero pad
		memset(x + WINDOW_SIZE, 0, WINDOW_SIZE);

		// TODO Perform reverb here.

		memcpy(copyPtr, x, WINDOW_SIZE);

		copyPtr += WINDOW_SIZE;
		if (copyPtr > lastElementOfBufferPtr) {
			copyPtr = g_inputSamples;
		}

	}
}


