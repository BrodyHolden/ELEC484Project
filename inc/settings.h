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

#define HALF_REAL_SAMPLES_PER_WINDOW (REAL_SAMPLES_PER_WINDOW / 2)

// REAL_SAMPLES_PER_WINDOW real values interlaced by REAL_SAMPLES_PER_WINDOW complex values.
// Real values in even position. Complex values in odd position.
#define WINDOW_SIZE (REAL_SAMPLES_PER_WINDOW * 2)

#define HALF_WINDOW_SIZE (WINDOW_SIZE / 2)

// If windows are length N, then the FFT values are 2N, since they need to be padded twice as long by zeros.
// This is because we are using FFTs to simulate convolution and convolution with two inputs of length M
// produces output of length 2M.
#define REAL_SAMPLES_PER_FFT (REAL_SAMPLES_PER_WINDOW * 2)
#define FFT_SIZE (REAL_SAMPLES_PER_FFT * 2)

#define INPUT_BUFFER_SIZE (WINDOW_SIZE * 8)

#define OFFSET_BETWEEN_INPUT_AND_OUTPUT_INDICES (WINDOW_SIZE * 5)

#define OFFSET_BETWEEN_OUTPUT_AND_COPY_PTR (WINDOW_SIZE * 4)

static_assert(WINDOW_SIZE % 2 == 0, "WINDOW_SIZE must be even.");
static_assert(HALF_WINDOW_SIZE % 2 == 0, "HALF_WINDOW_SIZE must be even.");
static_assert(INPUT_BUFFER_SIZE % 2 == 0, "INPUT_BUFFER_SIZE must be even.");
static_assert(INPUT_BUFFER_SIZE < UINT16_MAX, "Buffer is too large to for uint16_t to index it.");

#endif /* SETTINGS_H_ */
