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

#define ARM_MATH_CM4
#define __FPU_PRESENT 1
#include "arm_math.h"
#include "arm_const_structs.h"

#include "impulse_response_arrays.h"

RCC_ClocksTypeDef RCC_Clocks;

uint16_t ADCvalue;

bool g_userButtonPressed;
uint16_t g_inputSamples [INPUT_BUFFER_SIZE];
uint16_t g_inputIndex;
uint16_t g_outputIndex;
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


const uint8_t fftBitReversal = 1;


int main(void)
{
	enableFloatingPointUnit();

	g_userButtonPressed = false;
	g_outputIndex = 0;
	g_inputIndex = OFFSET_BETWEEN_INPUT_AND_OUTPUT_INDICES;
	g_samplesInNewWindow = 0;
	g_hasNewWindow = false;

	const uint16_t* lastElementOfBufferPtr = g_inputSamples + INPUT_BUFFER_SIZE - 1;
	uint16_t* copyPtr1 = g_inputSamples + OFFSET_BETWEEN_OUTPUT_AND_COPY_PTR;
	uint16_t* copyPtr2 = copyPtr1 + HALF_WINDOW_SIZE;

	const arm_cfft_instance_f32* fftOptions = &arm_cfft_sR_f32_len512;

	float x[FFT_SIZE];
	float* const middle_of_x = x + HALF_WINDOW_SIZE;

	float prev_y[FFT_SIZE];
	float prev_x[FFT_SIZE];

	float y[FFT_SIZE];
	float* const middle_of_y = y + HALF_WINDOW_SIZE;

	const int fill = 0;
	memset(x, fill, FFT_SIZE);
	memset(prev_y, fill, FFT_SIZE);
	memset(prev_x, fill, FFT_SIZE);
	memset(y, fill, FFT_SIZE);

	memset(g_inputSamples, fill, INPUT_BUFFER_SIZE);

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
		SPI3->DR = g_inputSamples[g_outputIndex];
	}

	while(1)
	{
		GPIO_PinToggle(GPIOD, LED4_PIN);
		// Spin lock until a windows worth of input is collected.
		while (! g_hasNewWindow);
		g_hasNewWindow = false;

		if (g_userButtonPressed == true) {

			memcpy(x          , copyPtr1, HALF_WINDOW_SIZE);
			memcpy(middle_of_x, copyPtr2, HALF_WINDOW_SIZE);
			// Zero pad
			//memset(x + WINDOW_SIZE, 0, WINDOW_SIZE);

			// TODO Perform reverb here.
			arm_cfft_f32(fftOptions, x, 0, fftBitReversal);

			arm_cmplx_mult_cmplx_f32(x, impulseResponse_0, y, REAL_SAMPLES_PER_WINDOW);

	//		for (int i = 0; i < FFT_SIZE; i++) {
	//			y[i] = y[i] + prev_y[i];
	//		}

			arm_cfft_f32(fftOptions, y, 1, fftBitReversal);

			memcpy(copyPtr1, y          , HALF_WINDOW_SIZE);
			memcpy(copyPtr2, middle_of_y, HALF_WINDOW_SIZE);

		}

		copyPtr1 += HOP_SIZE;
		if (copyPtr1 > lastElementOfBufferPtr) {
			copyPtr1 = g_inputSamples;
		}

		copyPtr2 += HOP_SIZE;
		if (copyPtr2 > lastElementOfBufferPtr) {
			copyPtr2 = g_inputSamples;
		}

	}
}


