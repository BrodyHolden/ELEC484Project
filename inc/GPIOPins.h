/*
 * GPIOPins.h
 *
 *  Created on: Jul 5, 2016
 *      Author: Tristan
 */

#ifndef GPIOPINS_H_
#define GPIOPINS_H_

#include "main.h"
#include "stm32f4xx.h"

#define LED4_PIN                         GPIO_Pin_12
#define LED_GPIO_CLK                    RCC_AHB1Periph_GPIOD

#define LED3_PIN                         GPIO_Pin_13

#define LED5_PIN                         GPIO_Pin_14

#define LED6_PIN                         GPIO_Pin_15

#define USER_BUTTON_PIN                GPIO_Pin_0
#define USER_BUTTON_GPIO_CLK           RCC_AHB1Periph_GPIOA

extern uint8_t a, b, c;

void OutputPortPinInit (GPIO_TypeDef * portn, uint32_t pin, uint32_t clk);
void InputPortPinInit(GPIO_TypeDef * portn, uint32_t pin, uint32_t clk);
void AltFuncPortPinInit (GPIO_TypeDef * portn, uint32_t pin, uint32_t clk);
void GPIO_PinOn(GPIO_TypeDef * portn, uint32_t pin);
void GPIO_PinOff(GPIO_TypeDef * portn, uint32_t pin);
void GPIO_PinToggle(GPIO_TypeDef * portn, uint32_t pin);



#endif /* GPIOPINS_H_ */
