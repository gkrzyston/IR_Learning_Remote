/*
 * buttons.c
 *
 *  Created on: Dec 3, 2022
 *      Author: Matthew Priskorn
 */

#include "buttons.h"

extern I2C_HandleTypeDef hi2c1;

static uint8_t io_ctrl_1 = 0xFF;
static uint8_t io_ctrl_2 = 0x01;

// Enable a button (1 indexed, left-to-right, top-to-bottom)
void enable_button(uint8_t button) {
	// Set IO Reg to 0 to enable the button
	if (button == 9) {
		io_ctrl_2 &= ~(0b1);
	} else if (button <= 8) {
		io_ctrl_1 &= ~(0b1 << (button - 1));
	}
}

// Disable a button (1 indexed, left-to-right, top-to-bottom)
void disable_button(uint8_t button) {
	// Set IO Reg to 1 to disable the button
	if (button == 9) {
		io_ctrl_2 |= 0b1;
	} else if (button <= 8) {
		io_ctrl_1 |= 0b1 << (button - 1);
	}
}

// Toggle a button (1 indexed, left-to-right, top-to-bottom)
void toggle_button(uint8_t button) {
	// Set IO Reg to 1 to disable the button
	if (button == 9) {
		io_ctrl_2 ^= 0b1;
	} else if (button <= 8) {
		io_ctrl_1 ^= 0b1 << (button - 1);
	}
}

// Enable all of the buttons
void enable_all_buttons() {
	for (uint8_t i = 1; i <= 9; ++i) {
		enable_button(i);
	}
}

// Disable all of the buttons
void disable_all_buttons() {
	for (uint8_t i = 1; i <= 9; ++i) {
		disable_button(i);
	}
}

// Toggle all buttons
void toggle_all_buttons() {
	for (uint8_t i = 1; i <= 9; ++i) {
		toggle_button(i);
	}
}

// Write the updated button configuration to the remote
void update_buttons() {
	uint8_t buf[] = {IO_CTRL_REG_1, io_ctrl_1, io_ctrl_2};
	HAL_I2C_Master_Transmit(&hi2c1, PX1_ADDR, buf, 3, HAL_MAX_DELAY);
}

// Poll buttons, returns the first pressed button
// or zero if no button is pressed
uint8_t poll_buttons() {
	C1_HIGH();
	C2_HIGH();
	C0_LOW();
	if (!R0_READ()) return 1;
	if (!R1_READ()) return 4;
	if (!R2_READ()) return 7;
	C0_HIGH();

	C1_LOW();
	if (!R0_READ()) return 2;
	if (!R1_READ()) return 5;
	if (!R2_READ()) return 8;
	C1_HIGH();

	C2_LOW();
	if (!R0_READ()) return 3;
	if (!R1_READ()) return 6;
	if (!R2_READ()) return 9;
	C2_HIGH();

	return 0;
}
