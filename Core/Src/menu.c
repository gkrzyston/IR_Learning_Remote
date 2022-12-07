/*
 * menu.c
 *
 *  Created on: Dec 4, 2022
 *      Author: mcprisk
 */

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "menu.h"
#include "IrTransmit.h"
#include "controller.h"
#include "buttons.h"
#include "ssd1306.h"

static uint8_t current_page;
extern Menu current_menu;
extern Controller controller;
extern uint16_t data[];
extern TIM_HandleTypeDef htim2;
extern uint8_t received;
extern uint32_t dataLen;
extern uint8_t timeoutoff;

// Store Menu Commands
static char menu_commands[9][11];

void Learn_command(uint8_t com,uint8_t device);

void display_menu(Menu menu, uint8_t page, uint8_t device) {
	if (!page) return;
	current_menu = menu;
	current_page = page;
	switch(menu) {
	case main_menu:
		enable_all_buttons();
		update_buttons();
		if (page > MAIN_MENU_PAGE_COUNT) return;
		//strcpy(cmd_buf, "Device ");
		//char device_number[3];
		for (uint8_t i = 0; i < 6; ++i) {
			strcpy(menu_commands[i], controller.devices[i + 6*(current_page - 1)].name);
			//itoa(i + (6 * (current_page - 1)), device_number, 10);
			//strcat(cmd_buf, device_number);
			//strcpy(menu_commands[i - 1], cmd_buf);
			//cmd_buf[7] = '\0';
		}
		strcpy(menu_commands[6], "Prev Page");
		strcpy(menu_commands[7], "Settings");
		strcpy(menu_commands[8], "Next Page");
		break;
	case settings_menu:
		enable_all_buttons();
		for (uint8_t i = 5; i <= 8; ++i) {
			disable_button(i);
		}
		update_buttons();
		strcpy(menu_commands[0], "PC Command");
		strcpy(menu_commands[1], "Learn Cmd");
		strcpy(menu_commands[2],"TimeoutOn");
		strcpy(menu_commands[3],"TimeoutOff");	
		for (uint8_t i = 4; i <= 7; ++i) {
			menu_commands[i][0] = '\0';
		}
		strcpy(menu_commands[8], "Main Menu");
		break;
	case device_menu:
		for (uint8_t i = 0; i < 6; ++i) {
			strcpy(menu_commands[i], controller.devices[device].commands[i + 6 * (current_page - 1)].desc);
		}
		strcpy(menu_commands[6], "Prev Page");
		strcpy(menu_commands[7], "Main Menu");
		strcpy(menu_commands[8], "Next Page");
		break;
	case learn_device_menu:
		if (page > MAIN_MENU_PAGE_COUNT) return;
		//strcpy(cmd_buf, "Device ");
		//char device_number[3];
		for (uint8_t i = 0; i < 6; ++i) {
			strcpy(menu_commands[i], controller.devices[i + 6*(current_page - 1)].name);
			//itoa(i + (6 * (current_page - 1)), device_number, 10);
			//strcat(cmd_buf, device_number);
			//strcpy(menu_commands[i - 1], cmd_buf);
			//cmd_buf[7] = '\0';
		}
		strcpy(menu_commands[6], "Prev Page");
		strcpy(menu_commands[7], "Quit");
		strcpy(menu_commands[8], "Next Page");
		break;
	case learn_command_menu:
		for (uint8_t i = 0; i < 6; ++i) {
			strcpy(menu_commands[i], controller.devices[device].commands[i + 6 * (current_page - 1)].desc);
		}
		strcpy(menu_commands[6], "Prev Page");
		strcpy(menu_commands[7], "Devices");
		strcpy(menu_commands[8], "Next Page");
		break;
	case awaiting_menu:
		strcpy(menu_commands[0], "Press");
		strcpy(menu_commands[1], "Button");
		strcpy(menu_commands[2], "On");
		strcpy(menu_commands[3], "External");
		strcpy(menu_commands[4], "Remote");
		for(int i = 5; i<=8; ++i){
			menu_commands[i][0]= '\0';
		}
		break;
	case xbee_menu:
		strcpy(menu_commands[0], "PC");
		strcpy(menu_commands[1], "Command");
		strcpy(menu_commands[2], "Mode");
		for(uint16_t i =3; i<8; ++i){
			menu_commands[i][0]='\0';
		}
		strcpy(menu_commands[8],"Back");
		break;
	}
	for (uint8_t i = 1; i <= 9; ++i) {
		erase_buffer();
		draw_string(menu_commands[i-1], white);
		update_display(i);
	}
}

