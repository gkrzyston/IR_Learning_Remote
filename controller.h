/*
 * controller.h
 *
 *  Created on: Nov 18, 2022
 *      Author: mcprisk
 */

#ifndef SRC_CONTROLLER_H_
#define SRC_CONTROLLER_H_

#include <stdint.h>

typedef struct Commands {
	uint8_t UART_Data;
	/*
	 * The full description is used in the CLI and also
	 * displayed on the touchscreen.
	*/
	char full_desc[16];
	/*
	 * The shortened description is used on the button array displays.
	*/
	char desc[5];
} Command;

typedef struct Devices {
	/*
	 * Commands contain the UART_Data to be sent to devices
	 * Command 0 is the power toggle
	 * Commands 1-9 are accessed on the button array
	 * Commands 10-15 are on the first page of the touchscreen
	 * Commands 16-21 are on the second page
	 * Commands 22-27 on the third page
	*/
	Command commands[28];
	/*
	 * The devices name is printed at the top of the touchscreen
	 * and is also used to identify devices in the CLI
	*/
	char name[32];
	/*
	 * The number of buttons associated with this device
	*/
	uint8_t num_button_commands;
	uint8_t num_touch_commands;
} Device;

typedef struct Controllers {
	Device devices[16];
	uint8_t num_devices;
} Controller;

void init_default_controller(Controller* controller);

void print_device(uint8_t device, Controller* controller);
void print_devices(Controller* controller);
void print_command(uint8_t device, uint8_t command, Controller* controller);
void print_commands(uint8_t device, Controller* controller);

void swap_device(uint8_t a, uint8_t b, Controller* controller);
void rename_device(char* new_name, uint8_t device, Controller* controller);
void add_device(Controller* controller);
void delete_device(uint8_t device, Controller* controller);

void export_configuration(UART_HandleTypeDef * huart, Controller* controller);

#endif /* SRC_CONTROLLER_H_ */
