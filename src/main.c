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

// CMSIS_DSP related includes
#define ARM_MATH_CM4
#define __FPU_PRESENT 1
#include "arm_math.h"
#include "arm_const_structs.h"


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


		// CMSIS DSP related functions.
		// This is all should compile but doesn't actually do anything useful.
		// It's here to test getting the project to build against CMSIS DSP.
		float x_f32[FFT_SIZE];
		float h_f32[FFT_SIZE];
		float y_f32[FFT_SIZE];
		arm_cfft_f32(&arm_cfft_sR_f32_len1024, x_f32, 0, 0);
		arm_cmplx_mult_cmplx_f32(x_f32, h_f32, y_f32, FFT_SIZE);

		int16_t x_q15[FFT_SIZE];
		int16_t h_q15[FFT_SIZE];
		int16_t y_q15[FFT_SIZE];
		arm_cfft_q15(&arm_cfft_sR_q15_len1024, x_q15, 0, 0);
		arm_cmplx_mult_cmplx_q15(x_q15, h_q15, y_q15, FFT_SIZE);
	}
}


