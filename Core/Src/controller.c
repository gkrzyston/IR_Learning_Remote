/*
 * controller.c
 *
 *  Created on: Nov 18, 2022
 *      Author: mcprisk
 */

#include "controller.h"

static const char* deliminator = "\n";
static const char* section_deliminator = ",";

extern UART_HandleTypeDef huart1;
extern Controller controller;
extern unsigned char UART1_rxBuffer[8192];

void init_default_controller() {
	char device_number[3];
	char cmd_number[3];
	for (uint8_t i = 0; i < 18; ++i) {
		strcpy(controller.devices[i].name, "Device ");
		itoa(i + 1, device_number, 10);
		strcat(controller.devices[i].name, device_number);
		for (uint8_t j = 0; j < 24; ++j) {
			strcpy(controller.devices[i].commands[j].desc, "Command ");
			itoa(j + 1, cmd_number, 10);
			strcat(controller.devices[i].commands[j].desc, cmd_number);
		}
	}
	controller.num_devices = 18;
}

void init_xbee_communication() {
	HAL_UART_Receive_IT(&huart1, UART1_rxBuffer, 1);
}

void print_number(uint8_t n) {
	char buf[3];
	itoa(n,buf,10);
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, 1, 0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)deliminator, 1, 0xFFFF);
}

void print_device(uint8_t device) {
	HAL_UART_Transmit(&huart1, (uint8_t*)(controller.devices[device].name),strlen(controller.devices[device].name),0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)deliminator, 1, 0xFFFF);
}

void print_devices() {
	for (uint8_t i = 0; i < controller.num_devices; ++i) {
		print_device(i);
	}
}

void print_command(uint8_t device, uint8_t command) {
	char data_buffer[3];
	itoa(controller.devices[device].commands[command].UART_Data, data_buffer, 16);

	HAL_UART_Transmit(&huart1, (uint8_t*)(controller.devices[device].commands[command].desc),strlen(controller.devices[device].commands[command].desc),0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)section_deliminator, 1, 0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)data_buffer,strlen(data_buffer),0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)deliminator, 1, 0xFFFF);
}

void print_commands(uint8_t device) {
	for (uint8_t i = 0; i < controller.devices[device].num_button_commands; ++i) {
		print_command(device, i);
	}
}

void add_device() {
	char buf[3];
	char msg[20];
	itoa(controller.num_devices,buf,10);
	strcpy(msg, "Device ");
	strncat(msg, buf, strlen(buf));
	strcpy(controller.devices[controller.num_devices].name,msg);
	controller.devices[controller.num_devices].num_button_commands = 0;
	++(controller.num_devices);
}

void delete_device(uint8_t device) {
	--(controller.num_devices);
	for (uint8_t i = device; i < controller.num_devices; ++i) {
		swap_device(i, i+1);
	}
}

/*
 * Device Export Format:
 * Device Name
 * Number of Button Commands (including pwr)
 * Command #1 Long Name, Short Name, Data
 * Command #2 Long Name, Short Name, Data
 * ...
 * Number of Touch Commands
 * Command #1 Long Name, Short Name, Data
 * Command #2 Long Name, Short Name, Data
 */
void export_device(uint8_t device) {
	print_device(device);

	print_number(controller.devices[device].num_button_commands);
	print_commands(device);
}
/*
 * Device Import Format:
 * Device Name
 * Number of Button Commands (including pwr)
 * Command #1 Long Name, Short Name, Data
 * Command #2 Long Name, Short Name, Data
 * ...
 * Number of Touch Commands
 * Command #1 Long Name, Short Name, Data
 * Command #2 Long Name, Short Name, Data
 */
void import_device();

void rename_device(char* new_name, uint8_t device) {
	strcpy(controller.devices[device].name, new_name);
}
void swap_device(uint8_t a, uint8_t b) {
	Device tmp = controller.devices[a];
	controller.devices[a] = controller.devices[b];
	controller.devices[b] = tmp;
}

void add_command();
void set_command();
void delete_command();
void rename_command(char* new_name, uint8_t device, uint8_t command) {
	strcpy(controller.devices[device].commands[command].desc, new_name);
}
void swap_command();
void copy_command();

/*
 * Configuration Export Format:
 * # of Devices
 * Device #1 Name
 * Number of Button Commands
 * Command #1 Name, Data
 * Command #2 Name, Data
 * (blank line)
 * Device #2 Name
 * ...
*/
void export_configuration() {
	print_number(controller.num_devices);

	for (uint8_t i = 0; i < controller.num_devices; ++i) {
		export_device(i);
		HAL_UART_Transmit(&huart1, (uint8_t*)deliminator, 1, 0xFFFF);
	}
}
/*
 * Configuration Export Format:
 * # of Devices
 * Device #1 Name
 * Number of Button Commands
 * Command #1 Name, Data
 * Command #2 Name, Data
 * (blank line)
 * Device #2 Name
 * ...
*/
void import_configuration();

