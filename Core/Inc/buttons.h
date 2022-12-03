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

// Columns are outputs
#define SW_C2_Port GPIOA
#define SW_C2_Pin  GPIO_PIN_6
#define SW_C1_Port GPIOA
#define SW_C1_Pin  GPIO_PIN_7
#define SW_C0_Port GPIOB
#define SW_C0_Pin  GPIO_PIN_6

// Rows are polled/interrupts
#define SW_R2_Port GPIOB
#define SW_R2_Pin  GPIO_PIN_4
#define SW_R1_Port GPIOB
#define SW_R1_Pin  GPIO_PIN_5
#define SW_R0_Port GPIOB
#define SW_R0_Pin  GPIO_PIN_3

// I2C Port Expander Addresses
#define PX1_ADDR      0xA0
#define IO_CTRL_REG_1 0xF2
#define IO_CTRL_REG_2 0xF3

// Macros
#define C2_HIGH() HAL_GPIO_WritePin(SW_C2_Port, SW_C2_Pin, GPIO_PIN_SET)
#define C2_LOW()  HAL_GPIO_WritePin(SW_C2_Port, SW_C2_Pin, GPIO_PIN_RESET)
#define C1_HIGH() HAL_GPIO_WritePin(SW_C1_Port, SW_C1_Pin, GPIO_PIN_SET)
#define C1_LOW()  HAL_GPIO_WritePin(SW_C1_Port, SW_C1_Pin, GPIO_PIN_RESET)
#define C0_HIGH() HAL_GPIO_WritePin(SW_C0_Port, SW_C0_Pin, GPIO_PIN_SET)
#define C0_LOW()  HAL_GPIO_WritePin(SW_C0_Port, SW_C0_Pin, GPIO_PIN_RESET)

#define R2_READ() HAL_GPIO_ReadPin(SW_R2_Port, SW_R2_Pin)
#define R1_READ() HAL_GPIO_ReadPin(SW_R1_Port, SW_R1_Pin)
#define R0_READ() HAL_GPIO_ReadPin(SW_R0_Port, SW_R0_Pin)

// Enable a button (1 indexed, left-to-right, top-to-bottom)
void enable_button(uint8_t button);

// Disable a button (1 indexed, left-to-right, top-to-bottom)
void disable_button(uint8_t button);

// Toggles a button (1 indexed, left-to-right, top-to-bottom)
void toggle_button(uint8_t button);

// Enable all of the buttons
void enable_all_buttons();

// Disable all of the buttons
void disable_all_buttons();

// Toggle all of the buttons
void toggle_all_buttons();

// Write the updated button configuration to the remote
void update_buttons();

// Poll buttons, returns the first pressed button
// or zero if no button is pressed
uint8_t poll_buttons();

#endif /* INC_BUTTONS_H_ */
