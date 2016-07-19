/*
 * adc.h
 *
 *  Created on: Jul 10, 2016
 *      Author: Tristan
 */

#ifndef ADC_H_
#define ADC_H_

#include "main.h"
#include "stm32f4xx.h"
#include "stm32f4xx_adc.h"

/*
 * 	void ADC3_CH12_Config(void)
 * 	Author: Tristan
 *
 * 	This function initializes ADC3 on the stm32f4 using channel 12.
 * 	The function enables the RCC timers for both the ADc peripheral
 * 	as well as the GPIO pin associated with the channel selected. The
 * 	NVIC global interrupts for the ADC are also enabled here.
 *
 * 	Peripheral:	ADC3, CH12
 * 	Pin:		PC2
 * 	Mode:		Independent
 * 				prescaler/2
 * 				DMA Disabled
 * 				Two Sample Delay
 * 				Left justified
 * 				EOC interrupt enabled
 */
void ADC3_CH12_Config(void);

#endif /* ADC_H_ */
