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
uint16_t g_index;
uint16_t g_samplesInNewWindow;
bool g_hasNewWindow;


void enableFloatingPointUnit(void)
{
	// Enable Floating Point Unit
	// From page 256 of "STM32F3, STM32F4 and STM32L4 Series Cortex(R)-M4 programming manual"
	// http://www.st.com/content/ccc/resource/technical/document/programming_manual/6c/3a/cb/e7/e4/ea/44/9b/DM00046982.pdf/files/DM00046982.pdf/jcr:content/translations/en.DM00046982.pdf
	__asm__ (
		"LDR.W	R0, =0xE000ED88\n\t"
		"LDR	R1, [R0]\n\t"
		"ORR	R1, R1, #(0xF << 20)\n\t"
		"STR	R1, [R0]\n\t"
		"DSB\n\t"
		"ISB"
	);
}


int main(void)
{
	enableFloatingPointUnit();

	UserButtonPressed = 0;
	g_index = 0;
	g_samplesInNewWindow = 0;
	g_hasNewWindow = false;

	const uint16_t* lastElementOfBufferPtr = g_inputSamples + INPUT_BUFFER_SIZE - 1;
	uint16_t* copyPtr = g_inputSamples + OFFSET_BETWEEN_OUTPUT_AND_COPY_PTR;

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

	//while(g_inputIndex <= (WINDOW_SIZE * 2));

	if((SPI3->SR && SPI_I2S_FLAG_TXE) == SET)
	{
		SPI3->DR = g_inputSamples[g_index];
	}

	while(1)
	{
		GPIO_PinToggle(GPIOD, LED4_PIN);
		// Spin lock until a windows worth of input is collected.
		while (! g_hasNewWindow);
		g_hasNewWindow = false;

		uint16_t x[FFT_SIZE];

		memcpy(x, copyPtr, WINDOW_SIZE);

		// TODO Perform reverb here.
		//memset(x, 32768, FFT_SIZE);

		memcpy(copyPtr, x, WINDOW_SIZE);

		copyPtr += (WINDOW_SIZE * 3);
		if (copyPtr > lastElementOfBufferPtr) {
			copyPtr = g_inputSamples;
		}

	}
}


