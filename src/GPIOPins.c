/*
 * GPIOPins.c
 *
 *  Created on: Jul 5, 2016
 *      Author: Tristan
 */

#include "GPIOPins.h"

void OutputPortPinInit (GPIO_TypeDef * portn, uint32_t pin, uint32_t clk)
{
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  // Enable the GPIO_LED Clock
	  RCC_AHB1PeriphClockCmd(clk, ENABLE);

	  // Configure the GPIO_LED pin
	  GPIO_InitStructure.GPIO_Pin = pin;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(portn, &GPIO_InitStructure);

}

void AltFuncPortPinInit (GPIO_TypeDef * portn, uint32_t pin, uint32_t clk)
{
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  // Enable the GPIO_LED Clock
	  RCC_AHB1PeriphClockCmd(clk, ENABLE);

	  // Configure the GPIO_LED pin
	  GPIO_InitStructure.GPIO_Pin = pin;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(portn, &GPIO_InitStructure);

}
void GPIO_PinOn(GPIO_TypeDef * portn, uint32_t pin)
{
	portn->BSRRL = pin;
}

void GPIO_PinOff(GPIO_TypeDef * portn, uint32_t pin)
{
	portn->BSRRH = pin;
}

void GPIO_PinToggle(GPIO_TypeDef * portn, uint32_t pin)
{
	portn->ODR ^= pin;
}

void InputPortPinInit(GPIO_TypeDef * portn, uint32_t pin, uint32_t clk)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// Enable the BUTTON Clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	// Configure Button pin as input
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Connect Button EXTI Line to Button GPIO Pin
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	// Configure Button EXTI line
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	// Enable and set Button EXTI Interrupt to the lowest priority
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

}
