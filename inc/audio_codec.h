/*
 * audio_codec.h
 *
 *  Created on: Jul 12, 2016
 *      Author: Tristan
 */

#ifndef AUDIO_CODEC_H_
#define AUDIO_CODEC_H_

#include "main.h"
#include "GPIOPins.h"

#define I2S_STANDARD_PHILLIPS		0x04
#define CODEC_I2S_ADDRESS           0x40003C0C

//Init functions
void codecInit (void);
void codecGPIOInit (void);
void codecI2SInit (void);
void codecI2CCommandBusInit (void);
//Command functions
void CodecResetCmd(void);
void Delay( __IO uint32_t nCount);

#endif /* AUDIO_CODEC_H_ */
