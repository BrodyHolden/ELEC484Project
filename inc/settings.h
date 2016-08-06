/*
 * settings.h
 *
 *  Created on: Aug 1, 2016
 *      Author: user
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <stdint.h>
#include <assert.h>

// 512 real values interlaced by 512 complex values.
// Real values in even position. Complex values in odd position.
#define WINDOW_SIZE (512 * 2)

#define INPUT_BUFFER_SIZE (WINDOW_SIZE * 8)

static_assert(INPUT_BUFFER_SIZE % 2 == 0, "INPUT_BUFFER_SIZE must be even.");
static_assert(INPUT_BUFFER_SIZE < UINT16_MAX, "Buffer is too large to for uint16_t to index it.");

#endif /* SETTINGS_H_ */
