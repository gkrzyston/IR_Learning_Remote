/*
 * buttons.h
 *
 *  Created on: Dec 3, 2022
 *      Author: Matthew Priskorn
 */

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

// Enable a button (1 indexed, left-to-right, top-to-bottom)
void enable_button(uint8_t button);

// Disable a button (1 indexed, left-to-right, top-to-bottom)
void disable_button(uint8_t button);

// Enable all of the buttons
void enable_all_buttons();

// Disable all of the buttons
void disable_all_buttons();

// Write the updated button configuration to the remote
void update_buttons();

#endif /* INC_BUTTONS_H_ */
