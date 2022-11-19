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
extern unsigned char UART1_rxBuffer[8192];

void init_default_controller(Controller* controller) {
	strcpy(controller->devices[0].commands[0].desc, "PWR");
	strcpy(controller->devices[0].commands[0].full_desc, "Power Toggle");
	controller->devices[0].commands[0].UART_Data = 0x7B;
	char buf[3];
	for (uint8_t i = 1; i <= 9; ++i) {
		itoa(i < 5 ? i : i + 5,buf,10);
		strcpy(controller->devices[0].name, "Example Device 0");
		strcpy(controller->devices[0].commands[i < 5 ? i : i + 5].desc, "CM");
		strncat(controller->devices[0].commands[(i < 5 ? i : i + 5)].desc, buf,strlen(buf));
		strcpy(controller->devices[0].commands[i < 5 ? i : i + 5].full_desc, "Long Desc ");
		strncat(controller->devices[0].commands[(i < 5 ? i : i + 5)].full_desc, buf,strlen(buf));
		controller->devices[0].commands[i < 5 ? i : i + 5].UART_Data = i < 5 ? i : i + 5;
	}
	controller->num_devices = 1;
	controller->devices[0].num_button_commands = 5;
	controller->devices[0].num_touch_commands = 5;

	strcpy(controller->devices[1].commands[0].desc, "PWR");
	strcpy(controller->devices[1].commands[0].full_desc, "Power Toggle");
	controller->devices[1].commands[0].UART_Data = 0x4A;
	for (uint8_t i = 1; i <= 8; ++i) {
		itoa(i < 5 ? i : i + 5,buf,10);
		strcpy(controller->devices[1].name, "Example Device 1");
		strcpy(controller->devices[1].commands[i < 5 ? i : i + 5].desc, "CM");
		strncat(controller->devices[1].commands[(i < 5 ? i : i + 5)].desc, buf,strlen(buf));
		strcpy(controller->devices[1].commands[i < 5 ? i : i + 5].full_desc, "Long Desc ");
		strncat(controller->devices[1].commands[(i < 5 ? i : i + 5)].full_desc, buf,strlen(buf));
		controller->devices[1].commands[i < 5 ? i : i + 5].UART_Data = i < 5 ? i : i + 5;
	}
	controller->num_devices = 2;
	controller->devices[1].num_button_commands = 5;
	controller->devices[1].num_touch_commands = 4;
}

void print_number(uint8_t n) {
	char buf[3];
	itoa(n,buf,10);
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, 1, 0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)deliminator, 1, 0xFFFF);
}

void print_device(uint8_t device, Controller* controller) {
	HAL_UART_Transmit(&huart1, (uint8_t*)(controller->devices[device].name),strlen(controller->devices[device].name),0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)deliminator, 1, 0xFFFF);
}

void print_devices(Controller* controller) {
	for (uint8_t i = 0; i < controller->num_devices; ++i) {
		print_device(i,controller);
	}
}

void print_command(uint8_t device, uint8_t command, Controller* controller) {
	char data_buffer[3];
	itoa(controller->devices[device].commands[command].UART_Data, data_buffer, 16);

	HAL_UART_Transmit(&huart1, (uint8_t*)(controller->devices[device].commands[command].full_desc),strlen(controller->devices[device].commands[command].full_desc),0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)section_deliminator, 1, 0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)(controller->devices[device].commands[command].desc),strlen(controller->devices[device].commands[command].desc),0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)section_deliminator, 1, 0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)data_buffer,strlen(data_buffer),0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)deliminator, 1, 0xFFFF);
}

void print_button_commands(uint8_t device, Controller* controller) {
	for (uint8_t i = 0; i < controller->devices[device].num_button_commands; ++i) {
		print_command(device, i, controller);
	}
}

void print_touch_commands(uint8_t device, Controller* controller) {
	for (uint8_t i = 10; i < 10 + controller->devices[device].num_touch_commands; ++i) {
		print_command(device, i, controller);
	}
}

void print_commands(uint8_t device, Controller* controller) {
	print_button_commands(device, controller);
	print_touch_commands(device, controller);
}

void add_device(Controller* controller) {
	char buf[3];
	char msg[20];
	itoa(controller->num_devices,buf,10);
	strcpy(msg, "Device ");
	strncat(msg, buf, strlen(buf));
	strcpy(controller->devices[controller->num_devices].name,msg);
	controller->devices[controller->num_devices].num_button_commands = 0;
	controller->devices[controller->num_devices].num_touch_commands = 0;
	++(controller->num_devices);
}

void delete_device(uint8_t device, Controller* controller) {
	--(controller->num_devices);
	for (uint8_t i = device; i < controller->num_devices; ++i) {
		swap_device(i, i+1, controller);
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
void export_device(uint8_t device, Controller* controller) {
	print_device(device, controller);

	print_number(controller->devices[device].num_button_commands);
	print_button_commands(device, controller);

	print_number(controller->devices[device].num_touch_commands);
	print_touch_commands(device, controller);
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

void rename_device(char* new_name, uint8_t device, Controller* controller) {
	strcpy(controller->devices[device].name, new_name);
}
void swap_device(uint8_t a, uint8_t b, Controller* controller) {
	Device tmp = controller->devices[a];
	controller->devices[a] = controller->devices[b];
	controller->devices[b] = tmp;
}

void add_command();
void set_command();
void delete_command();
void rename_command();
void swap_command();
void copy_command();

/*
 * Configuration Export Format:
 * # of Devices
 * Device #1 Name
 * Number of Button Commands (including pwr)
 * Command #1 Long Name, Short Name, Data
 * Command #2 Long Name, Short Name, Data
 * ...
 * Number of Touch Commands
 * Command #1 Long Name, Short Name, Data
 * Command #2 Long Name, Short Name, Data
 * (blank line)
 * Device #2 Name
 * ...
*/
void export_configuration(Controller* controller) {
	print_number(controller->num_devices);

	for (uint8_t i = 0; i < controller->num_devices; ++i) {
		export_device(i, controller);
		HAL_UART_Transmit(&huart1, (uint8_t*)deliminator, 1, 0xFFFF);
	}
}
/*
 * Configuration Export Format:
 * # of Devices
 * Device #1 Name
 * Number of Button Commands (including pwr)
 * Command #1 Long Name, Short Name, Data
 * Command #2 Long Name, Short Name, Data
 * ...
 * Number of Touch Commands
 * Command #1 Long Name, Short Name, Data
 * Command #2 Long Name, Short Name, Data
 * (blank line)
 * Device #2 Name
 * ...
*/
void import_configuration();