void button_pressed(uint8_t button) {
	static uint8_t in_device_menu = 0;
	static uint8_t active_device;
	if (!button || button > 9) return;

	switch(current_menu) {
	case main_menu:
		switch(button) {
		case 7: // Previous
			display_menu(main_menu, (current_page != 1) ? (current_page - 1) : MAIN_MENU_PAGE_COUNT, 0);
			break;
		case 8: // Settings
			display_menu(settings_menu, 1, 0);
			break;
		case 9: // Next
			display_menu(main_menu, (current_page != MAIN_MENU_PAGE_COUNT) ? (current_page + 1) : 1, 0);
			break;
		default: // Device Selected
			active_device =  button - 1 + (6 * (current_page - 1));
			display_menu(device_menu, 1, button - 1 + (6 * (current_page - 1)));
			break;
		}
		in_device_menu = 0;
		break;

	case settings_menu:
		switch(button) {
		case 1:
			init_xbee_communication();
			display_menu(xbee_menu,1,0);
			for (uint8_t i = 1; i <= 7; ++i) {
				disable_button(i);
			}	
			update_buttons();
			// Inform User that they are in PC mode somehow
			break;
		case 2:
			// Select Device, then command, possibly while flashing the screens.
			// Prompt user to press remote button.
			display_menu(learn_device_menu, 1, 0);
			enable_all_buttons();
			break;
		case 3:
			timeoutoff = 0;
			display_menu(main_menu, 1,0);
			enable_all_buttons();
			break;
		case 4:
			timeoutoff = 1;
			display_menu(main_menu, 1,0);
			enable_all_buttons();	
			break;
		case 9: // Main Menu
			display_menu(main_menu, 1, 0);
			enable_all_buttons();
			break;
		default:
			break;
		}
		in_device_menu = 0;
		active_device = 0;
		break;

	case device_menu:
		switch(button) {
		case 7: // Previous Page
			display_menu(
					device_menu,
					(current_page != 1) ? (current_page - 1) : DEVICE_MENU_PAGE_COUNT,
					in_device_menu ? current_page : (button + (6 * (current_page - 1)))
			);
			break;
		case 8: // Main Menu
			display_menu(main_menu, 1, 0);
			break;
		case 9: // Next Page
			display_menu(
					device_menu,
					(current_page != DEVICE_MENU_PAGE_COUNT) ? (current_page + 1) : 1,
					in_device_menu ? current_page : (button -1 + (6 * (current_page - 1)))
			);
			break;
		default: // Transmit IR Signal based on button!
			DeviceTransmit(controller.devices[active_device].commands[button - 1 + (6 * (current_page - 1))].Data);
			break;
		}
		in_device_menu = 1;
		break;
	case learn_device_menu:
		switch(button) {
		case 7: // Previous
			display_menu(learn_device_menu, (current_page != 1) ? (current_page - 1) : MAIN_MENU_PAGE_COUNT, 0);
			break;
		case 8: // Settings
			display_menu(settings_menu, 1, 0);
			for (uint8_t i = 3; i <= 8; ++i) {
				disable_button(i);
			}
			update_buttons();
			break;
		case 9: // Next
			display_menu(learn_device_menu, (current_page != MAIN_MENU_PAGE_COUNT) ? (current_page + 1) : 1, 0);
			break;
		default: // Device Selected
			active_device =  button - 1 + (6 * (current_page - 1));
			display_menu(learn_command_menu, 1, button - 1 + (6 * (current_page - 1)));
			break;
		}
		in_device_menu = 0;	
		break;
	case learn_command_menu:
		switch(button) {
		case 7: // Previous Page
			display_menu(
					learn_command_menu,
					(current_page != 1) ? (current_page - 1) : DEVICE_MENU_PAGE_COUNT,
					in_device_menu ? current_page : (button - 1 + (6 * (current_page - 1)))
			);
			break;
		case 8: // Main Menu
			display_menu(learn_device_menu, 1, 0);
			break;
		case 9: // Next Page
			display_menu(
					learn_command_menu,
					(current_page != DEVICE_MENU_PAGE_COUNT) ? (current_page + 1) : 1,
					in_device_menu ? current_page : (button - 1 + (6 * (current_page - 1)))
			);
			break;
		default: // Learn IR Signal based on button!
			disable_all_buttons();
			update_buttons();
			display_menu(awaiting_menu,current_page,0);
			Learn_command(button - 1 + (6 * (current_page - 1)),active_device);
			break;
		}
		in_device_menu = 1;
		break;
	case xbee_menu:
		switch(button){
		case 9:
			display_menu(settings_menu,1,0);
			break;
		default:
			break;
		}
		in_device_menu =0;
		break;
	default:	
		break;
	}
}
void Learn_command(uint8_t com,uint8_t device){	
	received = 0;
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1); //starts timer 2;
	while(!received);
	//writeback
	for(uint32_t i= 0; i<dataLen;++i)
		controller.devices[device].commands[com].Data[i] = data[i];
	enable_all_buttons();
	update_buttons();
	display_menu(main_menu,1,0);
}
