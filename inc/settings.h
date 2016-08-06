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

#define REAL_SAMPLES_PER_WINDOW 512

// REAL_SAMPLES_PER_WINDOW real values interlaced by REAL_SAMPLES_PER_WINDOW complex values.
// Real values in even position. Complex values in odd position.
#define WINDOW_SIZE (REAL_SAMPLES_PER_WINDOW * 2)

#define INPUT_BUFFER_SIZE (WINDOW_SIZE * 8)

static_assert(WINDOW_SIZE % 2 == 0, "WINDOW_SIZE must be even.");
static_assert(INPUT_BUFFER_SIZE % 2 == 0, "INPUT_BUFFER_SIZE must be even.");
static_assert(INPUT_BUFFER_SIZE < UINT16_MAX, "Buffer is too large to for uint16_t to index it.");

#endif /* SETTINGS_H_ */
