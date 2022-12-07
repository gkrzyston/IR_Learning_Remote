/*
 * controller.h
 *
 *  Created on: Nov 18, 2022
 *      Author: mcprisk
 */

#ifndef SRC_CONTROLLER_H_
#define SRC_CONTROLLER_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ssd1306.h"

typedef struct Commands {
	uint16_t Data[80]; //Change if Datasize changes
	/*
	 * The shortened description is used on the button array displays.
	*/
	char desc[11];
} Command;

typedef struct Devices {
	/*
	 * Commands contain the UART_Data to be sent to devices
	 * And are identified by their number and name
	*/
	Command commands[24];
	/*
	 * The devices name is printed at the top of the touchscreen
	 * and is also used to identify devices in the CLI
	*/
	char name[11];
	/*
	 * The number of buttons associated with this device
	*/
	uint8_t num_button_commands;
} Device;

typedef struct Controllers {
	Device devices[18];
	uint8_t num_devices;
} Controller;

void init_default_controller();
void init_xbee_communication();
void display_device_commands(uint8_t device);

void print_device(uint8_t device);
void print_devices();
void print_command(uint8_t device, uint8_t command);
void print_commands(uint8_t device);

void swap_device(uint8_t a, uint8_t b);
void rename_device(char* new_name, uint8_t device);
void add_device();
void delete_device(uint8_t device);

void export_configuration();

#endif /* SRC_CONTROLLER_H_ */
