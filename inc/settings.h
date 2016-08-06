/*
 * settings.h
 *
 *  Created on: Aug 1, 2016
 *      Author: user
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

// 512 real values interlaced by 512 complex values.
// Real values in even position. Complex values in odd position.
#define WINDOW_SIZE (512 * 2)

#define INPUT_BUFFER_SIZE (WINDOW_SIZE * 8)

#include <assert.h>

static_assert(INPUT_BUFFER_SIZE % 2 == 0, "INPUT_BUFFER_SIZE must be even.");

#endif /* SETTINGS_H_ */
