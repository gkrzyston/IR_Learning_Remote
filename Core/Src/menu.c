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
#include "controller.h"
#include "buttons.h"
#include "ssd1306.h"

static uint8_t current_page;
extern Menu current_menu;
extern Controller controller;

// Store Menu Commands
static char menu_commands[9][11];

void display_menu(Menu menu, uint8_t page, uint8_t device) {
	if (!page) return;
	current_menu = menu;
	current_page = page;
	switch(menu) {
	case main_menu:
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
		strcpy(menu_commands[0], "PC Connect");
		strcpy(menu_commands[1], "Learn Cmd");
		for (uint8_t i = 2; i <= 7; ++i) {
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
	}
	for (uint8_t i = 1; i <= 9; ++i) {
		erase_buffer();
		draw_string(menu_commands[i-1], white);
		update_display(i);
	}
}

void button_pressed(uint8_t button) {
	static uint8_t in_device_menu = 0;
	if (!button || button > 9) return;

	switch(current_menu) {
	case main_menu:
		switch(button) {
		case 7: // Previous
			display_menu(main_menu, (current_page != 1) ? (current_page - 1) : MAIN_MENU_PAGE_COUNT, 0);
			break;
		case 8: // Settings
			display_menu(settings_menu, 1, 0);
			for (uint8_t i = 3; i <= 8; ++i) {
				disable_button(i);
			}
			update_buttons();
			break;
		case 9: // Next
			display_menu(main_menu, (current_page != MAIN_MENU_PAGE_COUNT) ? (current_page + 1) : 1, 0);
			break;
		default: // Device Selected
			display_menu(device_menu, 1, button + (6 * (current_page - 1)));
			break;
		}
		in_device_menu = 0;
		break;

	case settings_menu:
		switch(button) {
		case 1:
			init_xbee_communication();
			// Inform User that they are in PC mode somehow
			break;
		case 2:
			// Select Device, then command, possibly while flashing the screens.
			// Prompt user to press remote button.
			//display_menu(main_menu, 1, 0);
			break;
		case 9: // Main Menu
			display_menu(main_menu, 1, 0);
			enable_all_buttons();
			break;
		default:
			break;
		}
		in_device_menu = 0;
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
					in_device_menu ? current_page : (button + (6 * (current_page - 1)))
			);
			break;
		default: // Transmit IR Signal based on button!
			break;
		}
		in_device_menu = 1;
		break;
	default:
		break;
	}
}
